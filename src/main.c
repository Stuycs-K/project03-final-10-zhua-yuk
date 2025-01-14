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

int main() {
    grid_dimen dimensions = read_fdata("test.csv", "out.csv");
    spawn_subprocess(dimensions, 0, dimensions.size.i, 0, 0);
    sleep(1);

    printf("%d\n",write_data("out.csv", dimensions.size, 1) );
}