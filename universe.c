#include "universe.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//typedef value nameofType

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) calloc(1, sizeof(Universe));
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t i = 0; i < rows; i++) {
        u->grid[i] = (bool *) calloc(cols, sizeof(bool));
    }
    return u;
}

void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i++) {
        free(u->grid[i]);
    }
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    //if (r < u->rows && c < u->cols) {
    u->grid[r][c] = true;
    //}
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    //if (r < u->rows && c < u->cols) {
    u->grid[r][c] = false;
    //}
}

uint32_t isNeighbor(Universe *u, uint32_t r, uint32_t c) {
    if (r >= 0 && r < u->rows && c >= 0 && c < u->cols) {
        return u->grid[r][c];
    }
    return 0;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= u->rows || c >= u->cols) {
        return false;
    }
    return u->grid[r][c];
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t numNeighbors = 0;

    for (int row = -1; row <= 1; row++) {
        for (int col = -1; col <= 1; col++) {
            if (row != 0 || col != 0) {
                int nr = r + row;
                int nc = c + col;
                if (u->toroidal) {
                    nr = (nr + u->rows) % u->rows;
                    nc = (nc + u->cols) % u->cols;
                }
                numNeighbors += isNeighbor(u, nr, nc);
                //numNeighbors += isNeighbor(u, r + row, c + col);
            }
        }
    }
    return numNeighbors;
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r, c;
    while (fscanf(infile,
               "%d"
               "%d",
               &r, &c)
           == 2) {
        uv_live_cell(u, r, c);
    }
    return true;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t i = 0; i < u->rows; i++) {
        for (uint32_t j = 0; j < u->cols; j++) {
            if (u->grid[i][j]) {
                fputc('o', outfile);
            } else {
                fputc('.', outfile);
            }
        }
        fputc('\n', outfile);
    }
}
