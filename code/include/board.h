#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdint.h>

#include "structs.h"

#define LINE_MASK  (uint16_t) 0x7FE0 // 0b0111111111100000
#define EMPTY_LINE (uint16_t) 0x8010 // 0b1000000000010000
#define FULL_LINE  (uint16_t) 0xFFF0 // 0b1111111111110000

uint16_t cell_masks[BOARD_WIDTH];

struct board initialize_board ();
struct board copy_board (struct board * board);
int remove_lines (struct board * board, struct t_last_placement * tlp);
void remove_line (struct board * board, int line);
int get_tile (int x, int y, struct board * board);
int set_tile (int x, int y, struct board * board, int value);
void print_board (FILE * stream, struct board * board);
void free_board (struct board * board);
void read_board (struct board * board);

#endif
