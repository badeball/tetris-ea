#include <string.h>

#include "board.h"
#include "feature_functions.h"
#include "feature_helpers.h"
#include "options.h"

char* program_name;

void print_help_text () {
    printf(
        "usage: %s [options]\n"
        "Options:\n"
        "  -p                  print out the board received from stdin\n"
        "  --f-all             analyze all features (default if none are specified)\n",
        program_name
    );

    for (int i = 0; i < N_FEATURES; i++) {
        printf("  %s\n", features[i].name);
    }
}

int main (int argc, char **argv) {
    program_name = argv[0];

    struct options opt = {
        .n_features_enabled = 0,
        .n_weights_enabled = 0,
    };

    int p_board = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 ||
            strcmp(argv[i], "--help") == 0) {
            print_help_text();
            return 0;
        } else if (strcmp(argv[i], "-p") == 0) {
            p_board = 1;
        } else if (feature_exists(argv[i])) {
            enable_feature(feature_index(argv[i]), &opt);
        } else if (strcmp(argv[i], "--f-all") == 0) {
            for (int a = 0; a < N_FEATURES; a++) {
                enable_feature(a, &opt);
            }
        } else {
            printf("Unknown argument '%s'.\n", argv[i]);
            return 1;
        }
    }

    if (opt.n_features_enabled == 0) {
        for (int a = 0; a < N_FEATURES; a++) {
            enable_feature(a, &opt);
        }
    }

    struct board board;

    if (read_board(&board) == 1) {
        printf("Unable to properly read board from standard input.\n");
        return 1;
    }

    opt.board_width = board.width;
    opt.board_height = board.height;

    initialize_dynamic_weight_numbers(&opt);

    if (p_board) {
        print_board(stdout, &board);
    }

    printf("These are the values of the given board for the specified features.\n");

    int max_feature_length = 0;

    for (int i = 0; i < N_FEATURES; i++) {
        if (opt.feature_enabled[i]) {
            if (strlen(features[i].name) > max_feature_length) {
                max_feature_length = strlen(features[i].name);
            }
        }
    }

    for (int i = 0; i < N_FEATURES; i++) {
        if (opt.feature_enabled[i]) {
            for (int a = 0; a < features[i].weights; a++) {
                printf(
                    "%-*s % .1f\n",
                    max_feature_length,
                    features[i].name,
                    (features[i].function) (&board, &board, &((struct t_last_placement) {
                        .tetromino = &tetrominos[0],
                        .x = 0,
                        .y = 0,
                    })));
            }
        }
    }

    return 0;
}