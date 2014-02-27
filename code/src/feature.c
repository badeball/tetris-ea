#include <string.h>

#include "board.h"
#include "feature.h"

int index_feature (char * name) {
    if (strcmp(name, "--f-max-height") == 0) {
        return 0;
    } else if (strcmp(name, "--f-n-holes") == 0) {
        return 1;
    } else if (strcmp(name, "--f-landing-height") == 0) {
        return 2;
    } else if (strcmp(name, "--f-removed-lines") == 0) {
        return 3;
    } else if (strcmp(name, "--f-weighted-blocks") == 0) {
        return 4;
    } else if (strcmp(name, "--f-well-sum") == 0) {
        return 5;
    } else if (strcmp(name, "--f-n-blocks") == 0) {
        return 6;
    } else if (strcmp(name, "--f-eroded-piece-cells") == 0) {
        return 7;
    } else if (strcmp(name, "--f-row-transitions") == 0) {
        return 8;
    } else if (strcmp(name, "--f-column-transitions") == 0) {
        return 9;
    } else if (strcmp(name, "--f-cumulative-wells-dell") == 0) {
        return 10;
    } else if (strcmp(name, "--f-cumulative-wells-fast") == 0) {
        return 11;
    } else if (strcmp(name, "--f-min-height") == 0) {
        return 12;
    } else if (strcmp(name, "--f-max-height-difference") == 0) {
        return 13;
    } else if (strcmp(name, "--f-n-adjacent-holes") == 0) {
        return 14;
    } else if (strcmp(name, "--f-max-well-depth") == 0) {
        return 15;
    } else if (strcmp(name, "--f-hole-depths") == 0) {
        return 16;
    } else if (strcmp(name, "--f-n-rows-with-holes") == 0) {
        return 17;
    } else {
        return -1;
    }
}

char * feature_name (int feature_i) {
    switch (feature_i) {
    case 0:
        return "--f-max-height";
    case 1:
        return "--f-n-holes";
    case 2:
        return "--f-landing-height";
    case 3:
        return "--f-removed-lines";
    case 4:
        return "--f-weighted-blocks";
    case 5:
        return "--f-well-sum";
    case 6:
        return "--f-n-blocks";
    case 7:
        return "--f-eroded-piece-cells";
    case 8:
        return "--f-row-transitions";
    case 9:
        return "--f-column-transitions";
    case 10:
        return "--f-cumulative-wells-dell";
    case 11:
        return "--f-cumulative-wells-fast";
    case 12:
        return "--f-min-height";
    case 13:
        return "--f-max-height-difference";
    case 14:
        return "--f-n-adjacent-holes";
    case 15:
        return "--f-max-well-depth";
    case 16:
        return "--f-hole-depths";
    case 17:
        return "--f-n-rows-with-holes";
    }
}

float (* feature_function(int feature_i)) (struct board * board, struct t_last_placement * tlp) {
    switch (feature_i) {
    case 0:
        return &f_max_height;
    case 1:
        return &f_n_holes;
    case 2:
        return &f_landing_height;
    case 3:
        return &f_removed_lines;
    case 4:
        return &f_weighted_blocks;
    case 5:
        return &f_well_sum;
    case 6:
        return &f_n_blocks;
    case 7:
        return &f_eroded_piece_cells;
    case 8:
        return &f_row_transitions;
    case 9:
        return &f_column_transitions;
    case 10:
        return &f_cumulative_wells_dell;
    case 11:
        return &f_cumulative_wells_fast;
    case 12:
        return &f_min_height;
    case 13:
        return &f_max_height_difference;
    case 14:
        return &f_n_adjacent_holes;
    case 15:
        return &f_max_well_depth;
    case 16:
        return &f_hole_depths;
    case 17:
        return &f_n_rows_with_holes;
    }
}

int max (int a, int b) {
    return a > b ? a : b;
}

int min (int a, int b) {
    return a < b ? a : b;
}

