#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "population.h"
#include "random.h"
#include "selection.h"

struct phenotype** select_parent_pairs (struct population* population, struct options* opt) {
    struct phenotype** pairs = malloc(sizeof(struct phenotype*) * population->size * 2);

    if (opt->selection == TOURNAMENT) {
        for (int i = 0; i < population->size; i++) {
            pairs[i * 2 + 0] = select_by_tournament(population, opt);
            pairs[i * 2 + 1] = select_by_tournament(population, opt);
        }
    } else if (opt->selection == SUS) {
        pairs = select_by_sus(population, population->size * 2, opt);
    } else {
        pairs = select_by_sigma_scaling(population, population->size * 2, opt);
    }

    return pairs;
}

struct phenotype* select_by_tournament (struct population* population, struct options* opt) {
    struct population* tournament_group = sample_population(population, opt->tournament_group_size, opt);
    struct phenotype* selected_phenotype;

    if (l_rand(opt) > RAND_MAX * opt->tournament_group_random_selection) {
        selected_phenotype = select_best_individual(tournament_group);
    } else {
        selected_phenotype = tournament_group->individuals[0];
    }

    free_population_pool(tournament_group);

    return selected_phenotype;
}

struct phenotype** select_with_cumulation (struct population* population, int n, float cumulation[], struct options* opt) {
    struct phenotype** selection = malloc(sizeof(struct phenotype*) * n);

    float sum      = cumulation[population->size - 1];
    float distance = (float) sum / (float) n;
    float start    = (float) (l_rand(opt) * sum / RAND_MAX) / (float) n;

    for (int i = 0, a = 0; i < n; i++) {
        while (cumulation[a] < start + i * distance) {
            a++;
        }

        selection[i] = population->individuals[a];
    }

    return selection;
}

struct phenotype** select_by_sus (struct population* population, int n, struct options* opt) {
    float cumulation[population->size];

    for (int i = 0; i < population->size; i++) {
        if (i == 0) {
            cumulation[i] = population->individuals[i]->fitness;
        } else {
            cumulation[i] = cumulation[i - 1] + population->individuals[i]->fitness;
        }
    }

    return select_with_cumulation(population, n, cumulation, opt);
}

struct phenotype** select_by_sigma_scaling (struct population* population, int n, struct options* opt) {
    float average = 0, std_deviation, square_deviations = 0;

    for (int i = 0; i < population->size; i++) {
        average += population->individuals[i]->fitness;
    }

    average /= population->size;

    for (int i = 0; i < population->size; i++) {
        square_deviations = pow(population->individuals[i]->fitness - average, 2);
    }

    std_deviation = sqrt(square_deviations / population->size);

    float cumulation[population->size];

    if (std_deviation == 0) {
        for (int i = 0; i < population->size; i++) {
            cumulation[i] = 1;
        }
    } else {
        for (int i = 0; i < population->size; i++) {
            if (i == 0) {
                cumulation[i] = 1 + (population->individuals[i]->fitness - average) / (2 * std_deviation);
            } else {
                cumulation[i] = cumulation[i - 1] + 1 + (population->individuals[i]->fitness - average) / (2 * std_deviation);
            }
        }
    }

    return select_with_cumulation(population, n, cumulation, opt);
}