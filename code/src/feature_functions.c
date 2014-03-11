#include <string.h>
#include <stdlib.h>

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
    for (int y = 0; y < new_board->height; y++) {
        for (int x = 0; x < new_board->width; x++) {
            if (*address_tile(x, y, new_board) == 1) {
                return new_board->height - y;
            }
        }
    }

    return 0;
}

float f_n_holes (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int n_holes = 0;

    for (int x = 0; x < new_board->width; x++) {
        for (int y = 0; y < new_board->height; y++) {
            if (*address_tile(x, y, new_board) == 1) {
                for (y++; y < new_board->height; y++) {
                    if (*address_tile(x, y, new_board) == 0) {
                        n_holes++;
                    }
                }

                break;
            }
        }
    }

    return n_holes;
}

int current_column = 0;

float f_column_heights (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int height = column_height(new_board, current_column++);

    if (current_column == new_board->width) {
        current_column = 0;
    }

    return height;
}

float f_column_difference (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int difference = abs(column_height(new_board, current_column) - column_height(new_board, current_column + 1));

    current_column++;

    if (current_column == new_board->width - 1) {
        current_column = 0;
    }

    return difference;
}

float f_landing_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    return new_board->height - tlp->y
        - 4
        + tlp->tetromino->p_bottom
        + (4 - 1 - tlp->tetromino->p_top - tlp->tetromino->p_bottom) / 2.0f;
}

