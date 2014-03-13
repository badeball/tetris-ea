#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <signal.h>

#include "board.h"
#include "feature_functions.h"
#include "feature_helpers.h"
#include "genotype.h"
#include "options.h"
#include "phenotype.h"
#include "population.h"
#include "selection.h"
#include "tetromino.h"
#include "threading.h"

#define PRINT_V(string, ...) if (opt.verbose) { printf(string, ##__VA_ARGS__); }

struct options opt = {
    .n_features_enabled    = 0,
    .n_weights_enabled     = 0,

    .verbose               = 0,
    .population_size       = 20,
    .tournament_group_size = 10,
    .max_n_generations     = 100,
    .crossover_points      = 2,
    .elitism               = 0,
    .multi_threading       = 0,
    .n_cores               = 0,
    .no_log                = 0,
    .no_change_duration    = 50,
    .reset_volume          = 0,
    .n_trials              = 10,
    .board_width           = 10,
    .board_height          = 20,
    .print_board           = 0,
    .n_piece_lookahead     = 0,
    .randomization_range   = 100,
    .mutation_range        = 100,

    .feature_enable_rate               = 1.f / 6.f,
    .mutation_rate                     = 0.995,
    .crossover_rate                    = 0.5,
    .tournament_group_random_selection = 0.1,

    .selection = TOURNAMENT,
};

char* program_name;

int user_exit = 0;

void print_help_text () {
    printf(
        "usage: %s [options]\n"
        "Options:\n"
        "  -h or --help        show this help message and exit\n"
        "  -v or --verbose\n"
        "  -i                  read problem instance from standard input\n"
        "  --pupulation-size N (defaults to 20)\n"
        "  --tournament-group-size N (defaults to 10)\n"
        "  --tournament-group-random-selection P (defaults to 0.1)\n"
        "                      Don't select the best individual in the tournament\n"
        "                      with a probability of P. A low P will increase\n"
        "                      selection pressure, while a high P will decrease it.\n"
        "  --max-n-generations N (defaults to 100)\n"
        "  --elitism N (defaults to 0)\n"
        "                      keep the N best individuals for next generation\n"
        "  --mutation-rate F (defaults to 0.995)\n"
        "  --crossover-rate F (defaults to 0.5)\n"
        "  --crossover-points N (defaults to 2)\n"
        "                      set equal to genotype size to do uniform crossover\n"
        "  --selection {TOURNAMENT, SUS} (defaults to TOURNAMENT)\n"
        "                      which method of parent selection to use\n"
        "  --no-change-duration N (defaults to 50)\n"
        "                      If above zero, the EA will reset itself if no change\n"
        "                      in the winning individual has occurred. The number\n"
        "                      of individuals to be reset is given by --reset-volume.\n"
        "  --reset-volume N (defaults to 0)\n"
        "                      The number of individuals to reset if no change has\n"
        "                      happened during the no-change-duration. If set to 0,\n"
        "                      all individuals will be reset.\n"
        "  --n-trials N (defaults to 10)\n"
        "                      The number of trials to run in order to determine the\n"
        "                      average fitness score of an individual.\n"
        "  --board-width N (defaults to 10)\n"
        "  --board-height N (defaults to 20)\n"
        "  --n-piece-lookahead N (defaults to 0)\n"
        "                      The number of tetrominos that the controller will see\n"
        "                      ahead of the current tetromino. Higher means that the\n"
        "                      controller can take more informed choices, but it will\n"
        "                      result in significantly higher computation times.\n"
        "  --randomization-range N (defaults to 100)\n"
        "                      Determines the range of value a weight can obtain during\n"
        "                      randomization which occurs at initialization.\n"
        "  --mutation-range n (defaults to 100)\n"
        "                      Determines the range of value that is added to a weight\n"
        "                      during mutation.\n"
        "  --feature-enable-rate F (defaults to 1 / 6)\n"
        "                      Determines the probability of a feature to be enabled\n"
        "                      during randomization and mutation. The reason for a\n"
        "                      seemingly low number is that there is quite a lot of\n"
        "                      features.\n"
        "  --multi-threading   calculate fitness values in parallel by multi-threading\n"
        "  -l or --log-dir     specify the location for run logs\n"
        "  --no-log            do not log results (-i or --no-log needs to be defined)\n"
        "\n"
        "Additionally, the following arguments can be used to enable features.\n"
        "\n"
        "  --f-all             enables all features\n",
        program_name
    );

    for (int i = 0; i < N_FEATURES; i++) {
        printf("  %s\n", features[i].name);
    }
}

