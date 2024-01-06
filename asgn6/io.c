#include "io.h"
#include "code.h"
#include "endian.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

//global buffer used by read_sym, write_word, and flush_words
static uint8_t word_buffer[BLOCK];
static uint8_t pair_buffer[BLOCK];

//used by write_pair and flush pair. keeps track of current bit in buffer.
static int bit_index = 0;

//
// Read up to to_read bytes from infile and store them in buf. Return the number of bytes actually
// read.
//
// Since read() may not read in as many bytes as you asked for, this function should continuously
// call read() and attempt to read as many bytes as it has not yet read. For instance, if to_read is
// 100 and the first read() call only reads 20 bytes, it should attempt to read 80 bytes the next
// time it calls read().
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int bytes_read = 0;
    int curr_read = read(infile, buf, to_read);
    while (to_read != 0 && curr_read > 0) {
        bytes_read += curr_read;
        to_read -= curr_read;
        curr_read = read(infile, buf, to_read);
    }

    return bytes_read;
}

//
// Write up to to_write bytes from buf into outfile. Return the number of bytes actually written.
//
// Similarly to read_bytes, this function will need to call write() in a loop to ensure that it
// writes as many bytes as possible.
//
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int bytes_written = 0;
    int curr_written = write(outfile, buf, to_write);
    while (to_write != 0 && curr_written > 0) {
        bytes_written += curr_written;
        to_write -= curr_written;
        curr_written = write(outfile, buf, to_write);
    }

    return bytes_written;
}

//
// Read a file header from infile into *header.
//
// This function need not create any additional buffer to store the contents of the file header.
// File headers, like any struct or any value in C, are just represented by bytes in memory which
// means you can use read_bytes to read however many bytes the header consumes (use sizeof!) into
// *header.
//
// Since we decided that the canonical byte order for our headers is little-endian, this function
// will need to swap the byte order of both the header fields if it is run on a big-endian system.
// For example, here is how the 4 bytes of the magic number will look when written to the file:
//
// +------+------+------+------+
// | 0xAC | 0xBA | 0xAD | 0xBA |
// +------+------+------+------+
//
// A big-endian computer would interpret those bytes as the 4-byte number 0xACBAADBA, which is
// not what we want, so you would have to change the order of those bytes in memory. A little-endian
// computer will interpret that as 0xBAADBAAC.
//
// This function should also make sure the magic number is correct. Since it has no return value you
// may call assert() to do that, or print out an error message and exit the program, or use some
// other way to report the error.
//
void read_header(int infile, FileHeader *header) {
    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
    //checks if bros an eldian (aot refrence lmao)
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
    assert(header->magic == MAGIC);
}

//
// Write a file header from *header to outfile. Like above, this function should swap the byte order
// of the header's two fields if necessary.
//
void write_header(int outfile, FileHeader *header) {
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }

    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
}

//
// Read one symbol from infile into *sym. Return true if a symbol was successfully read, false
// otherwise.
//
// Reading one symbol at a time is slow, so this function will need to maintain a global buffer
// (an array) of BLOCK bytes. Most calls will only need to read a symbol out of that buffer, and
// then update some counter so that the function knows what position in the buffer it is at. If
// there are no more bytes in the buffer for it to return, it will have to call read_bytes to refill
// the buffer with fresh data. If this call fails then you cannot read a symbol and should return
// false.
//
bool read_sym(int infile, uint8_t *sym) {
    static uint32_t counter;
    *sym = 0;
    //fills buffer if buffer is empty or if entire buffer has been read
    if (counter == 0) {
        //checks if there anymore of the file to read
        if (read_bytes(infile, word_buffer, BLOCK) == 0) {
            return false;
        }
    }

    //read sym from buffer to sym pointer
    if (counter == BLOCK) {
        if (read_bytes(infile, word_buffer, BLOCK) == 0) {
            return false;
        }
        counter = 0;
    }
    *sym = word_buffer[counter];
    counter += 1;
    return true;
}

