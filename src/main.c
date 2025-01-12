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

int main() {
    vec3i size;
    size.i=2;
    size.j=2;
    size.k=2;
    printf("shared memory set up: %d\n",shared_mem_setup(size));
    printf("semaphore set up: %d\n", semaphore_setup(0));
    read_fdata("test.csv", "out.csv");
    printf("wrote data: %d\n",write_data("out.csv", size, 0) );
    // spawn_subprocess(size,1,0, 0,1, 10, 3);
    printf("shmget works: %d\n", shmget(11707, sizeof(double)*(size.i+1)*(size.j+1)*(size.k+1), 0));
    // printf("%d\n",write_data("out.csv", size, 0) );
}
