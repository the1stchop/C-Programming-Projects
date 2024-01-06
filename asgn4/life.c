#include "universe.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "tsn:i:o:h"

int main(int argc, char **argv) {
    int opt = 0;

    //defualt settings
    uint32_t gens = 100;
    bool t = false;
    int h = 0;
    int s = 0;
    FILE *in_file = stdin;
    FILE *out_file = stdout;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': t = true; break;
        case 's': s = 1; break;
        case 'n': gens = atoi(optarg); break;
        case 'i': in_file = fopen(optarg, "r"); break;
        case 'o': out_file = fopen(optarg, "r"); break;
        case 'h': h = 1; break;
        }
    }
    //help
    if (h == 1) {
        printf("SYNOPSIS\n");
        printf("    Conway's Game of Life\n\n");
        printf("USAGE\n");
        printf("    ./life_amd64 tsn:i:o:h\n\n");
        printf("OPTIONS\n");
        printf("    -t             Create your universe as a toroidal\n");
        printf("    -s             Silent - do not use animate the evolution\n");
        printf("    -n {number}    Number of generations [default: 100]\n");
        printf("    -i {file}      Input file [default: stdin]\n");
        printf("    -o {file}      Output file [default: stdout]\n");
        return 0;
    }

    int rows, cols;
    //checking if fil is valid
    if (in_file == NULL) {
        printf("File is empty\n");
        return 0;
    }

    //checking if dimensions are valid
    fscanf(in_file, "%d%d", &rows, &cols);
    if (rows < 1 || cols < 1) {
        printf("invalid universe dimensions. Check Rows and Columns\n");
        return 0;
    }

    //creating universes
    Universe *uni_A = uv_create(rows, cols, t);
    Universe *uni_B = uv_create(rows, cols, t);
    if (uv_populate(uni_A, in_file) == 0) {
        printf("file coordinates invalid\n");
        return 0;
    }

    //setting up ncurses screen
    if (s == 0) {
        initscr();
        curs_set(FALSE);
    }
    for (uint32_t g = 0; g < gens; g++) { //looping over number of generations
        clear(); //clears window
        for (uint32_t r = 0; r < uv_rows(uni_A); r++) { //looping through every cell
            for (uint32_t c = 0; c < uv_cols(uni_A); c++) {
                //apply Game of Life rules
                if (uv_census(uni_A, r, c) == 3
                    || (uv_census(uni_A, r, c) == 2 && uv_get_cell(uni_A, r, c))) {
                    uv_live_cell(uni_B, r, c);
                } else {
                    uv_dead_cell(uni_B, r, c);
                }

                if (s == 0) { //display universe if ncurses isn't silenced
                    if (uv_get_cell(uni_B, r, c)) {
                        mvprintw(r, c, "o");
                    }
                }
            }
        }
        if (s == 0) { //refresh and all that if ncurse = true
            refresh();
            usleep(50000);
        }
        //swapping universes
        Universe *temp = uni_A;
        uni_A = uni_B;
        uni_B = temp;
    }
    if (s == 0) {
        endwin(); //closing screen
    }

    //printing final generation to outfile
    uv_print(uni_A, out_file);

    //freeing allocated memory
    uv_delete(uni_A);
    uv_delete(uni_B);

    fclose(in_file);
    fclose(out_file);
}
