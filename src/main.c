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

#include <math.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#include "shared_memory.h"
#include "config.h"
#include "fdmcalc.h"
#include "subprocess.h"
#include "types.h"
#include "utils.h"

grid_dimen DIMENSIONS;
int START, NEND, ORDER;

int main(int argc, char *argv[]) {
  char * input = malloc(sizeof(int)*FILE_BUFF_SIZE);
  char * output = malloc(sizeof(int)*512);
  strcpy(input, "test.csv");
  strcpy(output,"out.csv");
  if(argc>1){
    strcpy(input, argv[1]);
  }
  if(argc>2){
    strcpy(output, argv[2]);
  }
  //read in file, calculate grid dimensions, timesteps, and subprocesses needed
  DIMENSIONS = read_fdata(input,output);
  int num_SP = ceil(DIMENSIONS.size.k/LAYERS_PER_SP) + 1;
  int num_timesteps = (DIMENSIONS.tf)/DIMENSIONS.dt;

  //Allocate pipe fd array, and create pipes
  int** pipes = (int**)malloc(sizeof(int*)*num_SP);
  for (int i=0; i<num_SP; i++) {
    pipes[i] = (int*)malloc(sizeof(int)*2);
    pipe(pipes[i]);
  }

  //Create semaphores
  int semDes = semaphore_setup();

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
  while(semctl(semDes, 0, GETVAL) != num_SP);

  //Start iterating timesteps
  for (int i=0; i<num_timesteps; i++) {
    //Pick which command to send
    int command = (i % 2 == 0) ? ACALCB : BCALCA;

    //write command to all pipes
    for (int y=0; y<num_SP; y++) {
      write(pipes[y][1], &command, sizeof(int));
      //Down semaphore
      struct sembuf operation; 
      operation.sem_num = 0; 
      operation.sem_op = -1;
      semop(semget(SEMKEY, 1, 0), &operation, 1); 
    }

    //Wait until calculations are finished
    while(semctl(semDes, 0, GETVAL) != num_SP);

    //write data to file
    write_data(output, DIMENSIONS.size, i%2);
  }

  //Kill all subprocesses
  for (int i=0; i<num_SP; i++) {
    int command = QUIT;
    write(pipes[i][1], &command, sizeof(int));
    free(pipes[i]);
  }
  
  remove_semaphores();
  remove_shared_mem();
  free(pipes);
  return 0;
}