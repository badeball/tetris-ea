#ifndef FEATURE_H
#define FEATURE_H

#include "tetromino.h"
#include "board.h"

struct t_placement {
    struct tetromino * tetromino;
    int x;
    int y;
};

int index_feature (char * name);
char * feature_name (int feature_i);
int (* feature_function(int feature_i)) (struct board * board, struct t_placement * last_t_placement);

int f_max_height            (struct board * board, struct t_placement * last_t_placement);
int f_n_holes               (struct board * board, struct t_placement * last_t_placement);
int f_landing_height        (struct board * board, struct t_placement * last_t_placement);
int f_removed_lines         (struct board * board, struct t_placement * last_t_placement);
int f_weighted_blocks       (struct board * board, struct t_placement * last_t_placement);
int f_well_sum              (struct board * board, struct t_placement * last_t_placement);
int f_n_blocks              (struct board * board, struct t_placement * last_t_placement);
int f_eroded_piece_cells    (struct board * board, struct t_placement * last_t_placement);
int f_row_transitions       (struct board * board, struct t_placement * last_t_placement);
int f_column_transitions    (struct board * board, struct t_placement * last_t_placement);
int f_cumulative_wells      (struct board * board, struct t_placement * last_t_placement);
int f_min_height            (struct board * board, struct t_placement * last_t_placement);
int f_max_height_difference (struct board * board, struct t_placement * last_t_placement);
int f_n_adjacent_holes      (struct board * board, struct t_placement * last_t_placement);
int f_max_well_depth        (struct board * board, struct t_placement * last_t_placement);
int f_hole_depths           (struct board * board, struct t_placement * last_t_placement);
int f_n_rows_with_holes     (struct board * board, struct t_placement * last_t_placement);

#endif