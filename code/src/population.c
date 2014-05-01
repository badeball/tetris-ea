#include <stdlib.h>
#include <string.h>

#include "genotype.h"
#include "phenotype.h"
#include "population.h"
#include "random.h"

struct population* initialize_population_pool (int size) {
    struct population* p = malloc(sizeof(struct population));

    p->size = size;
    p->individuals = malloc(sizeof(struct phenotype*) * size);

    return p;
}

void free_population_pool (struct population* p) {
    free(p->individuals);
    free(p);
}

void swap_populations (struct population** population_1, struct population** population_2) {
    struct population* temp_pool = *population_1;

    (*population_1) = *population_2;
    (*population_2) = temp_pool;
}

struct population* sample_population (struct population* p, int n, struct options* opt) {
    if (p->size < n) {
        n = p->size;
    }

    struct phenotype* population_copy[p->size];
    struct population* sample = initialize_population_pool(n);

    memcpy(population_copy, p->individuals, sizeof(struct phenotype*) * p->size);

    int r;

    for (int i = 0; i < n; i++) {
        r = i + l_rand(0, p->size - i, opt);

        sample->individuals[i] = population_copy[r];
        population_copy[r] = population_copy[i];
    }

    return sample;
}

struct phenotype* select_best_individual (struct population* p) {
    int max_fitness = -1;
    struct phenotype* max_individual;

    for (int i = 0; i < p->size; i++) {
        if (p->individuals[i]->fitness > max_fitness) {
            max_fitness = p->individuals[i]->fitness;
            max_individual = p->individuals[i];
        }
    }

    return max_individual;
}

struct phenotype* mate_individuals (struct phenotype* phenotype_1, struct phenotype* phenotype_2, struct options* opt) {
    struct phenotype* offspring;

    if (f_rand(opt) < opt->crossover_rate) {
        offspring = initialize_phenotype(crossover_genotypes(phenotype_1->genotype, phenotype_2->genotype, opt));
    } else {
        offspring = copy_phenotype(b_rand(opt) ? phenotype_1 : phenotype_2, opt);
    }

    if (mutate_genotype(offspring->genotype, opt)) {
        offspring->has_fitness = 0;
    }

    return offspring;
}