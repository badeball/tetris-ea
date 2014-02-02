#ifndef PHENOTYPE_H
#define PHENOTYPE_H

#include "genotype.h"

struct phenotype {
    int fitness;
    struct genotype* genotype;
};

struct phenotype* copy_phenotype (struct phenotype* phenotype, struct options* opt);
struct phenotype* initialize_phenotype (struct genotype* g);
void free_phenotype (struct phenotype * phenotype);
int compare_phenotypes (const void * p_1, const void * p_2);
void write_phenotype (FILE * stream, struct phenotype * phenotype, struct options * opt);
int board_score (struct board * board, struct phenotype* phenotype, struct t_placement * last_t_placement, struct options * opt);
int phenotype_fitness (struct phenotype* phenotype, struct options* opt);
int average_phenotype_fitness (struct phenotype* phenotype, struct options* opt);

#endif