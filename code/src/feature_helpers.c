#include <string.h>

#include "board.h"
#include "feature_functions.h"
#include "structs.h"

struct feature features[N_FEATURES] = {
    {
        .name = "--f-max-height",
        .function = &f_max_height,
    }, {
        .name = "--f-n-holes",
        .function = &f_n_holes,
    }, {
        .name = "--f-landing-height",
        .function = &f_landing_height,
    }, {
        .name = "--f-cell-transitions",
        .function = &f_cell_transitions,
    }, {
        .name = "--f-deep-well-sum",
        .function = &f_deep_well_sum,
    }, {
        .name = "--f-mean-height",
        .function = &f_mean_height,
    }, {
        .name = "--f-removed-lines",
        .function = &f_removed_lines,
    }, {
        .name = "--f-weighted-blocks",
        .function = &f_weighted_blocks,
    }, {
        .name = "--f-well-sum",
        .function = &f_well_sum,
    }, {
        .name = "--f-n-blocks",
        .function = &f_n_blocks,
    }, {
        .name = "--f-eroded-piece-cells",
        .function = &f_eroded_piece_cells,
    }, {
        .name = "--f-row-transitions",
        .function = &f_row_transitions,
    }, {
        .name = "--f-column-transitions",
        .function = &f_column_transitions,
    }, {
        .name = "--f-cumulative-wells-dell",
        .function = &f_cumulative_wells_dell,
    }, {
        .name = "--f-cumulative-wells-fast",
        .function = &f_cumulative_wells_fast,
    }, {
        .name = "--f-min-height",
        .function = &f_min_height,
    }, {
        .name = "--f-max-minus-mean-height",
        .function = &f_max_minus_mean_height,
    }, {
        .name = "--f-mean-minux-min-height",
        .function = &f_mean_minus_min_height,
    }, {
        .name = "--f-mean-hole-depth",
        .function = &f_mean_hole_depth,
    }, {
        .name = "--f-max-height-difference",
        .function = &f_max_height_difference,
    }, {
        .name = "--f-n-adjacent-holes",
        .function = &f_n_adjacent_holes,
    }, {
        .name = "--f-max-well-depth",
        .function = &f_max_well_depth,
    }, {
        .name = "--f-hole-depths",
        .function = &f_hole_depths,
    }, {
        .name = "--f-n-rows-with-holes",
        .function = &f_n_rows_with_holes,
    }, {
        .name = "--f-diversity",
        .function = &f_diversity,
    }
};

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