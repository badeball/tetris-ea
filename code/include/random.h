#ifndef RANDOM_H
#define RANDOM_H

#include "structs.h"

int l_rand (int a, int b, struct options * opt);
int b_rand (struct options * opt);
float f_rand (struct options * opt);
void initalize_rng (struct options * opt);

#endif