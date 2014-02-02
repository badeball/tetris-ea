#include <stdlib.h>
#include "options.h"

int l_rand (struct options* opt) {
    return rand_r(&(opt->seedp));
}