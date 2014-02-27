#ifndef GENOTYPE_H
#define GENOTYPE_H

#include "structs.h"

void free_genotype (struct genotype* g);
struct genotype* initialize_genotype (struct options* opt);
struct genotype* copy_genotype (struct genotype* genotype, struct options* opt);
void randomize_genotype (struct genotype* g, struct options* opt);
struct genotype* crossover_genotypes (struct genotype* g_1, struct genotype* g_2, struct options* opt);
void mutate_genotype (struct genotype* g, struct options* opt);

#endif