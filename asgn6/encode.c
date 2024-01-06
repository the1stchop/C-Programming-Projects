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
        printf("   Compresses files using the LZ78 compression algorithm.\n");
        printf("   Compressed files are decompressed with the corresponding decoder.\n\n");
        printf("USAGE\n");
        printf("   ./encode [-vh] [-i input] [-o output]\n\n");
        printf("OPTIONS\n");
        printf("   -v          Display compression statistics\n");
        printf("   -i input    Specify input to compress (stdin by default)\n");
        printf("   -o output   Specify output of compressed input (stdout by default)\n");
        printf("   -h          Display program help and usage\n");
        return 0;
    }
    printf("is this shit working?");
    FileHeader fileheader;
    fileheader.magic = MAGIC;

    struct stat stats;
    fstat(infile, &stats);
    fileheader.protection = stats.st_mode;

    //write filehead to file
    write_header(outfile, &fileheader);

    //create trienode
    TrieNode *root = trie_create();
    //copy of root
    TrieNode *curr_node = root;

    //counter to keep track of next available code
    uint16_t next_code = START_CODE;

    //variables to keep track of previously read trie node and symbol
    TrieNode *prev_node;
    uint8_t prev_sym;

    uint8_t curr_sym;
    TrieNode *next_node;
    //loop to read in all symbols from infile. Stops when read_sym returns false
    while (read_sym(infile, &curr_sym)) {
        printf("shitter");
        next_node = trie_step(curr_node, curr_sym);

        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, sizeof(next_code) * 8);
            curr_node->children[curr_sym] = trie_node_create(next_code);
            //curr_bide->chilren[curr_sym] is new node, set to new root node
            curr_node = curr_node->children[curr_sym];
            next_code += 1;
        }

        //check if next_code is equal to MAX_CODE
        if (next_code == MAX_CODE) {
            trie_reset(root);
        }

        //update preve_sym to be curr_sym
        prev_sym = curr_sym;
    }

    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, sizeof(next_code) * 8);
        if (next_code % MAX_CODE != 0) {
            next_code += 1;
        }
    }

    write_pair(outfile, STOP_CODE, 0, sizeof(next_code) * 8);

    flush_pairs(outfile);

    close(infile);
    close(outfile);

    if (v == 1) {
        printf("oopsies not finished :(");
    }
}
