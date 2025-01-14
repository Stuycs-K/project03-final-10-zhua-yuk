#include "config.h"
#include "shared_memory.h"
#include "subprocess.h"
#include <stdlib.h>
#include <stdio.h>
#include "fdmcalc.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

static grid_dimen DIMENSIONS;
static int START, NEND, ORDER;

int main() {
    // printf("shared memory set up: %d\n",shared_mem_setup(size));
    // printf("semaphore set up: %d\n", semaphore_setup(0));
    DIMENSIONS = read_fdata("test.csv", "out.csv");
    spawn_subprocess(0, DIMENSIONS.size.i, 0);
    sleep(1);
    printf("%d\n",write_data("out.csv", DIMENSIONS.size, 1) );
}
