#include <stdlib.h>
#include <string.h>

#include "genotype.h"
#include "options.h"
#include "random.h"

void free_genotype (struct genotype* g) {
    free(g->feature_weights);
    free(g->feature_enabled);
    free(g);
}

struct genotype* initialize_genotype (struct options* opt) {
    struct genotype* g = malloc(sizeof(struct genotype));

    g->feature_weights = malloc(sizeof(float) * opt->n_weights_enabled);
    g->feature_enabled = malloc(sizeof(int) * opt->n_features_enabled);

    return g;
}

struct genotype* copy_genotype (struct genotype* genotype, struct options* opt) {
    struct genotype* copy = initialize_genotype(opt);

    memcpy(copy->feature_weights, genotype->feature_weights, sizeof(float) * opt->n_weights_enabled);
    memcpy(copy->feature_enabled, genotype->feature_enabled, sizeof(int) * opt->n_features_enabled);

    return copy;
}

void randomize_genotype (struct genotype* g, struct options* opt) {
    for (int i = 0; i < opt->n_weights_enabled; i++) {
        g->feature_weights[i] = l_rand(opt) % opt->randomization_range - opt->randomization_range / 2;
    }

    for (int i = 0; i < opt->n_features_enabled; i++) {
        g->feature_enabled[i] = l_rand(opt) < RAND_MAX * opt->feature_enable_rate ? 1 : 0;
    }
}

int intcmp (const void *aa, const void *bb) {
    const int *a = aa, *b = bb;
    return (*a < *b) ? -1 : (*a > *b);
}

struct genotype* crossover_genotypes (struct genotype* g_1, struct genotype* g_2, struct options* opt) {
    struct genotype* g = initialize_genotype(opt);

    if (opt->crossover_points == opt->n_features_enabled) {
        // Uniform crossover

        int weight_i = 0;

        for (int i = 0; i < opt->n_features_enabled; i++) {
            struct genotype * selecting_from_genotype;

            if (l_rand(opt) > RAND_MAX / 2) {
                selecting_from_genotype = g_1;
            } else {
                selecting_from_genotype = g_2;
            }

            g->feature_enabled[i] = selecting_from_genotype->feature_enabled[i];

            for (int a = 0; a < features[opt->enabled_f_indices[i]].weights; a++, weight_i++) {
                g->feature_weights[weight_i] = selecting_from_genotype->feature_weights[weight_i];
            }
        }
    } else {
        // Generate N crossover points and proceed with the crossover

        int possible_points[opt->n_features_enabled - 1];
        int selected_points[opt->crossover_points];

        for (int i = 0; i < opt->n_features_enabled - 1; i++) {
            possible_points[i] = i;
        }

        int r;

        for (int i = 0; i < opt->crossover_points; i++) {
            r = i + l_rand(opt) % (opt->n_features_enabled - 1 - i);

            selected_points[i] = possible_points[r];
            possible_points[r] = possible_points[i];
        }

        qsort(selected_points, opt->crossover_points, sizeof(int), intcmp);

        int selecting_from = l_rand(opt) % 2,
            at_point = 0,
            weight_i = 0;

        for (int i = 0; i < opt->n_features_enabled; i++) {
            struct genotype * selecting_from_genotype;

            if (selecting_from) {
                selecting_from_genotype = g_1;
            } else {
                selecting_from_genotype = g_2;
            }

            g->feature_enabled[i] = selecting_from_genotype->feature_enabled[i];

            for (int a = 0; a < features[opt->enabled_f_indices[i]].weights; a++, weight_i++) {
                g->feature_weights[weight_i] = selecting_from_genotype->feature_weights[weight_i];
            }

            if (at_point < opt->crossover_points && i == selected_points[at_point]) {
                at_point++;
                selecting_from = !selecting_from;
            }
        }
    }

    return g;
}

void mutate_genotype (struct genotype* g, struct options* opt) {
    for (int i = 0; i < opt->n_weights_enabled; i++) {
        if (l_rand(opt) > RAND_MAX * opt->mutation_rate) {
            g->feature_weights[i] += l_rand(opt) % opt->mutation_range - opt->mutation_range / 2;
        }
    }

    for (int i = 0; i < opt->n_features_enabled; i++) {
        if (l_rand(opt) > RAND_MAX * opt->mutation_rate) {
            g->feature_enabled[i] += l_rand(opt) < RAND_MAX * opt->feature_enable_rate ? 1 : 0;
        }
    }
}