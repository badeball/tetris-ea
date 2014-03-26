#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "board.h"
#include "feature_functions.h"
#include "feature_helpers.h"

int max (int a, int b) {
    return a > b ? a : b;
}

int min (int a, int b) {
    return a < b ? a : b;
}

float f_max_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        if (new_board->lines[y] != EMPTY_LINE) {
            return BOARD_HEIGHT - y;
        }
    }

    return 0;
}

float f_n_holes (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int n_holes = 0;

    uint16_t row_holes = 0x0000,
             previous_row = 0x0000;

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        row_holes = ~new_board->lines[y] & (previous_row | row_holes);

        n_holes += full_cells_on_line[row_holes];

        previous_row = new_board->lines[y];
    }

    return n_holes;
}

int current_column = 0;

float f_column_heights (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int height = column_height(new_board, current_column++);

    if (current_column == BOARD_WIDTH) {
        current_column = 0;
    }

    return height;
}

float f_column_difference (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int difference = abs(column_height(new_board, current_column) - column_height(new_board, current_column + 1));

    current_column++;

    if (current_column == BOARD_WIDTH - 1) {
        current_column = 0;
    }

    return difference;
}

float f_landing_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return BOARD_HEIGHT - tlp->y
        - 4
        + tlp->tetromino->p_bottom
        + (4 - 1 - tlp->tetromino->p_top - tlp->tetromino->p_bottom) / 2.0f;
}

float f_cell_transitions (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int transitions = 0;

    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (get_tile(x, y, new_board) == 0) {
                if (x > 0 && get_tile(x - 1, y, new_board))
                    transitions++;

                if (x < BOARD_WIDTH - 1 && get_tile(x + 1, y, new_board))
                    transitions++;

                if (y > 0 && get_tile(x, y - 1, new_board))
                    transitions++;

                if (y < BOARD_HEIGHT - 1 && get_tile(x, y + 1, new_board))
                    transitions++;
            }
        }
    }

    return transitions;
}

float f_deep_well_sum (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int well_sum = 0, well;

    well = column_height(new_board, 1) - column_height(new_board, 0);

    if (well > 1) well_sum += well;

    well = column_height(new_board, BOARD_WIDTH - 2) - column_height(new_board, BOARD_WIDTH - 1);

    if (well > 1) well_sum += well;

    for (int i = 1; i < BOARD_WIDTH - 1; i++) {
        well = min(
                column_height(new_board, i - 1),
                column_height(new_board, i + 1)
            ) - column_height(new_board, i);

        if (well > 1) well_sum += well;
    }

    return well_sum;
}

float f_height_differences (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int sum_differences = 0;

    for (int i = 0; i < BOARD_WIDTH - 1; i++) {
        sum_differences += f_column_difference(new_board, old_board, tlp);
    }

    return sum_differences;
}

float f_mean_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int sum = 0;

    for (int i = 0; i < BOARD_WIDTH; i++) {
        sum += column_height(new_board, i);
    }

    return (float) sum / (float) BOARD_WIDTH;
}

float feature_difference (char * name, struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int feature_i = feature_index(name);

    return call_feature(feature_i, new_board, old_board, tlp)
        - call_feature(feature_i, old_board, new_board, tlp);
}

float f_v_max_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return feature_difference("--f-max-height", new_board, old_board, tlp);
}

float f_v_n_holes (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return feature_difference("--f-n-holes", new_board, old_board, tlp);
}

float f_v_height_differences (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return feature_difference("--f-height-differences", new_board, old_board, tlp);
}

float f_v_mean_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return feature_difference("--f-mean-height", new_board, old_board, tlp);
}

float f_removed_lines (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return tlp->n_lines_removed;
}

float f_weighted_blocks (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int weighted_blocks = 0;

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        weighted_blocks += full_cells_on_line[new_board->lines[y]] * (BOARD_HEIGHT - y);
    }

    return weighted_blocks;
}

float f_well_sum (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int well_sum = 0;

    well_sum += max(0, column_height(new_board, 1) - column_height(new_board, 0));
    well_sum += max(0, column_height(new_board, BOARD_WIDTH - 2) - column_height(new_board, BOARD_WIDTH - 1));

    for (int i = 1; i < BOARD_WIDTH - 1; i++) {
        well_sum += max(
            0,
            min(
                column_height(new_board, i - 1),
                column_height(new_board, i + 1)
            ) - column_height(new_board, i)
        );
    }

    return well_sum;
}

float f_n_blocks (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int n_blocks = 0;

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        n_blocks += full_cells_on_line[new_board->lines[y]];
    }

    return n_blocks;
}

float f_eroded_piece_cells (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    if (tlp->n_lines_removed == 0) {
        return 0;
    }

    int eroded_cells = 0;

    for (int i = 0; i < tlp->n_lines_removed; i++) {
        int y = tlp->lines_removed[i];

        if (y >= tlp->y + tlp->tetromino->p_top &&
            y <= tlp->y + 3 - tlp->tetromino->p_bottom) {
            eroded_cells += full_cells_on_line[tlp->tetromino->lines[y - tlp->y]];
        }
    }

    return tlp->n_lines_removed * eroded_cells;
}

float f_row_transitions (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int row_transitions = 0;

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        uint16_t previous_bit = cell_masks[0], // 0b0100000000000000
                 row = new_board->lines[y];

        for (int x = 0; x < BOARD_WIDTH; x++) {
            uint16_t current_bit = row & cell_masks[0];

            if (previous_bit ^ current_bit) {
                row_transitions++;
            }

            row = row << 1;
            previous_bit = current_bit;
        }

        if (previous_bit ^ cell_masks[0]) {
            row_transitions++;
        }
    }

    return row_transitions;
}

