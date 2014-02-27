#ifndef PIECE_H
#define PIECE_H

#include "structs.h"

#define N_TETROMINO(tetromino, n) \
    if (n == 0) {                 \
        *tetromino = 0;           \
    } else if(n == 1) {           \
        *tetromino = 1;           \
    } else if(n == 2) {           \
        *tetromino = 3;           \
    } else if(n == 3) {           \
        *tetromino = 5;           \
    } else if(n == 4) {           \
        *tetromino = 7;           \
    } else if(n == 5) {           \
        *tetromino = 11;          \
    } else if(n == 6) {           \
        *tetromino = 15;          \
    }

#define N_ROTATIONS(rotation, n) \
    if (n == 0) {                \
        *rotation = 1;           \
    } else if(n == 1) {          \
        *rotation = 2;           \
    } else if(n == 3) {          \
        *rotation = 2;           \
    } else if(n == 5) {          \
        *rotation = 2;           \
    } else if(n == 7) {          \
        *rotation = 4;           \
    } else if(n == 11) {         \
        *rotation = 4;           \
    } else if(n == 15) {         \
        *rotation = 4;           \
    }

int place_tetromino (struct board * board, struct tetromino * tetromino, int position, int * placement);
int random_tetromino (struct options* opt);

#endif