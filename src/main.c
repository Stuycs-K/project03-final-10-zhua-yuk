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
  int r_file = open("/dev/random", O_RDONLY, 0);
    grid_dimen DIMENSIONS = read_fdata("test.csv", "out.csv");
    int num_SP = ceil(DIMENSIONS.size.k/LAYERS_PER_SP); // number subprocesses
    int num_timesteps = (DIMENSIONS.tf)/DIMENSIONS.dt; // number of timesteps
    int * PipeNames = malloc(sizeof(int)*num_SP); // array holding pipe names
    int * writePipes = malloc(sizeof(int)*num_SP);
    int * readPipes = malloc(sizeof(int)*num_SP);
    printf("num SP: %d, ts: %d, layers each: %d\n",num_SP, num_timesteps, LAYERS_PER_SP);
    int order = 0;
    int parentPID = getpid();
    semaphore_setup(num_SP);
    // while(semctl(semget(SEMKEY, 0, 0), 0, GETVAL)!=num_SP);
    printf("parentPID: %d\n", parentPID);
    printf("semaphore value: %d\n",semctl(semget(SEMKEY, 0, 0), 0, GETVAL));
    for(int layers_done = 0; layers_done <DIMENSIONS.size.k; layers_done+=LAYERS_PER_SP){ 
      // create subprocess pipes with random int names and add to arrays with names/fds
      read(r_file, PipeNames+order, sizeof(int));
      char * subPIPE = malloc(sizeof(char)*256);
      sprintf(subPIPE, "%d", PipeNames[order]); 
      mkfifo(subPIPE, 0640); 
      writePipes[order]=open(subPIPE, O_WRONLY, 0);
      readPipes[order]=open(subPIPE, O_RDONLY, 0);

      // spawn subprocesses and give pipe file descriptors
      if(layers_done+LAYERS_PER_SP>DIMENSIONS.size.k){
        // printf("%d to %d, order %d\n", layers_done, DIMENSIONS.size.k, order%2);
        spawn_subprocess(layers_done,DIMENSIONS.size.k,order%2, readPipes[order]);
      }
      else{
        if(getpid()==parentPID){
        // printf("%d to %d, order %d\n", layers_done, layers_done+LAYERS_PER_SP, order%2);
        spawn_subprocess(layers_done,layers_done+LAYERS_PER_SP,order%2,readPipes[order]);
        }
      }
      order++;
    }
    if(getpid()==parentPID){
      while(semctl(semget(SEMKEY, 0, 0), 0, GETVAL)!=num_SP);
      int timesteps_done = 0;
      printf("here2\n");
      while(timesteps_done<num_timesteps){ // while more timesteps to do, send messages to subprocess pipes
        int * message = malloc(sizeof(int)); 
        if(timesteps_done%2==0){
          printf("sending ACALCB\n");
          *message = ACALCB;
        }
        else{
          printf("sending BCALCA\n");
          *message = BCALCA;
        }
        printf("doing a time step! \n");
        for(int i = 0; i<num_SP; i++){ 
          write(writePipes[i],message, sizeof(int));
        }
        // wait until calculation complete, then write
        while(semctl(semget(SEMKEY, 0, 0), 0, GETVAL)!=num_SP);
        timesteps_done++;
        printf("writing here!\n");
        write_data("out.csv", DIMENSIONS.size, timesteps_done);
      }
      printf("here3\n");
      for(int i = 0; i<num_SP; i++){ // exit all  
        int * message;
        * message = QUIT;
        write(writePipes[i],message, sizeof(int));
      }
      remove_shared_mem();
      remove_semaphores();
      exit(0);
    }

}