float f_column_transitions (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int column_transitions = 0;

    uint16_t previous_line = EMPTY_LINE;

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        column_transitions += full_cells_on_line[previous_line ^ new_board->lines[y]];

        previous_line = new_board->lines[y];
    }

    column_transitions += full_cells_on_line[previous_line ^ FULL_LINE];

    return column_transitions;
}

float f_cumulative_wells_dell (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int cumulative_well_sum = 0;

    uint16_t well_mask = 0xE000,    // 0b1110000000000000
             well_pattern = 0xA000; // 0b1010000000000000

    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if ((new_board->lines[y] & well_mask) == well_pattern) {
                cumulative_well_sum++;

                for (int i = y + 1; i < BOARD_HEIGHT; i++) {
                    if (new_board->lines[i] & cell_masks[x]) {
                        break;
                    } else {
                        cumulative_well_sum++;
                    }
                }
            }
        }

        well_mask = well_mask >> 1;
        well_pattern = well_pattern >> 1;
    }

    return cumulative_well_sum;
}

int series_sum (int n) {
    if (n == 0) {
        return 0;
    } else {
        return n * (n + 1) / 2;
    }
}

float f_cumulative_wells_fast (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int cumulative_well_sum = 0;

    uint16_t well_mask = 0xE000,    // 0b1110000000000000
             well_pattern = 0xA000; // 0b1010000000000000

    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if ((new_board->lines[y] & well_mask) == well_pattern) {
                int well_depth = 1;

                for (y++; y < BOARD_HEIGHT; y++) {
                    if (new_board->lines[y] & cell_masks[x]) {
                        break;
                    } else {
                        well_depth++;
                    }
                }

                cumulative_well_sum += series_sum(well_depth);
            }
        }

        well_mask = well_mask >> 1;
        well_pattern = well_pattern >> 1;
    }

    return cumulative_well_sum;
}

float f_min_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int min_height = BOARD_HEIGHT,
        height;

    for (int x = 0; x < BOARD_WIDTH; x++) {
        height = column_height(new_board, x);

        if (min_height > height) {
            min_height = height;

            if (min_height == 0) {
                break;
            }
        }
    }

    return min_height;
}

float f_max_minus_mean_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return call_feature(feature_index("--f-max-height"), new_board, old_board, tlp)
        - call_feature(feature_index("--f-mean-height"), new_board, old_board, tlp);
}

float f_mean_minus_min_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return call_feature(feature_index("--f-mean-height"), new_board, old_board, tlp)
        - call_feature(feature_index("--f-min-height"), new_board, old_board, tlp);
}

float f_mean_hole_depth (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return call_feature(feature_index("--f-hole-depths"), new_board, old_board, tlp)
        / call_feature(feature_index("--f-n-holes"), new_board, old_board, tlp);
}

float f_max_height_difference (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return call_feature(feature_index("--f-max-height"), new_board, old_board, tlp)
        - call_feature(feature_index("--f-min-height"), new_board, old_board, tlp);
}

float f_n_adjacent_holes (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int n_holes = 0;

    uint16_t row_holes = 0x0000,
             previous_row = 0x0000;

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        row_holes = ~new_board->lines[y] & previous_row;

        n_holes += full_cells_on_line[row_holes];

        previous_row = new_board->lines[y];
    }

    return n_holes;
}

float f_max_well_depth (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int max_well_depth = 0;

    max_well_depth = max(max_well_depth, column_height(new_board, 1) - column_height(new_board, 0));
    max_well_depth = max(max_well_depth, column_height(new_board, BOARD_WIDTH - 2) - column_height(new_board, BOARD_WIDTH - 1));

    for (int i = 1; i < BOARD_WIDTH - 1; i++) {
        max_well_depth = max(
            max_well_depth,
            min(
                column_height(new_board, i - 1),
                column_height(new_board, i + 1)
            ) - column_height(new_board, i)
        );
    }

    return max_well_depth;
}

float f_hole_depths (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int whole_depths = 0;

    uint16_t covering_cells = 0x0000,
             previous_row = new_board->lines[BOARD_HEIGHT - 1];

    for (int y = BOARD_HEIGHT - 2; y >= 0; y--) {
        covering_cells = new_board->lines[y] & (~previous_row | covering_cells);

        whole_depths += full_cells_on_line[covering_cells];

        previous_row = new_board->lines[y];
    }

    return whole_depths;
}

float f_n_rows_with_holes (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int n_rows_with_holes = 0;

    uint16_t row_holes = 0x0000,
             previous_row = 0x0000;

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        row_holes = ~new_board->lines[y] & (previous_row | row_holes);

        if (full_cells_on_line[row_holes]) {
            n_rows_with_holes++;
        }

        previous_row = new_board->lines[y];
    }

    return n_rows_with_holes;
}

float f_diversity (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int diversities[5], diversity = 0;

    for (int i = 0; i < 5; i++) {
        diversities[i] = 0;
    }

    for (int i = 0; i < BOARD_WIDTH - 1; i++) {
        switch (column_height(new_board, i) - column_height(new_board, i + 1)) {
            case -2:
                diversities[0] = 1;
                break;
            case -1:
                diversities[1] = 1;
                break;
            case 0:
                diversities[2] = 1;
                break;
            case 1:
                diversities[3] = 1;
                break;
            case 2:
                diversities[4] = 1;
                break;
        }
    }

    for (int i = 0; i < 5; i++) {
        diversity += diversities[i];
    }

    return diversity;
}