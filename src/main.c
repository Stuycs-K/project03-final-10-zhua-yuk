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
union semun {
  int val;                  
  struct semid_ds *buf;     
  unsigned short  *array;   
  struct seminfo  *__buf;};

int main() {
    DIMENSIONS = read_fdata("test.csv", "out.csv");
    union semun semDATA;
    int semVal = semctl(semget(SEMKEY, 0, 0), 0, GETVAL, semDATA);

    spawn_subprocess(0, DIMENSIONS.size.i, 0);
    sleep(1);
    printf("%d\n",write_data("out.csv", DIMENSIONS.size, 1) );
}