int column_height (struct board * board, int column) {
    for (int y = 0; y < board->height; y++) {
        if (*address_tile(column, y, board) == 1) {
            return board->height - y;
        }
    }

    return 0;
}

float f_max_height (struct board * board, struct t_last_placement * tlp) {
    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            if (*address_tile(x, y, board) == 1) {
                return board->height - y;
            }
        }
    }

    return 0;
}

float f_n_holes (struct board * board, struct t_last_placement * tlp) {
    int n_holes = 0;

    for (int x = 0; x < board->width; x++) {
        for (int y = 0; y < board->height; y++) {
            if (*address_tile(x, y, board) == 1) {
                for (y++; y < board->height; y++) {
                    if (*address_tile(x, y, board) == 0) {
                        n_holes++;
                    }
                }

                break;
            }
        }
    }

    return n_holes;
}

float f_landing_height (struct board * board, struct t_last_placement * tlp) {
    return board->height - tlp->y
        - 4
        + tlp->tetromino->p_bottom
        + (4 - 1 - tlp->tetromino->p_top - tlp->tetromino->p_bottom) / 2.0f;
}

float f_removed_lines (struct board * board, struct t_last_placement * tlp) {
    int lines_removed = 0;

    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            if (*address_tile(x, y, board) == 0) {
                break;
            } else if (x == board->width - 1) {
                lines_removed++;
            }
        }
    }

    return lines_removed;
}

float f_weighted_blocks (struct board * board, struct t_last_placement * tlp) {
    int weighted_blocks = 0;

    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            if (*address_tile(x, y, board) == 1) {
                weighted_blocks += board->height - y;
            }
        }
    }

    return weighted_blocks;
}

float f_well_sum (struct board * board, struct t_last_placement * tlp) {
    int well_sum = 0;

    well_sum += max(0, column_height(board, 1) - column_height(board, 0));
    well_sum += max(0, column_height(board, board->width - 2) - column_height(board, board->width - 1));

    for (int i = 1; i < board->width - 1; i++) {
        well_sum += max(
            0,
            min(
                column_height(board, i - 1),
                column_height(board, i + 1)
            ) - column_height(board, i)
        );
    }

    return well_sum;
}

float f_n_blocks (struct board * board, struct t_last_placement * tlp) {
    int n_blocks = 0;

    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            if (*address_tile(x, y, board) == 1) {
                n_blocks++;
            }
        }
    }

    return n_blocks;
}

float f_eroded_piece_cells (struct board * board, struct t_last_placement * tlp) {
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

float f_row_transitions (struct board * board, struct t_last_placement * tlp) {
    int row_transitions = 0;

    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width - 1; x++) {
            int a = *address_tile(x, y, board),
                b = *address_tile(x + 1, y, board);

            row_transitions += a ^ b;
        }

        if (*address_tile(0, y, board) == 0) {
            row_transitions++;
        }

        if (*address_tile(board->width - 1, y, board) == 0) {
            row_transitions++;
        }
    }

    return row_transitions;
}

float f_column_transitions (struct board * board, struct t_last_placement * tlp) {
    int column_transitions = 0;

    for (int x = 0; x < board->width; x++) {
        for (int y = 0; y < board->height - 1; y++) {
            int a = *address_tile(x, y, board),
                b = *address_tile(x, y + 1, board);

            column_transitions += a ^ b;
        }

        if (*address_tile(x, 0, board) == 1) {
            column_transitions++;
        }

        if (*address_tile(x, board->height - 1, board) == 0) {
            column_transitions++;
        }
    }

    return column_transitions;
}

