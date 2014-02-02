#include <stdlib.h>

#include "tetromino.h"
#include "board.h"
#include "random.h"

struct tetromino tetrominos[19] = {
    {
        .tiles = {{0, 0, 0, 0},
                  {0, 1, 1, 0},
                  {0, 1, 1, 0},
                  {0, 0, 0, 0}},

        .p_top = 1,
        .p_left = 1,
        .p_right = 1,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 0, 0},
                  {1, 1, 1, 1},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0}},

        .p_top = 1,
        .p_left = 0,
        .p_right = 0,
        .p_bottom = 2,
    }, {
        .tiles = {{0, 0, 1, 0},
                  {0, 0, 1, 0},
                  {0, 0, 1, 0},
                  {0, 0, 1, 0}},

        .p_top = 0,
        .p_left = 2,
        .p_right = 1,
        .p_bottom = 0,
    }, {
        .tiles = {{0, 0, 0, 0},
                  {0, 0, 1, 1},
                  {0, 1, 1, 0},
                  {0, 0, 0, 0}},

        .p_top = 1,
        .p_left = 1,
        .p_right = 0,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 1, 0},
                  {0, 0, 1, 1},
                  {0, 0, 0, 1},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 2,
        .p_right = 0,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 0, 0},
                  {0, 1, 1, 0},
                  {0, 0, 1, 1},
                  {0, 0, 0, 0}},

        .p_top = 1,
        .p_left = 1,
        .p_right = 0,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 0, 1},
                  {0, 0, 1, 1},
                  {0, 0, 1, 0},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 2,
        .p_right = 0,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 0, 0},
                  {0, 1, 1, 1},
                  {0, 1, 0, 0},
                  {0, 0, 0, 0}},

        .p_top = 1,
        .p_left = 1,
        .p_right = 0,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 1, 0},
                  {0, 0, 1, 0},
                  {0, 0, 1, 1},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 2,
        .p_right = 0,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 0, 1},
                  {0, 1, 1, 1},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 1,
        .p_right = 0,
        .p_bottom = 2,
    }, {
        .tiles = {{0, 1, 1, 0},
                  {0, 0, 1, 0},
                  {0, 0, 1, 0},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 1,
        .p_right = 1,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 0, 0},
                  {0, 1, 1, 1},
                  {0, 0, 0, 1},
                  {0, 0, 0, 0}},

        .p_top = 1,
        .p_left = 1,
        .p_right = 0,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 1, 1},
                  {0, 0, 1, 0},
                  {0, 0, 1, 0},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 2,
        .p_right = 0,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 1, 0, 0},
                  {0, 1, 1, 1},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 1,
        .p_right = 0,
        .p_bottom = 2,
    }, {
        .tiles = {{0, 0, 1, 0},
                  {0, 0, 1, 0},
                  {0, 1, 1, 0},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 1,
        .p_right = 1,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 0, 0},
                  {0, 1, 1, 1},
                  {0, 0, 1, 0},
                  {0, 0, 0, 0}},

        .p_top = 1,
        .p_left = 1,
        .p_right = 0,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 1, 0},
                  {0, 0, 1, 1},
                  {0, 0, 1, 0},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 2,
        .p_right = 0,
        .p_bottom = 1,
    }, {
        .tiles = {{0, 0, 1, 0},
                  {0, 1, 1, 1},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 1,
        .p_right = 0,
        .p_bottom = 2,
    }, {
        .tiles = {{0, 0, 1, 0},
                  {0, 1, 1, 0},
                  {0, 0, 1, 0},
                  {0, 0, 0, 0}},

        .p_top = 0,
        .p_left = 1,
        .p_right = 1,
        .p_bottom = 1,
    },
};

int place_tetromino (struct board * board, struct tetromino * tetromino, int position, int * placement) {
    int i;

    for (i = -tetromino->p_top; i < board->height + tetromino->p_bottom - 4 + 1; i++) {
        //printf("Checking for i %d..\n", i);
        for (int y = tetromino->p_top; y < 4 - tetromino->p_bottom; y++) {
            for (int x = tetromino->p_left; x < 4 - tetromino->p_right; x++) {
                if (tetromino->tiles[y][x] == 1) {
                    //printf("Checking %d-%d (%d-%d: %d)..\n", x, y, position + x, i + y, *address_tile(position + x, i + y, board));
                    if (*address_tile(position + x, i + y, board) == 1) {
                        i--;
                        goto found_i;
                    }
                }
            }
        }
    }

    i--;

    found_i:

    //printf("Found i to be %d..\n", i);

    if (i < -tetromino->p_top) {
        return 1;
    }

    *placement = i;

    for (int y = tetromino->p_top; y < 4 - tetromino->p_bottom; y++) {
        for (int x = tetromino->p_left; x < 4 - tetromino->p_right; x++) {
            if (tetromino->tiles[y][x] == 1) {
                *address_tile(position + x, i + y, board) = 1;
            }
        }
    }

    return 0;
}

int random_tetromino (struct options* opt) {
    int tetromino, rotations,
        random_t = l_rand(opt) % 7;

    N_TETROMINO(&tetromino, random_t);
    N_ROTATIONS(&rotations, tetromino);

    return tetromino + l_rand(opt) % rotations;
}