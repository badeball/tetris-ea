#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "feature.h"
#include "genotype.h"
#include "options.h"
#include "phenotype.h"

char* program_name;

void print_help_text () {
    printf(
        "usage: %s [options]\n"
        "Options:\n"
        "  --n-trials N (defaults to 10)\n"
        "  --board-width N (defaults to 10)\n"
        "  --board-height N (defaults to 20)\n"
        "  --n-piece-lookahead N (defaults to 0)\n"
        "                      The number of tetrominos that the controller will see\n"
        "                      ahead of the current tetromino. Higher means that the\n"
        "                      controller can take more informed choices, but it will\n"
        "                      result in significantly higher computation times.\n"
        "  -i                  print parsable boards\n",
        program_name
    );

    for (int i = 0; i < N_FEATURES; i++) {
        printf("  %s N (defaults to 0)\n", feature_name(i));
    }
}

int main (int argc, char **argv) {
    program_name = argv[0];

    struct options opt = {
        .seedp              = time(NULL),
        .n_features_enabled = 0,
        .n_trials           = 10,
        .board_width        = 10,
        .board_height       = 20,
        .print_board        = 0,
        .n_piece_lookahead  = 0,
    };

    struct phenotype * phenotype =
        initialize_phenotype(
            initialize_genotype(&(struct options) { .n_features_enabled = N_FEATURES }));

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 ||
            strcmp(argv[i], "--help") == 0) {
            print_help_text();
            return 0;
        } else if (strcmp(argv[i], "--n-trials") == 0) {
            opt.n_trials = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--board-width") == 0) {
            opt.board_width = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--board-height") == 0) {
            opt.board_height = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--n-piece-lookahead") == 0) {
            opt.n_piece_lookahead = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-i") == 0) {
            opt.print_board = 1;
        } else if (index_feature(argv[i]) >= 0) {
            enable_feature(index_feature(argv[i]), &opt);
            phenotype->genotype->feature_weights[opt.n_features_enabled - 1] = atof(argv[++i]);
            phenotype->genotype->feature_enabled[opt.n_features_enabled - 1] = 1;
        } else {
            printf("Unknown argument '%s'.\n", argv[i]);
            return 1;
        }
    }

    if (opt.n_features_enabled == 0) {
        printf("No features defined.\n");
        return 0;
    }

    if (opt.print_board) {
        phenotype_fitness(phenotype, &opt);
    } else {
        printf("The following phenotype has been initialized.\n");
        write_phenotype(stdout, phenotype, &opt);

        int sum = 0;

        for (int i = 0; i < opt.n_trials; i++) {
            int score = phenotype_fitness(phenotype, &opt);

            printf("Play %d. resulted in a score of %d.\n", i + 1, score);

            sum += score;
        }

        printf("The average score of %d trials is %d.\n", opt.n_trials, sum / opt.n_trials);
    }

    free_phenotype(phenotype);
}