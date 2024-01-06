#include "universe.h"

#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

//creates universe
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) calloc(1, sizeof(Universe));
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;

    //allocating memory for grid
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r++) {
        u->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }

    return u;
}

//deleting universe and freeing memory
void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i++) {
        free(u->grid[i]);
    }
    free(u->grid);
    free(u);
}

//returns rows of universe
uint32_t uv_rows(Universe *u) {
    return u->rows;
}

//returns columns of universe
uint32_t uv_cols(Universe *u) {
    return u->cols;
}

//assigns given cell to true
void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = true;
}

//assigns given cell to false
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = false;
}

//returns bool of given cell whether its alive/dead
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r > u->rows || c > u->cols) {
        return false;
    }
    return u->grid[r][c];
}

//populates universe of living cells from coords of given File
bool uv_populate(Universe *u, FILE *infile) {
    uint32_t row, column;
    while (fscanf(infile, "%u%u", &row, &column) != EOF) {
        //checking if coords are valid
        if (row < 0 || row > u->rows || column < 0 || column > u->cols) {
            return false;
        }
        uv_live_cell(u, row, column);
    }
    return true;
}

//returns number of living neighbors of given cell
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t neighbors = 0; //neighbor count
    int ri, ci; //changing uint to int
    ri = r;
    ci = c;

    //checking if toroidal or not
    if (u->toroidal) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                neighbors += u->grid[(u->rows + i + ri) % u->rows][(u->cols + j + ci) % u->rows];
            }
        }

    }
    //if not toroidal
    else {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (r + i >= 0 && r + i <= u->rows - 1 && ci + j >= 0 && c + j <= u->cols - 1) {
                    neighbors += u->grid[ri + i][ci + j];
                }
            }
        }
    }
    //removes middle cell if it was counted as living neighbor
    if (u->grid[r][c]) {
        neighbors -= 1;
    }
    return neighbors;
}

//prints universe to given file
void uv_print(Universe *u, FILE *outfile) {

    //iterating over every cell
    for (uint32_t i = 0; i < u->rows; i++) {
        for (uint32_t j = 0; j < u->cols; j++) {
            if (u->grid[i][j]) {
                fprintf(outfile, "o");
            } else {
                fprintf(outfile, ".");
            }
        }
        fprintf(outfile, "\n");
    }
}
