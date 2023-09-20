#include "universe.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_GENERATIONS 100
#define SLEEP_TIME          50000

int main(int argc, char *argv[]) {
    // Command line arguments
    bool toroidal = false;
    bool silence = false;
    uint32_t generations = DEFAULT_GENERATIONS;
    FILE *input = stdin;
    FILE *output = stdout;

    // Parse command line options
    int opt;
    while ((opt = getopt(argc, argv, "tsn:i:o:")) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': silence = true; break;
        case 'n':
            generations = (uint32_t) atoi(optarg);
            if (generations < 0) {
                fprintf(stderr, "Error: Invalid number of generations\n");
                return 1;
            }
            break;
        case 'i':
            input = fopen(optarg, "r");
            if (input == NULL) {
                fprintf(stderr, "Error: Input file %s does not exist\n", optarg);
                return 1;
            }
            break;
        case 'o': output = fopen(optarg, "w"); break;
        default:
            printf("Usage: %s [-t] [-s] [-n generations] [-i input] [-o output]\n", argv[0]);
            return 1;
        }
    }

    // Read number of rows and columns
    uint32_t rows, cols;
    fscanf(input, "%u%u", &rows, &cols);

    if (ferror(input) || feof(input)) {
        fprintf(stderr, "Error: Input file is empty\n");
        return 1;
    }

    //printf("%u", cols);

    // Create two universes
    Universe *universe_a = uv_create(rows, cols, toroidal);
    Universe *universe_b = uv_create(rows, cols, toroidal);
    //printf("TESTING");

    // Populate universe A
    uv_populate(universe_a, input);
    //uv_print(universe_a, stdout);

    // Setup ncurses screen
    if (silence == false) {
        initscr();
        //cbreak();
        //noecho();
        //nodelay(stdscr, true);
        curs_set(FALSE);
    }

    //printw("TESTING");
    // Perform generations
    for (uint32_t i = 0; i < generations; i++) {
        if (silence == false) {
            clear();
            uv_print(universe_a, stdout);
            //refresh();
            //usleep(SLEEP_TIME);
        }

        // Take census of each cell in universe A
        for (uint32_t r = 0; r < rows; r++) {
            for (uint32_t c = 0; c < cols; c++) {
                uint32_t census = uv_census(universe_a, r, c);
                if (uv_get_cell(universe_a, r, c)) {
                    if (silence == false) {
                        mvprintw(r, c, "o");
                    }
                    //refresh();
                    //usleep(SLEEP_TIME);
                    // Live cell
                    if (census < 2 || census > 3) {
                        uv_dead_cell(universe_b, r, c);
                    } else {
                        uv_live_cell(universe_b, r, c);
                    }
                } else {
                    if (census == 3) {
                        uv_live_cell(universe_b, r, c);
                    } else {
                        uv_dead_cell(universe_b, r, c);
                    }
                }
            }
        }
        if (silence == false) {
            refresh();
            usleep(SLEEP_TIME);
        }
        // Swap universes
        Universe *tmp = universe_a;
        universe_a = universe_b;
        universe_b = tmp;
    }

    // Clean up
    if (silence == false) {
        endwin();
    }
    uv_print(universe_a, output);
    uv_delete(universe_a);
    uv_delete(universe_b);
    fclose(input);
    fclose(output);
    return 0;
}
