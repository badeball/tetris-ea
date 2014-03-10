#ifndef FEATURE_HELPERS_H
#define FEATURE_HELPERS_H

#include "board.h"

int feature_exists (char * name);
int feature_index (char * name);
int column_height (struct board * board, int column);

#endif