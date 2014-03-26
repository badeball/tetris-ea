#include <string.h>

#include "board.h"
#include "feature_functions.h"
#include "feature_helpers.h"
#include "structs.h"

struct feature features[N_FEATURES] = {
    {
        .name = "--f-max-height",
        .weights = 1,
        .function = &f_max_height,
    }, {
        .name = "--f-n-holes",
        .weights = 1,
        .function = &f_n_holes,
    }, {
        .name = "--f-column-heights",
        .weights = 0, // The number of weights is determined by board-width
        .function = &f_column_heights,
    }, {
        .name = "--f-column-difference",
        .weights = 0, // The number of weights is determined by board-width
        .function = &f_column_difference,
    }, {
        .name = "--f-landing-height",
        .weights = 1,
        .function = &f_landing_height,
    }, {
        .name = "--f-cell-transitions",
        .weights = 1,
        .function = &f_cell_transitions,
    }, {
        .name = "--f-deep-well-sum",
        .weights = 1,
        .function = &f_deep_well_sum,
    }, {
        .name = "--f-height-differences",
        .weights = 1,
        .function = &f_height_differences,
    }, {
        .name = "--f-mean-height",
        .weights = 1,
        .function = &f_mean_height,
    }, {
        .name = "--f-v-max-height",
        .weights = 1,
        .function = &f_v_max_height,
    }, {
        .name = "--f-v-n-holes",
        .weights = 1,
        .function = &f_v_n_holes,
    }, {
        .name = "--f-v-height-differences",
        .weights = 1,
        .function = &f_v_height_differences,
    }, {
        .name = "--f-v-mean-height",
        .weights = 1,
        .function = &f_v_mean_height,
    }, {
        .name = "--f-removed-lines",
        .weights = 1,
        .function = &f_removed_lines,
    }, {
        .name = "--f-weighted-blocks",
        .weights = 1,
        .function = &f_weighted_blocks,
    }, {
        .name = "--f-well-sum",
        .weights = 1,
        .function = &f_well_sum,
    }, {
        .name = "--f-n-blocks",
        .weights = 1,
        .function = &f_n_blocks,
    }, {
        .name = "--f-eroded-piece-cells",
        .weights = 1,
        .function = &f_eroded_piece_cells,
    }, {
        .name = "--f-row-transitions",
        .weights = 1,
        .function = &f_row_transitions,
    }, {
        .name = "--f-column-transitions",
        .weights = 1,
        .function = &f_column_transitions,
    }, {
        .name = "--f-cumulative-wells-dell",
        .weights = 1,
        .function = &f_cumulative_wells_dell,
    }, {
        .name = "--f-cumulative-wells-fast",
        .weights = 1,
        .function = &f_cumulative_wells_fast,
    }, {
        .name = "--f-min-height",
        .weights = 1,
        .function = &f_min_height,
    }, {
        .name = "--f-max-minus-mean-height",
        .weights = 1,
        .function = &f_max_minus_mean_height,
    }, {
        .name = "--f-mean-minux-min-height",
        .weights = 1,
        .function = &f_mean_minus_min_height,
    }, {
        .name = "--f-mean-hole-depth",
        .weights = 1,
        .function = &f_mean_hole_depth,
    }, {
        .name = "--f-max-height-difference",
        .weights = 1,
        .function = &f_max_height_difference,
    }, {
        .name = "--f-n-adjacent-holes",
        .weights = 1,
        .function = &f_n_adjacent_holes,
    }, {
        .name = "--f-max-well-depth",
        .weights = 1,
        .function = &f_max_well_depth,
    }, {
        .name = "--f-hole-depths",
        .weights = 1,
        .function = &f_hole_depths,
    }, {
        .name = "--f-n-rows-with-holes",
        .weights = 1,
        .function = &f_n_rows_with_holes,
    }, {
        .name = "--f-diversity",
        .weights = 1,
        .function = &f_diversity,
    }
};

int * column_heigths;
int features_cached[N_FEATURES];
float cached_feature_values[N_FEATURES];

void reset_feature_caches (struct options * opt) {
    for (int i = 0; i < opt->board_width; i++) {
        column_heigths[i] = -1;
    }

    for (int i = 0; i < N_FEATURES; i++) {
        features_cached[i] = 0;
    }
}

void initialize_feature_helpers (struct options * opt) {
    column_heigths = malloc(sizeof(int) * opt->board_width);

    features[feature_index("--f-column-heights")].weights = opt->board_width;
    features[feature_index("--f-column-difference")].weights = opt->board_width - 1;

    reset_feature_caches(opt);
}

int feature_exists (char * name) {
    for (int i = 0; i < N_FEATURES; i++) {
        if (strcmp(features[i].name, name) == 0) {
            return 1;
        }
    }

    return 0;
}

int feature_index (char * name) {
    for (int i = 0; i < N_FEATURES; i++) {
        if (strcmp(features[i].name, name) == 0) {
            return i;
        }
    }

    exit(1);
}

float call_feature (int feature_i, struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    if (features_cached[feature_i] == 0 || features[feature_i].weights > 1) {
        cached_feature_values[feature_i] = (features[feature_i].function) (new_board, old_board, tlp);
        features_cached[feature_i] = 1;
    }

    return cached_feature_values[feature_i];
}

int column_height (struct board * board, int column) {
    if (column_heigths[column] == -1) {
        column_heigths[column] = 0;

        for (int y = 0; y < board->height; y++) {
            if (*address_tile(column, y, board) == 1) {
                column_heigths[column] = board->height - y;
                break;
            }
        }
    }

    return column_heigths[column];
}