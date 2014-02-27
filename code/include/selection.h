#ifndef SELECTION_H
#define SELECTION_H

#include "structs.h"

struct phenotype** select_parent_pairs (struct population* population, struct options* opt);
struct phenotype* select_by_tournament (struct population* population, struct options* opt);
struct phenotype** select_with_cumulation (struct population* population, int n, float cumulation[], struct options* opt);
struct phenotype** select_by_sus (struct population* population, int n, struct options* opt);
struct phenotype** select_by_sigma_scaling (struct population* population, int n, struct options* opt);

#endif