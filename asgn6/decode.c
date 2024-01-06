#include "word.h"
#include "trie.h"
#include "io.h"
#include "code.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define OPTIONS "hvi:o:"

int main(int argc, char **argv) {
    int opt = 0;

    //default settings
    int v = 0;
    int h = 0;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': v = 1; break;
        case 'h': h = 1; break;
        case 'i': infile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC); break;
        }
    }

    if (h == 1) {
        printf("SYNOPSIS\n");
        printf("   Decompresses files using the LZ78 decompression algorithm.\n");
        printf("   Used with files compressed with the corresponding encoder.\n\n");
        printf("USAGE\n");
        printf("   ./decode [-vh] [-i input] [-o output]\n\n");
        printf("OPTIONS\n");
        printf("   -v          Display decompression statistics\n");
        printf("   -i input    Specify input to decompress (stdin by default)\n");
        printf("   -o output   Specify output of decompressed input (stdout by default)\n");
        printf("   -h          Display program help and usage\n");
        return 0;
    }
    //read file header
    FileHeader fileheader;
    read_header(infile, &fileheader);

    //create new word table
    WordTable *table = wt_create();

    //variables to keep track of current code and next code
    uint16_t curr_code;
    uint16_t next_code = START_CODE;

    uint8_t curr_sym;

    read_pair(infile, &curr_code, &curr_sym, sizeof(next_code) * 8);
    while (curr_code != STOP_CODE) {
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(outfile, table[next_code]);
        next_code += 1;
        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }
    flush_words(outfile);

    close(infile);
    close(outfile);

    if (v == 1) {
        printf("oopsies didnt finished D:");
    }
}
