#ifndef FEATURE_HELPERS_H
#define FEATURE_HELPERS_H

#include "board.h"

void reset_feature_caches (struct options * opt);
void initialize_feature_helpers (struct options * opt);
int feature_exists (char * name);
int feature_index (char * name);
float call_feature (int feature_i, struct board * new_board, struct board * old_board, struct t_last_placement * tlp);
int column_height (struct board * board, int column);

#endif