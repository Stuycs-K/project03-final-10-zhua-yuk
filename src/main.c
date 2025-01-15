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

grid_dimen DIMENSIONS;
int START, NEND, ORDER;

int main() {
    grid_dimen dimensions = read_fdata("test.csv", "out.csv");
    spawn_subprocess(dimensions, 0, dimensions.size.i, 0, 0);
    DIMENSIONS = read_fdata("test.csv", "out.csv");
    spawn_subprocess(0, DIMENSIONS.size.i, 0);
    sleep(1);
    printf("%d\n",write_data("out.csv", DIMENSIONS.size, 1) );
}