float f_cell_transitions (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int transitions = 0;

    for (int x = 0; x < new_board->width; x++) {
        for (int y = 0; y < new_board->height; y++) {
            if (*address_tile(x, y, new_board) == 0) {
                if (x > 0 && *address_tile(x - 1, y, new_board) == 1)
                    transitions++;

                if (x < new_board->width - 1 && *address_tile(x + 1, y, new_board) == 1)
                    transitions++;

                if (y > 0 && *address_tile(x, y - 1, new_board) == 1)
                    transitions++;

                if (y < new_board->height - 1 && *address_tile(x, y + 1, new_board) == 1)
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

    well = column_height(new_board, new_board->width - 2) - column_height(new_board, new_board->width - 1);

    if (well > 1) well_sum += well;

    for (int i = 1; i < new_board->width - 1; i++) {
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

    for (int i = 0; i < new_board->width - 1; i++) {
        sum_differences += f_column_difference(new_board, old_board, tlp);
    }

    return sum_differences;
}

float f_mean_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int sum = 0;

    for (int i = 0; i < new_board->width; i++) {
        sum += column_height(new_board, i);
    }

    return (float) sum / (float) new_board->width;
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
    int lines_removed = 0;

    for (int y = 0; y < new_board->height; y++) {
        for (int x = 0; x < new_board->width; x++) {
            if (*address_tile(x, y, new_board) == 0) {
                break;
            } else if (x == new_board->width - 1) {
                lines_removed++;
            }
        }
    }

    return lines_removed;
}

float f_weighted_blocks (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int weighted_blocks = 0;

    for (int y = 0; y < new_board->height; y++) {
        for (int x = 0; x < new_board->width; x++) {
            if (*address_tile(x, y, new_board) == 1) {
                weighted_blocks += new_board->height - y;
            }
        }
    }

    return weighted_blocks;
}

float f_well_sum (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int well_sum = 0;

    well_sum += max(0, column_height(new_board, 1) - column_height(new_board, 0));
    well_sum += max(0, column_height(new_board, new_board->width - 2) - column_height(new_board, new_board->width - 1));

    for (int i = 1; i < new_board->width - 1; i++) {
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

    for (int y = 0; y < new_board->height; y++) {
        for (int x = 0; x < new_board->width; x++) {
            if (*address_tile(x, y, new_board) == 1) {
                n_blocks++;
            }
        }
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
            for (int x = tlp->tetromino->p_left; x < 4 - tlp->tetromino->p_right; x++) {
                eroded_cells += tlp->tetromino->tiles[y - tlp->y][x];
            }
        }
    }

    return tlp->n_lines_removed * eroded_cells;
}

float f_row_transitions (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int row_transitions = 0;

    for (int y = 0; y < new_board->height; y++) {
        for (int x = 0; x < new_board->width - 1; x++) {
            int a = *address_tile(x, y, new_board),
                b = *address_tile(x + 1, y, new_board);

            row_transitions += a ^ b;
        }

        if (*address_tile(0, y, new_board) == 0) {
            row_transitions++;
        }

        if (*address_tile(new_board->width - 1, y, new_board) == 0) {
            row_transitions++;
        }
    }

    return row_transitions;
}

float f_column_transitions (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int column_transitions = 0;

    for (int x = 0; x < new_board->width; x++) {
        for (int y = 0; y < new_board->height - 1; y++) {
            int a = *address_tile(x, y, new_board),
                b = *address_tile(x, y + 1, new_board);

            column_transitions += a ^ b;
        }

        if (*address_tile(x, 0, new_board) == 1) {
            column_transitions++;
        }

        if (*address_tile(x, new_board->height - 1, new_board) == 0) {
            column_transitions++;
        }
    }

    return column_transitions;
}

float f_cumulative_wells_dell (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int cumulative_well_sum = 0;

    for (int x = 0; x < new_board->width; x++) {
        for (int y = 0; y < new_board->height; y++) {
            int left, right;

            if (x == 0) {
                left = 1;
            } else {
                left = *address_tile(x - 1, y, new_board);
            }

            if (x == new_board->width - 1) {
                right = 1;
            } else {
                right = *address_tile(x + 1, y, new_board);
            }

            if (*address_tile(x, y, new_board) == 0 && left == 1 && right == 1) {
                cumulative_well_sum++;

                for (int i = y + 1; i < new_board->height; i++) {
                    if (*address_tile(x, i, new_board) == 0) {
                        cumulative_well_sum++;
                    } else {
                        break;
                    }
                }
            }
        }
    }

    return cumulative_well_sum;
}

float f_cumulative_wells_fast (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int cumulative_well_sum = 0;

    int series_sum (int n) {
        if (n == 0) {
            return 0;
        } else {
            return n * (n + 1) / 2;
        }
    }

    for (int x = 0; x < new_board->width; x++) {
        for (int y = 0; y < new_board->height; y++) {
            int left, right;

            if (x == 0) {
                left = 1;
            } else {
                left = *address_tile(x - 1, y, new_board);
            }

            if (x == new_board->width - 1) {
                right = 1;
            } else {
                right = *address_tile(x + 1, y, new_board);
            }

            if (*address_tile(x, y, new_board) == 0 && left == 1 && right == 1) {
                int depth = 1;

                for (y++; y < new_board->height; y++) {
                    if (*address_tile(x, y, new_board) == 0) {
                        depth++;
                    } else {
                        break;
                    }
                }

                cumulative_well_sum += series_sum(depth);
            }
        }
    }

    return cumulative_well_sum;
}

float f_min_height (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int min_height = new_board->height;

    for (int x = 0; x < new_board->width; x++) {
        for (int y = 0; y < new_board->height; y++) {
            if (*address_tile(x, y, new_board) == 1) {
                if (min_height > new_board->height - y) {
                    min_height = new_board->height - y;
                }

                break;
            } else if (y == new_board->height - 1) {
                return 0;
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
    int n_holes = 0,
        in_hole;

    for (int x = 0; x < new_board->width; x++) {
        in_hole = 1;

        for (int y = 0; y < new_board->height; y++) {
            if (*address_tile(x, y, new_board) == 1 && in_hole == 1) {
                in_hole = 0;
            } else if (*address_tile(x, y, new_board) == 0 && in_hole == 0) {
                in_hole = 1;
                n_holes++;
            }
        }
    }

    return n_holes;
}

float f_max_well_depth (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int max_well_depth = 0;

    max_well_depth = max(max_well_depth, column_height(new_board, 1) - column_height(new_board, 0));
    max_well_depth = max(max_well_depth, column_height(new_board, new_board->width - 2) - column_height(new_board, new_board->width - 1));

    for (int i = 1; i < new_board->width - 1; i++) {
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
    int holes_depths = 0;

    for (int x = 0; x < new_board->width; x++) {
        int depth = 1;

        for (int y = 0; y < new_board->height; y++) {
            if (*address_tile(x, y, new_board) == 1) {
                for (y++; y < new_board->height; y++) {
                    if (*address_tile(x, y, new_board) == 0) {
                        holes_depths += depth;
                    } else {
                        depth++;
                    }
                }

                break;
            }
        }
    }

    return holes_depths;
}

float f_n_rows_with_holes (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int rows_with_holes[new_board->height],
        n_rows_with_holes = 0;

    for (int i = 0; i < new_board->height; i++) {
        rows_with_holes[i] = 0;
    }

    for (int x = 0; x < new_board->width; x++) {
        for (int y = 0; y < new_board->height; y++) {
            if (*address_tile(x, y, new_board) == 1) {
                for (y++; y < new_board->height; y++) {
                    if (*address_tile(x, y, new_board) == 0) {
                        rows_with_holes[y] = 1;
                    }
                }

                break;
            }
        }
    }

    for (int i = 0; i < new_board->height; i++) {
        n_rows_with_holes += rows_with_holes[i];
    }

    return n_rows_with_holes;
}

float f_diversity (struct board * new_board, struct board * old_board, struct t_last_placement * tlp) {
    int diversities[5], diversity = 0;

    for (int i = 0; i < 5; i++) {
        diversities[i] = 0;
    }

    for (int i = 0; i < new_board->width - 1; i++) {
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