//
// Write a pair -- bitlen bits of code, followed by all 8 bits of sym -- to outfile.
//
// This function should also use a buffer. It writes into individual bits in the buffer, starting
// with the least significant bit of the first byte, until the most significant bit of the first
// byte, and then the least significant bit of the second byte, and so on. You will need bitwise
// arithmetic to manipulate individual bits within your buffer -- consult asgn3/set.c if you don't
// remember how to do this.
//
// The first bit of code to be written is the least significant bit, and the same holds for sym.
//
// This function will need to track which *bit* in the buffer will be written to next. If it ever
// reaches the end of the buffer it needs to write out the contents of the buffer to outfile; you
// may use flush_pairs to do this.
//
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {

    //writing code into buffer
    for (int i = 0; i < bitlen; i++) {
        //grabs ith bit from code
        int bit = (code >> i) & 1;

        //writes bit into the buffer. only changes bit of the buffer with OR operator
        pair_buffer[bit_index / 8] |= (bit << (bit_index % 16));
        bit_index += 1;

        //check if buffer is full. Run flush_pairs and reset index if yea
        if (bit_index == BLOCK * 8) {
            flush_pairs(outfile);
            bit_index = 0;
        }
    }

    //same process as writing code to buffer but with sym instead
    for (int i = 0; i < 8; i++) {
        int bit = (sym >> i) & 1;
        pair_buffer[bit_index / 8] |= (bit << (bit_index % 8));
        bit_index += 1;
        if (bit_index == BLOCK * 8) {
            flush_pairs(outfile);
            bit_index = 0;
        }
    }
}

//
// Write any pairs that are in write_pair's buffer but haven't been written yet to outfile.
//
// This function will need to be called at the end of encode since otherwise those pairs would never
// be written. You don't have to, but it would be easy to make this function also work when called
// by write_pair, since otherwise you would write essentially the same code in two places.
//
// If not all bits of the last byte in your buffer have been written to, make sure that the
// unwritten bits are set to zero. An easy way to do this is by zeroing the entire buffer after
// flushing it every time.
//
void flush_pairs(int outfile) {
    //writing full pair_buffer to outfile
    write_bytes(outfile, pair_buffer, BLOCK);

    if (bit_index % 8 != 0) {
        write_bytes(outfile, pair_buffer, 1);
        bit_index = 0;
    }

    //zeroing entire buffer
    memset(pair_buffer, 0, BLOCK);
}

//
// Read bitlen bits of a code into *code, and then a full 8-bit symbol into *sym, from infile.
// Return true if the complete pair was read and false otherwise.
//
// Like write_pair, this function must read the least significant bit of each input byte first, and
// will store those bits into the LSB of *code and of *sym first.
//
// It may be useful to write a helper function that reads a single bit from a file using a buffer.
//
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    *code = 0;
    *sym = 0;
    //checks if there anything to read from infile and puts in buffer
    if (bit_index == 0) {
        if (read_bytes(infile, pair_buffer, BLOCK) == 0) {
            return false;
        }
    }

    //reads code bits to code
    for (int i = 0; i < bitlen; i++) {
        //checks if buffer is full. read to buffer and reset index
        if (bit_index == BLOCK) {
            if (read_bytes(infile, pair_buffer, bitlen) == 0) {
                return false;
            }
            bit_index = 0;
        }
        //grabs ith bit from buffer
        int bit = (pair_buffer[bit_index / 8] >> i) & 1;

        //write bit to code
        *code |= (uint16_t) bit << i;
        bit_index += 1;
    }

    //reads sym bits to sym
    for (int i = 0; i < 8; i++) {

        if (bit_index == BLOCK) {
            if (read_bytes(infile, pair_buffer, 8) == 0) {
                return false;
            }
            bit_index = 0;
        }

        int bit = (pair_buffer[bit_index / 8] >> i) & 1;

        *sym |= (uint8_t) bit << i;
        bit_index += 1;
    }
    //checks if code is STOP_CODE
    if (*code == STOP_CODE) {
        return false;
    }

    return true;
}

//
// Write every symbol from w into outfile.
//
// These symbols should also be buffered and the buffer flushed whenever necessary (note you will
// likely sometimes fill up your buffer in the middle of writing a word, so you cannot only check
// that the buffer is full at the end of this function).
//
void write_word(int outfile, Word *w) {
    static int index_counter = 0;
    //writing word to buffer
    for (uint32_t i = 0; i < w->len; i++) {
        word_buffer[index_counter] = w->syms[i];
        index_counter += 1;

        //check if buffer is full
        if (index_counter == BLOCK) {
            flush_words(outfile);
            index_counter = 0;
        }
    }
}

//
// Write any unwritten word symbols from the buffer used by write_word to outfile.
//
// Similarly to flush_pairs, this function must be called at the end of decode since otherwise you
// would have symbols remaining in the buffer that were never written.
//
void flush_words(int outfile) {
    //writing full word_buffer to outfile
    write_bytes(outfile, word_buffer, BLOCK);
}
