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
        .n_features_enabled = 0,
        .n_weights_enabled  = 0,
        .n_trials           = 10,
        .board_width        = 10,
        .board_height       = 20,
        .print_board        = 0,
        .n_piece_lookahead  = 0,
    };

    initalize_rng(&opt);

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

    if (opt.print_board) {
        phenotype_fitness(phenotype, &opt);
    } else {
        int rank;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank == MASTER_RANK) {
            printf("The following phenotype has been initialized.\n");
            write_phenotype(stdout, phenotype, &opt);

            int n_workers,
                n_trials_scheduled = 0,
                n_workers_let_down = 0;

            unsigned long long total_cleared_lines = 0;

            MPI_Comm_size(MPI_COMM_WORLD, &n_workers);

            while (n_workers - 1 > n_workers_let_down) {
                int request;

                MPI_Status status;

                MPI_Recv(
                    &request, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                    MPI_COMM_WORLD, &status);

                if (request == -1) {
                    // A worker is requestion permission to perform a calculations.

                    int result;

                    if (opt.n_trials > n_trials_scheduled) {
                        // The worker is given permission to perform a calculation.
                        n_trials_scheduled++;
                        result = 1;
                    } else {
                        // The worker is denied permission to perform a calculation.
                        n_workers_let_down++;
                        result = 0;
                    }

                    MPI_Send(
                        &result, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                } else {
                    // A worker is returning a result of a calculation.
                    total_cleared_lines += request;
                }
            }

            end = clock();

            double seconds = (double)(end - begin) / CLOCKS_PER_SEC;

            if (opt.n_trials > 1) {
                printf("The average score of %d trials is %d.\n", opt.n_trials, total_cleared_lines / opt.n_trials);
                printf("Total cleared lines was %'llu.\n", total_cleared_lines);
            }

            printf("Execution finished after %.2f seconds.\n", seconds);
            printf("This amounts to %.2f cleared lines per second.\n", total_cleared_lines / seconds);
        } else {
            while (1) {
                int request = -1,
                    result;

                MPI_Send(
                    &request, 1, MPI_INT, MASTER_RANK, 0, MPI_COMM_WORLD);

                MPI_Recv(
                    &result, 1, MPI_INT, MASTER_RANK, MPI_ANY_TAG,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                if (result == 1) {
                    // Permission is given to perform a calculation.
                    int score = phenotype_fitness(phenotype, &opt);

                    printf("A play resulted in a score of %d.\n", score);

                    MPI_Send(
                        &score, 1, MPI_INT, MASTER_RANK, 0, MPI_COMM_WORLD);
                } else {
                    // Permission is denied and the worker can terminate.
                    break;
                }
            }
        }

        MPI_Finalize();
    }

    free_phenotype(phenotype);
}