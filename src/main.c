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
#include <math.h>
#include <signal.h>

grid_dimen DIMENSIONS;
int START, NEND, ORDER;

int main() {
    DIMENSIONS = read_fdata("test.csv", "out.csv");
    union semun semDATA;
    int num_SP = ceil(DIMENSIONS.size.k/LAYERS_PER_SP); // number subprocesses
    int num_timesteps = (DIMENSIONS.tf)/DIMENSIONS.dt; // number of timesteps
    int * subprocessPIDs = malloc(sizeof(int)*num_SP); // array holding subprocess pids
    printf("num SP: %d, ts: %d, layers each: %d\n",num_SP, num_timesteps, LAYERS_PER_SP);
    int order = 0;
    int parentPID = getpid();
    int x = semaphore_setup(num_SP);
    // while(semctl(semget(SEMKEY, 0, 0), 0, GETVAL, semDATA)!=num_SP);
    printf("%d\n", x);
    semctl(x, 0, GETVAL, semDATA);
    printf("parentPID: %d\n", parentPID);
    printf("semaphore value: %d\n", semDATA.val);
    for(int layers_done = 0; layers_done <DIMENSIONS.size.k; layers_done+=LAYERS_PER_SP){ 
      // add subprocesses' PIDs to subprocessPIDs array, spawn processes, run 1 timestep calculation
      int subPID = 0; 
      if(layers_done+LAYERS_PER_SP>DIMENSIONS.size.k){
        subPID = spawn_subprocess(layers_done,DIMENSIONS.size.k,order%2);
      }
      else{
        subPID = spawn_subprocess(layers_done,layers_done+LAYERS_PER_SP,order%2);
      }
      printf("mysubPID: %d\n", subPID);
      if(subPID>0){
        subprocessPIDs[order] = subPID;
      }
      order++;
    }
    if(getpid()==parentPID){
      printf("kldjlkfaj\n");
      while(semctl(semget(SEMKEY, 0, 0), 0, GETVAL, semDATA)!=num_SP);
      int timesteps_done = 1;
      printf("%d\n",write_data("out.csv", DIMENSIONS.size, 1));
      printf("here2\n");
      while(timesteps_done<num_timesteps){ // while more timesteps to do
        for(int i = 0; i<num_SP; i++){ // signal sent to subprocess 
          if(timesteps_done%2==0){
            kill(subprocessPIDs[i], ACALCB);
          }
          else{
            kill(subprocessPIDs[i], BCALCA);
          }
        }
        while(semctl(semget(SEMKEY, 0, 0), 0, GETVAL, semDATA)!=num_SP);
        timesteps_done++;
        write_data("out.csv", DIMENSIONS.size, timesteps_done%2);
      }
      printf("here3\n");
      for(int i = 0; i<num_SP; i++){ // exit all  
          kill(subprocessPIDs[i], QUIT);
        }
      remove_shared_mem();
      remove_semaphores();
      exit(0);
    }
    // sleep(1);
    // printf("myPID: %d\n", getpid());
}
