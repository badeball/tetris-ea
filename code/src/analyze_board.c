#include <string.h>

#include "board.h"
#include "feature.h"
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
        printf("  %s\n", feature_name(i));
    }
}

int main (int argc, char **argv) {
    program_name = argv[0];

    struct options opt = {
        .n_features_enabled = 0,
    };

    int p_board = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 ||
            strcmp(argv[i], "--help") == 0) {
            print_help_text();
            return 0;
        } else if (strcmp(argv[i], "-p") == 0) {
            p_board = 1;
        } else if (index_feature(argv[i]) >= 0) {
            enable_feature(index_feature(argv[i]), &opt);
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

    if (p_board) {
        print_board(stdout, &board);
    }

    printf("These are the values of the given board for the specified features.\n");

    int max_feature_length = 0;

    for (int i = 0; i < N_FEATURES; i++) {
        if (opt.feature_enabled[i]) {
            if (strlen(feature_name(i)) > max_feature_length) {
                max_feature_length = strlen(feature_name(i));
            }
        }
    }

    for (int i = 0; i < N_FEATURES; i++) {
        if (opt.feature_enabled[i]) {
            printf(
                "%-*s %d\n",
                max_feature_length, 
                feature_name(i),
                (feature_function(i)) (&board, &((struct t_placement) {
                    .tetromino = &tetrominos[0],
                    .x = 0,
                    .y = 0,
                })));
        }
    }

    return 0;
}