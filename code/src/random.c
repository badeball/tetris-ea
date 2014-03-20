#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "options.h"

int l_rand (int a, int b, struct options * opt) {
    return gsl_ran_flat(opt->rng, a, b);
}

int b_rand (struct options * opt) {
    return l_rand(0, 2, opt);
}

float f_rand (struct options * opt) {
    return (float) gsl_rng_uniform(opt->rng);
}

int seed () {
    int s, pid;

    s = time(NULL);
    pid = getpid();

    return abs(((s * 181) * ((pid - 83) * 359)) % 104729);
}

void initalize_rng (struct options * opt) {
    opt->rng = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(opt->rng, seed());
}