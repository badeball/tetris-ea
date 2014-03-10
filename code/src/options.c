#include <stdlib.h>

#include "options.h"

void enable_feature (int feature_i, struct options * opt) {
    if (opt->feature_enabled[feature_i] == 1) {
        return;
    }

    opt->feature_enabled[feature_i] = 1;
    opt->enabled_f_indices[opt->n_features_enabled] = feature_i;
    opt->n_features_enabled++;
}