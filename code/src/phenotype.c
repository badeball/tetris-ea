#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

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
    for (int i = 0; i < opt->n_features_enabled; i++) {
        fprintf(stream, "%s %d\n", feature_name(opt->enabled_f_indices[i]), phenotype->genotype->feature_weights[i]);
    }
}

int board_score (struct board * board, struct phenotype * phenotype, struct t_placement * last_t_placement, struct options * opt) {
    int score = 0;

    for (int i = 0; i < opt->n_features_enabled; i++) {
        score += phenotype->genotype->feature_weights[i] * (* feature_function(opt->enabled_f_indices[i])) (board, last_t_placement);
    }

    return score;
}

int phenotype_fitness (struct phenotype* phenotype, struct options* opt) {
    int fitness = 0;

    struct board * board = initialize_board(opt->board_width, opt->board_height);

    while (1) {
        // Select a random tetromino and see how many rotations exists of it.
        int tetromino_i, n_rotations,
            random_t = l_rand(opt) % 7;

        N_TETROMINO(&tetromino_i, random_t);
        N_ROTATIONS(&n_rotations, tetromino_i);

        // Calculate the number of boards that need to be checked for fitness.
        int n_boards = 0;

        for (int rotation_i = 0; rotation_i < n_rotations; rotation_i++) {
            struct tetromino tetromino = tetrominos[tetromino_i + rotation_i];
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
            struct tetromino tetromino = tetrominos[tetromino_i + rotation_i];

            int positions = opt->board_width - 4 + 1 + tetromino.p_left + tetromino.p_right;

            for (int position_i = -tetromino.p_left; position_i < positions - tetromino.p_left; position_i++) {
                int y;

                if (place_tetromino(boards[board_i], &tetromino, position_i, &y) == 0) {
                    last_t_placements[board_i] = (struct t_placement) {
                        .tetromino = &tetromino,
                        .x = position_i,
                        .y = y,
                    };

                    board_i++;
                }
            }
        }

        // At this point, board_i will determine if any placements were successful.
        if (board_i == 0) {
            for (int i = 0; i < n_boards; i++) {
                free_board(boards[i]);
            }

            free(boards);

            break;
        }

        // See which boards yields the highest score in accordance with the phenotype.
        int max_score = INT_MIN;
        struct board * max_board;

        for (int i = 0; i < board_i; i++) {
            int score = board_score(boards[i], phenotype, &last_t_placements[i], opt);

            if (score > max_score) {
                max_score = score;
                max_board = boards[i];
            }
        }

        // Replace the board with the best board and free everything else.
        free_board(board);
        board = max_board;

        for (int i = 0; i < n_boards; i++) {
            if (board != boards[i]) {
                free_board(boards[i]);
            }
        }

        free(boards);

        // Remove lines and add to the current fitness value.
        fitness += remove_lines(board);

        if (opt->print_board) {
            print_board(stdout, board);
        }
    }

    free(board);

    return fitness;
}

int average_phenotype_fitness (struct phenotype* phenotype, struct options* opt) {
    int sum = 0;

    for (int i = 0; i < opt->n_trials; i++) {
        sum += phenotype_fitness(phenotype, opt);
    }

    return sum / opt->n_trials;
}