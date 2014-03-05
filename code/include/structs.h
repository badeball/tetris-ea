#ifndef STRUCTS_H
#define STRUCTS_H

#define N_FEATURES 18

struct genotype {
    float * feature_weights;
    int * feature_enabled;
};

struct phenotype {
    int fitness;
    struct genotype* genotype;
};

struct population {
    int size;
    struct phenotype** individuals;
};

struct board {
    int width;
    int height;
    int * tiles;
};

struct tetromino {
    int p_top;
    int p_left;
    int p_right;
    int p_bottom;

    int tiles[4][4];
};

struct tetromino tetrominos[19];

struct t_last_placement {
    struct tetromino * tetromino;
    int x;
    int y;
    int n_lines_removed;
    int * lines_removed;
};

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

#endif