float f_cumulative_wells_dell (struct board * board, struct t_last_placement * tlp) {
    int cumulative_well_sum = 0;

    for (int x = 0; x < board->width; x++) {
        for (int y = 0; y < board->height; y++) {
            int left, right;

            if (x == 0) {
                left = 1;
            } else {
                left = *address_tile(x - 1, y, board);
            }

            if (x == board->width - 1) {
                right = 1;
            } else {
                right = *address_tile(x + 1, y, board);
            }

            if (*address_tile(x, y, board) == 0 && left == 1 && right == 1) {
                cumulative_well_sum++;

                for (int i = y + 1; i < board->height; i++) {
                    if (*address_tile(x, i, board) == 0) {
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

float f_cumulative_wells_fast (struct board * board, struct t_last_placement * tlp) {
    int cumulative_well_sum = 0;

    int series_sum (int n) {
        if (n == 0) {
            return 0;
        } else {
            return n * (n + 1) / 2;
        }
    }

    for (int x = 0; x < board->width; x++) {
        for (int y = 0; y < board->height; y++) {
            int left, right;

            if (x == 0) {
                left = 1;
            } else {
                left = *address_tile(x - 1, y, board);
            }

            if (x == board->width - 1) {
                right = 1;
            } else {
                right = *address_tile(x + 1, y, board);
            }

            if (*address_tile(x, y, board) == 0 && left == 1 && right == 1) {
                int depth = 1;

                for (y++; y < board->height; y++) {
                    if (*address_tile(x, y, board) == 0) {
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

float f_min_height (struct board * board, struct t_last_placement * tlp) {
    int min_height = board->height;

    for (int x = 0; x < board->width; x++) {
        for (int y = 0; y < board->height; y++) {
            if (*address_tile(x, y, board) == 1) {
                if (min_height > board->height - y) {
                    min_height = board->height - y;
                }

                break;
            } else if (y == board->height - 1) {
                return 0;
            }
        }
    }

    return min_height;
}

float f_max_height_difference (struct board * board, struct t_last_placement * tlp) {
    return f_max_height(board, tlp) - f_min_height(board, tlp);
}

float f_n_adjacent_holes (struct board * board, struct t_last_placement * tlp) {
    int n_holes = 0,
        in_hole;

    for (int x = 0; x < board->width; x++) {
        in_hole = 1;

        for (int y = 0; y < board->height; y++) {
            if (*address_tile(x, y, board) == 1 && in_hole == 1) {
                in_hole = 0;
            } else if (*address_tile(x, y, board) == 0 && in_hole == 0) {
                in_hole = 1;
                n_holes++;
            }
        }
    }

    return n_holes;
}

float f_max_well_depth (struct board * board, struct t_last_placement * tlp) {
    int max_well_depth = 0;

    max_well_depth = max(max_well_depth, column_height(board, 1) - column_height(board, 0));
    max_well_depth = max(max_well_depth, column_height(board, board->width - 2) - column_height(board, board->width - 1));

    for (int i = 1; i < board->width - 1; i++) {
        max_well_depth = max(
            max_well_depth,
            min(
                column_height(board, i - 1),
                column_height(board, i + 1)
            ) - column_height(board, i)
        );
    }

    return max_well_depth;
}

float f_hole_depths (struct board * board, struct t_last_placement * tlp) {
    int holes_depths = 0;

    for (int x = 0; x < board->width; x++) {
        int depth = 1;

        for (int y = 0; y < board->height; y++) {
            if (*address_tile(x, y, board) == 1) {
                for (y++; y < board->height; y++) {
                    if (*address_tile(x, y, board) == 0) {
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

float f_n_rows_with_holes (struct board * board, struct t_last_placement * tlp) {
    int rows_with_holes[board->height],
        n_rows_with_holes = 0;

    for (int i = 0; i < board->height; i++) {
        rows_with_holes[i] = 0;
    }

    for (int x = 0; x < board->width; x++) {
        for (int y = 0; y < board->height; y++) {
            if (*address_tile(x, y, board) == 1) {
                for (y++; y < board->height; y++) {
                    if (*address_tile(x, y, board) == 0) {
                        rows_with_holes[y] = 1;
                    }
                }

                break;
            }
        }
    }

    for (int i = 0; i < board->height; i++) {
        n_rows_with_holes += rows_with_holes[i];
    }

    return n_rows_with_holes;
}