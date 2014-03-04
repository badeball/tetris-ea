#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#include "feature.h"
#include "genotype.h"
#include "options.h"
#include "phenotype.h"

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
        printf("  %s N (defaults to 0)\n", feature_name(i));
    }
}

int main (int argc, char **argv) {
    clock_t begin = clock(),
            end;

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
        int rank;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        opt.seedp += rank * 7; // Mulitple of 7 gives the addition some moment.

        if (rank == MASTER_RANK) {
            printf("The following phenotype has been initialized.\n");
            write_phenotype(stdout, phenotype, &opt);

            int sum = 0,
                n_workers,
                n_trials_scheduled = 0,
                n_workers_let_down = 0;

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
                    sum += request;
                }
            }

            end = clock();

            double seconds = (double)(end - begin) / CLOCKS_PER_SEC;

            if (opt.n_trials > 1) {
                printf("The average score of %d trials is %d.\n", opt.n_trials, sum / opt.n_trials);
                printf("Total cleared lines was %'llu.\n", sum);
            }

            printf("Execution finished after %.2f seconds.\n", seconds);
            printf("This amounts to %.2f cleared lines per second.\n", sum / seconds);
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