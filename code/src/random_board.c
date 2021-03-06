#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "board.h"
#include "random.h"
#include "tetromino.h"
#include "feature_functions.h"

char* program_name;

void print_help_text () {
    printf(
        "usage: %s [options]\n"
        "Options:\n"
        "  -h or --help        show this help message and exit\n"
        "  -n                  number of random pieces to place (defaults to 10)\n",
        program_name
    );
}

int main (int argc, char **argv) {
    program_name = argv[0];

    int n_pieces = 10;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 ||
            strcmp(argv[i], "--help") == 0) {
            print_help_text();
            return 0;
        } else if (strcmp(argv[i], "-n") == 0) {
            n_pieces = atoi(argv[++i]);
        } else {
            printf("Unknown argument '%s'.\n", argv[i]);
            return 1;
        }
    }

    struct options opt;

    initalize_rng(&opt);

    struct board board = initialize_board();

    for (int i = 0; i < n_pieces; i++) {
        int tetromino_i = random_tetromino(&opt);

        struct tetromino tetromino = tetrominos[tetromino_i];

        place_tetromino(
            &board,
            &tetromino,
            l_rand(0, BOARD_WIDTH - 4 + 1 + tetromino.p_left + tetromino.p_right, &opt) - tetromino.p_left,
            NULL);
    }

    print_board(stdout, &board);

    free_board(&board);
}