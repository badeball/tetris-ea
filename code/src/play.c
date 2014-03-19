#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#include "feature_functions.h"
#include "feature_helpers.h"
#include "genotype.h"
#include "options.h"
#include "phenotype.h"
#include "random.h"

#define MASTER_RANK 0

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
        printf("  %s N (defaults to 0)\n", features[i].name);
    }
}

int main (int argc, char **argv) {
    clock_t begin = clock(),
            end;

    program_name = argv[0];

    struct options opt = {
        .seedp              = seed(),
        .n_features_enabled = 0,
        .n_weights_enabled  = 0,
        .n_trials           = 10,
        .board_width        = 10,
        .board_height       = 20,
        .print_board        = 0,
        .n_piece_lookahead  = 0,
    };

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
        } else if (feature_exists(argv[i])) {
            enable_feature(feature_index(argv[i++]), &opt);
        } else {
            printf("Unknown argument '%s'.\n", argv[i]);
            return 1;
        }
    }

    initialize_dynamic_weight_numbers(&opt);

    struct phenotype * phenotype =
        initialize_phenotype(
            initialize_genotype(&opt));

    int weight_i = 0;

    for (int i = 0; i < opt.n_features_enabled; i++) {
        phenotype->genotype->feature_enabled[i] = 1;
    }

    for (int i = 1; i < argc; i++) {
        if (feature_exists(argv[i])) {
            for (int a = 0; a < features[feature_index(argv[i])].weights; a++) {
                phenotype->genotype->feature_weights[weight_i++] = atof(argv[i + a + 1]);
            }

            i += features[feature_index(argv[i])].weights;
        }
    }

    if (opt.n_features_enabled == 0) {
        printf("No features defined.\n");
        return 0;
    }

    printf("The following phenotype has been initialized.\n");
    write_phenotype(stdout, phenotype, &opt);
    phenotype_fitness(phenotype, &opt);
    free_phenotype(phenotype);
}