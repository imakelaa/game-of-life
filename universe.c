#include "universe.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
} Universe;

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *univ = (Universe *) calloc(1, sizeof(Universe));
    univ->rows = rows;
    univ->cols = cols;
    univ->toroidal = toroidal;

    univ->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r += 1) {
        univ->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    return univ;
}

bool out_of_bounds(Universe *u, uint32_t r, uint32_t c) {
    return (c >= u->cols || r >= u->rows);
}

void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i++) {
        free(u->grid[i]);
        u->grid[i] = NULL;
    }
    free(u->grid);
    u->grid = NULL;
    free(u);
    u = NULL;
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (out_of_bounds(u, r, c) == false) {
        u->grid[r][c] = true;
    }
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (out_of_bounds(u, r, c) == false) {
        u->grid[r][c] = false;
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (out_of_bounds(u, r, c) == true) {
        return false;
    } else {
        return u->grid[r][c];
    }
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r = u->rows;
    uint32_t c = u->cols;
    while ((fscanf(infile, "%d %d\n", &r, &c)) != EOF) {
        if (out_of_bounds(u, r, c) == true) {
            return true;
        }
        uv_live_cell(u, r, c);
    }
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    //check if toroidal or not
    //every toroidal has 8 neighbours
    uint32_t counter = 0;
    for (int32_t x = -1; x < 2; x++) {
        for (int32_t y = -1; y < 2; y++) {
            if (x == 0 && y == 0) {
                continue;
            }

            uint32_t row = r + x;
            uint32_t col = c + y;
            if (u->toroidal == true) { //this is only if -t input in command line
                row += u->rows;
                row %= u->rows; //is only for non corner
                col += u->cols;
                col %= u->cols; //is only for non corner
            }

            counter += uv_get_cell(
                u, row, col); //update r and c here to update the grid and move to new cell in grid
        }
    }
    return counter;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t rows = 0; rows < u->rows; rows++) {
        for (uint32_t cols = 0; cols < u->cols; cols++) {
            if (uv_get_cell(u, rows, cols) == true) {
                fputc('o', outfile);
            } else {
                fputc('.', outfile);
            }
        }
        fputc('\n', outfile);
    }
}
