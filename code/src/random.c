#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "options.h"

int l_rand (struct options* opt) {
    return rand_r(&(opt->seedp));
}

int seed () {
    int s, pid;

    s = time(NULL);
    pid = getpid();

    return abs(((s * 181) * ((pid - 83) * 359)) % 104729);
}