void sigint_handle (int signal) {
    printf("Interrupting..\n");
    user_exit = 1;
}

int main (int argc, char **argv) {
    clock_t begin = clock(),
            end;

    opt.seedp = time(NULL);

    program_name = argv[0];

    int log_directory_defined = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 ||
            strcmp(argv[i], "--verbose") == 0) {
            opt.verbose = 1;
        } else if (strcmp(argv[i], "--population-size") == 0) {
            opt.population_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--tournament-group-size") == 0) {
            opt.tournament_group_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--tournament-group-random-selection") == 0) {
            opt.tournament_group_random_selection = atof(argv[++i]);
        } else if (strcmp(argv[i], "--max-n-generations") == 0) {
            opt.max_n_generations = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--mutation-rate") == 0) {
            opt.mutation_rate = atof(argv[++i]);
        } else if (strcmp(argv[i], "--crossover-rate") == 0) {
            opt.crossover_rate = atof(argv[++i]);
        } else if (strcmp(argv[i], "--crossover-points") == 0) {
            opt.crossover_points = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--selection") == 0) {
            i++;
            if (strcmp(argv[i], "TOURNAMENT") == 0) {
                opt.selection = TOURNAMENT;
            } else if (strcmp(argv[i], "SUS") == 0) {
                opt.selection = SUS;
            } else if (strcmp(argv[i], "SIGMA") == 0) {
                opt.selection = SIGMA;
            } else {
                printf("Unknown option for selection '%s'.\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "--no-change-duration") == 0) {
            opt.no_change_duration = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--reset-volume") == 0) {
            opt.reset_volume = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--n-trials") == 0) {
            opt.n_trials = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--board-width") == 0) {
            opt.board_width = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--board-height") == 0) {
            opt.board_height = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--n-piece-lookahead") == 0) {
            opt.n_piece_lookahead = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--randomization-range") == 0) {
            opt.randomization_range = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--mutation-range") == 0) {
            opt.mutation_range = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--feature-enable-rate") == 0) {
            opt.feature_enable_rate = atof(argv[++i]);
        } else if (strcmp(argv[i], "--elitism") == 0) {
            opt.elitism = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--multi-threading") == 0) {
            opt.multi_threading = 1;
        } else if (strcmp(argv[i], "-l") == 0 ||
            strcmp(argv[i], "--log-dir") == 0) {
            log_directory_defined = 1;
            opt.log_directory = argv[++i];
        } else if (strcmp(argv[i], "--no-log") == 0) {
            opt.no_log = 1;
        } else if (strcmp(argv[i], "-h") == 0 ||
            strcmp(argv[i], "--help") == 0) {
            print_help_text();
            return 0;
        } else if (feature_exists(argv[i]) || strcmp(argv[i], "--f-all") == 0) {
            // No operation. These are handled below.
        } else {
            printf("Unknown argument '%s'.\n", argv[i]);
            return 1;
        }
    }

    initialize_dynamic_weight_numbers(&opt);

    for (int i = 1; i < argc; i++) {
        if (feature_exists(argv[i])) {
            enable_feature(feature_index(argv[i]), &opt);
        } else if (strcmp(argv[i], "--f-all") == 0) {
            for (int a = 0; a < N_FEATURES; a++) {
                enable_feature(a, &opt);
            }
        }
    }

    if (opt.crossover_points > opt.n_features_enabled) {
        opt.crossover_points = opt.n_features_enabled;
    }

    if (!log_directory_defined && !opt.no_log) {
        printf("Missing log directory. Terminating.\n");
        return 1;
    }

    FILE * f;

    char time_format[50];
    char run_log_directory[200];
    char run_log_data_file[261];
    char run_log_solution_file[262];

    if (!opt.no_log) {
        time_t timer;
        struct tm* tm_info;
        time(&timer);
        tm_info = localtime(&timer);

        strftime(time_format, 50, "%Y-%m-%d-%H%M%S", tm_info);

        if (snprintf(run_log_directory, 200, "%s/%s", opt.log_directory, time_format) >= 200) {
            printf("Unable to allocate run log directory. Terminating.\n");
            return 1;
        }

        mkdir(run_log_directory, 0755);

        if (snprintf(run_log_data_file, 261, "%s/%s", run_log_directory, "fitness.dat") >= 261) {
            printf("Unable to allocate data log path. Terminating.\n");
            return 1;
        }

        if (snprintf(run_log_solution_file, 261, "%s/%s", run_log_directory, "solution.dat") >= 262) {
            printf("Unable to allocate solution log path. Terminating.\n");
            return 1;
        }

        PRINT_V("Results will be logged to %s.\n", run_log_data_file);

        f = fopen(run_log_data_file, "a");
    }

    struct population* children = initialize_population_pool(opt.population_size);
    struct population* parents = initialize_population_pool(opt.population_size);

    for (int i = 0; i < opt.population_size; i++) {
        children->individuals[i] = initialize_phenotype(initialize_genotype(&opt));
        parents->individuals[i] = initialize_phenotype(initialize_genotype(&opt));

        randomize_genotype(children->individuals[i]->genotype, &opt);
    }

    PRINT_V("A random population has been initialized.\n\n");

    if (opt.multi_threading) {
        set_number_of_cores(&opt);
    }

    signal(SIGINT, sigint_handle);

    struct phenotype* winner = initialize_phenotype(initialize_genotype(&opt));
    winner->fitness = -1;

    unsigned long long total_cleared_lines = 0;

    for (int i = 0; (i < opt.max_n_generations || opt.max_n_generations == 0) && !user_exit; i++) {
        PRINT_V("Simulating generation %d.\n", i + 1);

        for (int a = 0; a < opt.population_size; a++) {
            free_phenotype(parents->individuals[a]);
        }

        swap_populations(&children, &parents);

        struct phenotype** parent_pairs = select_parent_pairs(parents, &opt);

        for (int a = 0; a < opt.population_size - opt.elitism; a++) {
            children->individuals[a] = mate_individuals(
                parent_pairs[a * 2 + 0],
                parent_pairs[a * 2 + 1],
                &opt);
        }

        free(parent_pairs);

        for (int a = 0; a < opt.population_size - opt.elitism; a++) {
            children->individuals[a]->fitness = average_phenotype_fitness(children->individuals[a], &opt);
            total_cleared_lines += children->individuals[a]->fitness * opt.n_trials;
        }

        if (opt.elitism) {
            qsort(parents->individuals, parents->size, sizeof(struct phenotype*), compare_phenotypes);

            for (int a = 0; a < opt.elitism; a++) {
                children->individuals[opt.population_size - opt.elitism + a] = parents->individuals[opt.population_size - opt.elitism + a];

                parents->individuals[opt.population_size - opt.elitism + a] = NULL;
            }
        }

        if (!opt.no_log) {
            for (int a = 0; a < opt.population_size; a++) {
                fprintf(f, "%d", children->individuals[a]->fitness);

                if (a == opt.population_size - 1) {
                    fprintf(f, "\n");
                } else {
                    fprintf(f, "\t");
                }
            }

            fflush(f);
        }

        struct phenotype generational_winner = {.fitness = -1};

        for (int a = 0; a < opt.population_size; a++) {
            if (children->individuals[a]->fitness > generational_winner.fitness) {
                generational_winner = *children->individuals[a];
            }
        }

        if (generational_winner.fitness > winner->fitness) {
            free_phenotype(winner);
            winner = copy_phenotype(&generational_winner, &opt);
        }

        PRINT_V("The best phenotype has a fitness value of %d.\n", generational_winner.fitness);

        if (opt.verbose) {
            write_phenotype(stdout, &generational_winner, &opt);
        }
    }

    if (!opt.no_log) {
        fclose(f);

        f = fopen(run_log_solution_file, "a");
        write_phenotype(f, winner, &opt);
        fclose(f);
    }

    if (opt.verbose) {
        end = clock();

        double seconds = (double)(end - begin) / CLOCKS_PER_SEC;

        printf("Total cleared lines was %'llu.\n", total_cleared_lines);
        printf("Execution finished after %.2f seconds.\n", seconds);
        printf("This amounts to %.2f cleared lines per second.\n", total_cleared_lines / seconds);
    }
}
