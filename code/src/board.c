#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "feature_functions.h"

uint16_t cell_masks[BOARD_WIDTH] = {
    0x4000, // 0b0100000000000000
    0x2000, // 0b0010000000000000
    0x1000, // 0b0001000000000000
    0x0800, // 0b0000100000000000
    0x0400, // 0b0000010000000000
    0x0200, // 0b0000001000000000
    0x0100, // 0b0000000100000000
    0x0080, // 0b0000000010000000
    0x0040, // 0b0000000001000000
    0x0020, // 0b0000000000100000
};

struct board initialize_board () {
    struct board board;

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        board.lines[i] = EMPTY_LINE;
    }

    return board;
}

struct board copy_board (struct board * board) {
    struct board copy;

    memcpy(copy.lines, board->lines, BOARD_HEIGHT * sizeof(uint16_t));

    return copy;
}

int remove_lines (struct board * board, struct t_last_placement * tlp) {
    int n_lines_removed = 0;

    if (tlp != NULL) {
        tlp->n_lines_removed = 0;
        tlp->lines_removed = malloc(sizeof(int) * BOARD_HEIGHT);
    }

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        if (board->lines[y] == FULL_LINE) {
            if (tlp != NULL) {
                tlp->lines_removed[tlp->n_lines_removed++] = y;
            }

            n_lines_removed++;

            remove_line(board, y);
        }
    }

    return n_lines_removed;
}

void remove_line (struct board * board, int line) {
    for (int y = line; y > 0; y--) {
        board->lines[y] = board->lines[y - 1];
    }

    board->lines[0] = EMPTY_LINE;
}

int get_tile (int x, int y, struct board * board) {
    return board->lines[y] & cell_masks[x];
}

int set_tile (int x, int y, struct board * board, int value) {
    if (value) {
        board->lines[y] |= cell_masks[x];
    } else {
        board->lines[y] &= ~cell_masks[x];
    }
}

void print_board (FILE * stream, struct board * board) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        fprintf(stream, "|");

        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board->lines[y] & cell_masks[x]) {
                fprintf(stream, "#");
            } else {
                fprintf(stream, " ");
            }
        }

        fprintf(stream, "|\n");
    }

    for (int i = 0; i < BOARD_WIDTH  + 2; i++) {
        fprintf(stream, "-");
    }

    fprintf(stream, "\n");
}

void free_board (struct board * board) {
    // NOOP.
}

void read_board (struct board * board) {
    size_t len;
    char * line = NULL;

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        getline(&line, &len, stdin);

        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (line[x + 1] == '#') {
                set_tile(x, y, board, 1);
            } else {
                set_tile(x, y, board, 0);
            }
        }
    }
}