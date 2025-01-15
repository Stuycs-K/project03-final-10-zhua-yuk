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
    int num_SP = ceil(DIMENSIONS.size.k/LAYERS_PER_SP); 
    int num_timesteps = (DIMENSIONS.tf)/DIMENSIONS.dt;
    int * subprocessPIDs = malloc(sizeof(int)*num_SP);
    int order = 0;
    int parentPID = getpid();
    printf("parentPID: %d\n", parentPID);
    for(int layers_done = 0; layers_done <DIMENSIONS.size.k; layers_done+=LAYERS_PER_SP){ 
      // add subprocesses' PIDs to subprocessPIDs array, spawn processes, run 1 timestep calculation
      int subPID = 0; 
      if(layers_done+LAYERS_PER_SP>DIMENSIONS.size.k+1){
        subPID = spawn_subprocess(layers_done,DIMENSIONS.size.k+1,order%2);
      }
      else{
        subPID = spawn_subprocess(layers_done,layers_done+LAYERS_PER_SP,order%2);
      }
      if(subPID>0){
        subprocessPIDs[order] = subPID;
      }
      order++;
    }
    if(getpid()==parentPID){
      int timesteps_done = 1;
      printf("%d\n",write_data("out.csv", DIMENSIONS.size, 1));
      while(timesteps_done<num_timesteps){ // while more timesteps to do
        for(int i = 0; i<num_SP; i++){ // signal sent to subprocess 
          if(timesteps_done%2==0){
            kill(subprocessPIDs[i], ACALCB);
          }
          else{
            kill(subprocessPIDs[i], BCALCA);
          }
        }
        timesteps_done++;
        write_data("out.csv", DIMENSIONS.size, timesteps_done%2);
      }
    }
    sleep(1);
    // printf("myPID: %d\n", getpid());
}
