#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <time.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include "shared_memory.h"
#include "config.h"
#include "subprocess.h"
#include "types.h"
#include "utils.h"

grid_dimen DIMENSIONS;
int START, NEND, ORDER;

int main() {
  //read in file, calculate grid dimensions, timesteps, and subprocesses needed
  grid_dimen DIMENSIONS = read_fdata("test.csv", "out.csv");
  int num_SP = ceil(DIMENSIONS.size.k/LAYERS_PER_SP);
  int num_timesteps = (DIMENSIONS.tf)/DIMENSIONS.dt;

  //Allocate pipe fd array, and create pipes
  int** pipes = (int**)malloc(sizeof(int*)*num_SP);
  for (int i=0; i<num_SP; i++) {
    pipes[i] = (int*)malloc(sizeof(int)*2);
    pipe(pipes[i]);
  }

  //Create semaphores
  int semDes = semaphore_setup(num_SP);

  //Spawn children
  int order = 0;
  for(int i=0; i<DIMENSIONS.size.k; i+=LAYERS_PER_SP) {
    if (i+LAYERS_PER_SP > DIMENSIONS.size.k) {
      spawn_subprocess(i, DIMENSIONS.size.k, order % 2, pipes[i][0]);
    }
    else {
      spawn_subprocess(i, i+LAYERS_PER_SP, order % 2, pipes[i][0]);
    }
    order++;
  }

  //Block until children are ready (semaphore value is equal to num_subprocesses)
  while(semctl(semDes, 0, GETVAL) != num_SP) {
    printf("waiting, sem_val: %d\n", semctl(semDes, 0, GETVAL));
  }
  printf("done waiting, sem_val: %d\n", semctl(semDes, 0, GETVAL));

  //Start iterating timesteps
  for (int i=0; i<num_timesteps; i++) {
    //Pick which command to send
    int command = (i % 2 == 0) ? ACALCB : BCALCA;

    //write command to all pipes
    for (int y=0; y<num_SP; y++) {
      write(pipes[y][1], &command, sizeof(int));
    }

    //Wait until calculations are finished
    while(semctl(semDes, 0, GETVAL) != num_SP);

    //write data to file
    write_data("out.csv", DIMENSIONS.size, i%2);
  }

  //Kill all subprocesses
  for (int i=0; i<num_SP; i++) {
    int command = QUIT;
    write(pipes[i][1], &command, sizeof(int));
  }
  
  remove_semaphores();
  remove_shared_mem();
  return 0;
}
