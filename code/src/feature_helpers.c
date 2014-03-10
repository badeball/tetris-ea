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
        .name = "--f-mean-height",
        .weights = 1,
        .function = &f_mean_height,
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

void initialize_dynamic_weight_numbers (struct options * opt) {
    features[feature_index("--f-column-heights")].weights = opt->board_width;
    features[feature_index("--f-column-difference")].weights = opt->board_width - 1;
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
}

int column_height (struct board * board, int column) {
    for (int y = 0; y < board->height; y++) {
        if (*address_tile(column, y, board) == 1) {
            return board->height - y;
        }
    }

    return 0;
}