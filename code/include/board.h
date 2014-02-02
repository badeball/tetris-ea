#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>

struct board {
    int width;
    int height;
    int * tiles;
};

struct board * initialize_board (int width, int height);
struct board * copy_board (struct board * board);
int remove_lines (struct board * board);
void remove_line (struct board * board, int line);
int * address_tile (int x, int y, struct board * board);
void print_board (FILE * stream, struct board * board);
void free_board (struct board * board);
int read_board (struct board * board);

#endif
