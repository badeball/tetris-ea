#ifndef OPTIONS_H
#define OPTIONS_H

#define N_FEATURES 18

enum selection {
    TOURNAMENT,
    SUS,
    SIGMA,
};

struct options {
    int feature_enabled[N_FEATURES];
    int enabled_f_indices[N_FEATURES];
    int n_features_enabled;

    int verbose;
    int population_size;
    int tournament_group_size;
    int max_n_generations;
    int crossover_points;
    int elitism;
    int multi_threading;
    int n_cores;
    int no_log;
    int no_change_duration;
    int reset_volume;
    int n_trials;
    int board_width;
    int board_height;
    int print_board;
    int n_piece_lookahead;

    float mutation_rate;
    float crossover_rate;
    float tournament_group_random_selection;

    enum selection selection;

    char* log_directory;

    unsigned int seedp;
};

void enable_feature (int feature_i, struct options * opt);

#endif