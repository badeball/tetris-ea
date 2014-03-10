#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "feature_functions.h"

struct board * initialize_board (int width, int height) {
    struct board * board = malloc(sizeof(struct board));

    board->width  = width;
    board->height = height;
    board->tiles  = calloc(width * height, sizeof(int));

    return board;
}

struct board * copy_board (struct board * board) {
    struct board * copy = malloc(sizeof(struct board));

    copy->width  = board->width;
    copy->height = board->height;
    copy->tiles  = malloc(sizeof(int) * board->width * board->height);

    memcpy(copy->tiles, board->tiles, board->width * board->height * sizeof(int));

    return copy;
}

int remove_lines (struct board * board, struct t_last_placement * tlp) {
    int n_lines_removed = 0;

    if (tlp != NULL) {
        tlp->n_lines_removed = 0;
        tlp->lines_removed = malloc(sizeof(int) * board->height);
    }

    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            if (*address_tile(x, y, board) == 0) {
                break;
            } else if (x == board->width - 1) {
                if (tlp != NULL) {
                    tlp->lines_removed[tlp->n_lines_removed++] = y;
                }

                n_lines_removed++;

                remove_line(board, y);
            }
        }
    }

    return n_lines_removed;
}

void remove_line (struct board * board, int line) {
    for (int y = line; y > 0; y--) {
        for (int x = 0; x < board->width; x++) {
            *address_tile(x, y, board) = *address_tile(x, y - 1, board);
        }
    }

    for (int x = 0; x < board->width; x++) {
        *address_tile(x, 0, board) = 0;
    }
}

int * address_tile (int x, int y, struct board * board) {
    return &(board->tiles[x + y * board->width]);
}

void print_board (FILE * stream, struct board * board) {
    for (int y = 0; y < board->height; y++) {
        fprintf(stream, "|");

        for (int x = 0; x < board->width; x++) {
            if (*address_tile(x, y, board) == 0) {
                fprintf(stream, " ");
            } else {
                fprintf(stream, "#");
            }
        }

        fprintf(stream, "|\n");
    }

    for (int i = 0; i < board->width + 2; i++) {
        fprintf(stream, "-");
    }

    fprintf(stream, "\n");
}

void free_board (struct board * board) {
    free(board->tiles);
    free(board);
}

int read_board (struct board * board) {
    int width = 0;
    int height = 0;

    board->tiles = NULL;

    size_t len;
    char * line = NULL;

    while (1) {
        getline(&line, &len, stdin);

        if (line[0] == '|') {
            int local_width;

            if (line[strlen(line) - 1] == '\n') {
                local_width = strlen(line) - 3;
            } else {
                local_width = strlen(line) - 2;
            }

            if (width == 0) {
                width = local_width;
                board->width = local_width;
            } else if (width != local_width) {
                return 1;
            }

            height++;

            board->tiles = realloc(board->tiles, height * width * sizeof(int));

            for (int i = 0; i < width; i++) {
                if (line[i + 1] == '#') {
                    *address_tile(i, height - 1, board) = 1;
                } else {
                    *address_tile(i, height - 1, board) = 0;
                }
            }
        } else {
            break;
        }
    }

    if (width == 0) {
        return 1;
    }

    board->width = width;
    board->height = height;

    return 0;
}