#include <unistd.h>

#include "options.h"

void set_number_of_cores (struct options* opt) {
    opt->n_cores = sysconf(_SC_NPROCESSORS_ONLN);
}