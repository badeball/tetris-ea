#ifndef POPULATION_H
#define POPULATION_H

#include "structs.h"

struct population* initialize_population_pool (int size);
void free_population_pool (struct population* p);
void swap_populations (struct population** population_1, struct population** population_2);
struct population* sample_population (struct population* p, int n, struct options* opt);
struct phenotype* select_best_individual (struct population* p);
struct phenotype* mate_individuals (struct phenotype* phenotype_1, struct phenotype* phenotype_2, struct options* opt);

#endif