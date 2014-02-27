#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

#include "board.h"
#include "feature.h"
#include "genotype.h"
#include "options.h"
#include "phenotype.h"
#include "random.h"
#include "tetromino.h"

struct phenotype* copy_phenotype (struct phenotype* phenotype, struct options* opt) {
    struct phenotype* copy = malloc(sizeof(struct phenotype));

    *copy = *phenotype;
    copy->genotype = copy_genotype(phenotype->genotype, opt);

    return copy;
}

struct phenotype* initialize_phenotype (struct genotype* g) {
    struct phenotype* p = malloc(sizeof(struct phenotype));

    p->fitness = 0;
    p->genotype = g;

    return p;
}

void free_phenotype (struct phenotype * phenotype) {
    free_genotype(phenotype->genotype);
    free(phenotype);
}

int compare_phenotypes (const void * p_1, const void * p_2) {
    struct phenotype* phenotype_1 = * (struct phenotype**) p_1;
    struct phenotype* phenotype_2 = * (struct phenotype**) p_2;

    return (phenotype_1->fitness > phenotype_2->fitness) - (phenotype_1->fitness < phenotype_2->fitness);
}

void write_phenotype (FILE * stream, struct phenotype * phenotype, struct options * opt) {
    int max_feature_length = 0;

    for (int i = 0; i < opt->n_features_enabled; i++) {
        if (strlen(feature_name(opt->enabled_f_indices[i])) > max_feature_length) {
            max_feature_length = strlen(feature_name(opt->enabled_f_indices[i]));
        }
    }

    for (int i = 0; i < opt->n_features_enabled; i++) {
        fprintf(stream, "%-*s % .2f\n",
            max_feature_length,
            feature_name(opt->enabled_f_indices[i]),
            phenotype->genotype->feature_weights[i]);
    }
}

float board_score (struct board * board, struct phenotype * phenotype, struct t_placement * last_t_placement, struct options * opt) {
    float score = 0;

    for (int i = 0; i < opt->n_features_enabled; i++) {
        score += phenotype->genotype->feature_weights[i] * (* feature_function(opt->enabled_f_indices[i])) (board, last_t_placement);
    }

    return score;
}

int phenotype_fitness (struct phenotype * phenotype, struct options* opt) {
    int fitness = 0;

    struct board * board = initialize_board(opt->board_width, opt->board_height);

    int next_tetrominos[opt->n_piece_lookahead + 1];

    for (int i = 0; i < opt->n_piece_lookahead + 1; i++) {
        int random_t = l_rand(opt) % 7;

        N_TETROMINO(&next_tetrominos[i], random_t);
    }

    while (1) {
        // Place the next tetromino on the board. If the placement was unsuccessful, exit the loop.
        if (continue_board(board, phenotype, next_tetrominos, opt) == 1) {
            break;
        }

        // Remove lines and add to the current fitness value.
        fitness += remove_lines(board, NULL);

        // Fill the lookahead with a new tetromino.
        for (int i = 0; i < opt->n_piece_lookahead; i++) {
            next_tetrominos[i] = next_tetrominos[i + 1];
        }

        int random_t = l_rand(opt) % 7;

        N_TETROMINO(&next_tetrominos[opt->n_piece_lookahead], random_t);

        if (opt->print_board) {
            print_board(stdout, board);
        }
    }

    return fitness;
}

int average_phenotype_fitness (struct phenotype * phenotype, struct options* opt) {
    float sum = 0;

    for (int i = 0; i < opt->n_trials; i++) {
        sum += phenotype_fitness(phenotype, opt);
    }

    return sum / opt->n_trials;
}

struct alternative {
    int position_i;
    int rotation_i;
    float score;
};

struct future {
    int size;
    struct alternative * alternatives;
};

void expand_future(struct future * f, struct alternative * alt) {
    if (f->size == 0) {
        f->alternatives = malloc(sizeof(struct alternative));
    } else {
        f->alternatives = realloc(f->alternatives, sizeof(struct alternative) * (f->size + 1));
    }

    f->alternatives[f->size++] = *alt;
}

void _look_ahead(struct future * f, struct board * board, struct phenotype * phenotype, int n_ahead, struct alternative * alt, int next_tetrominos[], struct options* opt) {
    // See how many rotations exists of the next tetromino.
    int n_rotations;

    N_ROTATIONS(&n_rotations, next_tetrominos[n_ahead]);

    // Calculate the number of boards that need to be checked for fitness.
    int n_boards = 0;

    for (int rotation_i = 0; rotation_i < n_rotations; rotation_i++) {
        struct tetromino tetromino = tetrominos[next_tetrominos[n_ahead] + rotation_i];
        //printf("p left is %d..\n", tetromino.p_left);
        n_boards += opt->board_width - 4 + 1 + tetromino.p_left + tetromino.p_right;
    }

    // Initialize N boards in order to perform the trials.
    struct board ** boards = malloc(sizeof(struct board *) * n_boards);

    for (int i = 0; i < n_boards; i++) {
        boards[i] = copy_board(board);
    }

    // Place the tetromino in all possible ways on the boards.
    int board_i = 0;

    struct t_placement last_t_placements[n_boards];

    for (int rotation_i = 0; rotation_i < n_rotations; rotation_i++) {
        struct tetromino tetromino = tetrominos[next_tetrominos[n_ahead] + rotation_i];

        int positions = opt->board_width - 4 + 1 + tetromino.p_left + tetromino.p_right;

        for (int position_i = -tetromino.p_left; position_i < positions - tetromino.p_left; position_i++) {
            int y;

            if (place_tetromino(boards[board_i], &tetromino, position_i, &y) == 0) {
                if (n_ahead == 0) {
                    alt = & (struct alternative) {
                        .position_i = position_i,
                        .rotation_i = rotation_i,
                    };
                }

                if (n_ahead == opt->n_piece_lookahead) {
                    struct t_placement last_t_placement = {
                        .tetromino = &tetromino,
                        .x = position_i,
                        .y = y,
                    };

                    remove_lines(boards[board_i], &last_t_placement);

                    alt->score = board_score(boards[board_i], phenotype, &last_t_placement, opt);

                    free(last_t_placement.lines_removed);

                    expand_future(f, alt);
                } else {
                    remove_lines(boards[board_i], NULL);

                    _look_ahead(f, boards[board_i], phenotype, n_ahead + 1, alt, next_tetrominos, opt);
                }

                board_i++;
            }
        }
    }

    for (int i = 0; i < n_boards; i++) {
        free_board(boards[i]);
    }

    free(boards);
}

void look_ahead(struct future * f, struct board * board, struct phenotype* phenotype, int next_tetrominos[], struct options* opt) {
    _look_ahead(f, board, phenotype, 0, NULL, next_tetrominos, opt);
}

int continue_board(struct board * board, struct phenotype* phenotype, int next_tetrominos[], struct options* opt) {
    struct future f = {
        .size = 0,
    };

    look_ahead(&f, board, phenotype, next_tetrominos, opt);

    if (f.size == 0) {
        free(f.alternatives);
        return 1;
    } else {
        struct alternative max_alt = f.alternatives[0];

        for (int i = 1; i < f.size; i++) {
            if (f.alternatives[i].score > max_alt.score) {
                max_alt = f.alternatives[i];
            }
        }

        free(f.alternatives);

        place_tetromino(
            board,
            &tetrominos[next_tetrominos[0] + max_alt.rotation_i],
            max_alt.position_i, NULL);

        return 0;
    }
}