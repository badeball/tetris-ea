#ifndef FEATURE_H
#define FEATURE_H

#include "structs.h"

int index_feature (char * name);
char * feature_name (int feature_i);
float (* feature_function(int feature_i)) (struct board * board, struct t_last_placement * tlp);

float f_max_height            (struct board * board, struct t_last_placement * tlp);
float f_n_holes               (struct board * board, struct t_last_placement * tlp);
float f_landing_height        (struct board * board, struct t_last_placement * tlp);
float f_cell_transitions      (struct board * board, struct t_last_placement * tlp);
float f_deep_well_sum         (struct board * board, struct t_last_placement * tlp);
float f_mean_height           (struct board * board, struct t_last_placement * tlp);
float f_removed_lines         (struct board * board, struct t_last_placement * tlp);
float f_weighted_blocks       (struct board * board, struct t_last_placement * tlp);
float f_well_sum              (struct board * board, struct t_last_placement * tlp);
float f_n_blocks              (struct board * board, struct t_last_placement * tlp);
float f_eroded_piece_cells    (struct board * board, struct t_last_placement * tlp);
float f_row_transitions       (struct board * board, struct t_last_placement * tlp);
float f_column_transitions    (struct board * board, struct t_last_placement * tlp);
float f_cumulative_wells_dell (struct board * board, struct t_last_placement * tlp);
float f_cumulative_wells_fast (struct board * board, struct t_last_placement * tlp);
float f_min_height            (struct board * board, struct t_last_placement * tlp);
float f_max_minus_mean_height (struct board * board, struct t_last_placement * tlp);
float f_mean_minus_min_height (struct board * board, struct t_last_placement * tlp);
float f_mean_hole_depth       (struct board * board, struct t_last_placement * tlp);
float f_max_height_difference (struct board * board, struct t_last_placement * tlp);
float f_n_adjacent_holes      (struct board * board, struct t_last_placement * tlp);
float f_max_well_depth        (struct board * board, struct t_last_placement * tlp);
float f_hole_depths           (struct board * board, struct t_last_placement * tlp);
float f_n_rows_with_holes     (struct board * board, struct t_last_placement * tlp);
float f_diversity             (struct board * board, struct t_last_placement * tlp);

#endif