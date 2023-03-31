#include "universe.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#define ROW     0
#define DELAY   50000
#define OPTIONS "tsn:i:o:h"

int main(int argc, char **argv) {

    FILE *inputf = stdin;
    FILE *outputf = stdout;
    bool toroidal = false;
    uint32_t gen = 100;
    bool ncurses = false;

    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 'o': outputf = fopen(optarg, "w"); break;
        case 'n': gen = (uint32_t) strtoul(optarg, NULL, 100); break;
        case 's': ncurses = true; break;
        case 'i': inputf = fopen(optarg, "r"); break;
        case 'h':
            printf("please refer to README.md for instructions on how to run this program\n");
            break;
        }
    }

    uint32_t rows;
    uint32_t cols;

    int fif = fscanf(inputf, "%d %d\n", &rows, &cols);
    if (fif != 2) {
        printf("error");
    }

    Universe *univ_a = uv_create(rows, cols, toroidal); //create univ a
    uv_populate(univ_a, inputf); //populate univ a from inputf

    Universe *univ_b = uv_create(rows, cols, toroidal); //create univ b

    if (!ncurses) {
        initscr();
        curs_set(FALSE);
    }

    for (uint32_t x = 0; x < gen; x++) {
        for (uint32_t r = 0; r < uv_rows(univ_a); r++) {
            for (uint32_t c = 0; c < uv_cols(univ_a); c++) {
                //implement the rules of the game using functions from universe.c
                /*
                        1. any live cell w 2-3 neighbours survives\
                        2. any dead cell w 3 neighbours becomes live\
                        3. all other cells die
                        */
                //use uv_get_cell to check whether r&c are out of bounds\ aka if cell is live

                if (uv_get_cell(univ_a, r, c) == true
                    && (uv_census(univ_a, r, c) == 2 || uv_census(univ_a, r, c) == 3)) {
                    uv_live_cell(univ_b, r, c);
                } else if (((uv_get_cell(univ_a, r, c) == false)
                               && (uv_census(univ_a, r, c) == 3))) {
                    uv_live_cell(univ_b, r, c);
                } else {
                    uv_dead_cell(univ_b, r, c);
                }
            }
        }
        Universe *temp = univ_b;
        univ_b = univ_a;
        univ_a = temp;


        if (!ncurses) {
            clear();
            for (uint32_t r = 0; r < uv_rows(univ_a); r++) {
                for (uint32_t c = 0; c < uv_cols(univ_a); c++) {
                    if (uv_get_cell(univ_a, r, c) == true) {
                        mvprintw(r, c, "o");
                    }
                }
            }

            refresh();
            usleep(DELAY);
        }

    }
    if (!ncurses) {
        endwin();
    }

    uv_print(univ_a, outputf);
    fclose(inputf);
    fclose(outputf);
    uv_delete(univ_a);
    uv_delete(univ_b);

    return 0;
}
