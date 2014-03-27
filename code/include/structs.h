#ifndef STRUCTS_H
#define STRUCTS_H

#include <gsl/gsl_randist.h>
#include <stdint.h>

#define N_FEATURES 32

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

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
    uint16_t lines[BOARD_HEIGHT];
};

struct tetromino {
    int p_top;
    int p_left;
    int p_right;
    int p_bottom;

    uint16_t lines[4];
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

struct feature {
    char * name;
    int weights;
    float (* function) (struct board *, struct board *, struct t_last_placement *);
};

struct feature features[N_FEATURES];

struct options {
    int feature_enabled[N_FEATURES];
    int enabled_f_indices[N_FEATURES];
    int n_features_enabled;
    int n_weights_enabled;

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
    int print_board;
    int n_piece_lookahead;
    int randomization_range;
    int mutation_range;

    float feature_enable_rate;
    float mutation_rate;
    float crossover_rate;
    float tournament_group_random_selection;

    enum selection selection;

    char* log_directory;

    gsl_rng * rng;
};

#endif