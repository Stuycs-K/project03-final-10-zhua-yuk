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
#include <errno.h>
#include <string.h>

grid_dimen DIMENSIONS;
int START, NEND, ORDER;

int main() {
    grid_dimen DIMENSIONS = read_fdata("test.csv", "out.csv");
    int num_SP = ceil(DIMENSIONS.size.k/LAYERS_PER_SP); // number subprocesses
    int num_timesteps = (DIMENSIONS.tf)/DIMENSIONS.dt; // number of timesteps
    int * subprocessPIDs = malloc(sizeof(int)*num_SP); // array holding subprocess pids
    printf("num SP: %d, ts: %d, layers each: %d\n",num_SP, num_timesteps, LAYERS_PER_SP);
    int order = 0;
    int parentPID = getpid();
    semaphore_setup(num_SP);
    // while(semctl(semget(SEMKEY, 0, 0), 0, GETVAL)!=num_SP);
    printf("parentPID: %d\n", parentPID);
    printf("semaphore value: %d\n",semctl(semget(SEMKEY, 0, 0), 0, GETVAL));
    for(int layers_done = 0; layers_done <DIMENSIONS.size.k; layers_done+=LAYERS_PER_SP){ 
      // add subprocesses' PIDs to subprocessPIDs array, spawn processes, run 1 timestep calculation
      // int subPID = 0; 
      int subPID;
      if(layers_done+LAYERS_PER_SP>DIMENSIONS.size.k){
        // printf("%d to %d, order %d\n", layers_done, DIMENSIONS.size.k, order%2);
        subPID = spawn_subprocess(layers_done,DIMENSIONS.size.k,order%2);
      }
      else{
        if(getpid()==parentPID){
        // printf("%d to %d, order %d\n", layers_done, layers_done+LAYERS_PER_SP, order%2);
        subPID = spawn_subprocess(layers_done,layers_done+LAYERS_PER_SP,order%2);}
        subprocessPIDs[order] = subPID;
      }
      order++;
    }
    if(getpid()==parentPID){
      for(int ns = 0; ns <num_SP; ns++){
        printf("[%d] %d\n", ns, subprocessPIDs[ns]);
      }
      while(semctl(semget(SEMKEY, 0, 0), 0, GETVAL)!=num_SP);
      int timesteps_done = 0;
      printf("here2\n");
      while(timesteps_done<num_timesteps){ // while more timesteps to do
      printf("doing a time step! \n");
        for(int i = 0; i<num_SP; i++){ // signal sent to subprocess 
          if(timesteps_done%2==0){
            printf("sending ACALCB to %d\n",subprocessPIDs[i]);
            int ret = kill(subprocessPIDs[i], ACALCB);
            if(ret ==-1){
            printf("%s\n",strerror(errno));
            }
          }
          else{
            printf("sending BCALCA to %d\n",subprocessPIDs[i]);
            kill(subprocessPIDs[i], BCALCA);
          }
        }
        while(semctl(semget(SEMKEY, 0, 0), 0, GETVAL)!=num_SP);
        timesteps_done++;
        printf("writing here!\n");
        write_data("out.csv", DIMENSIONS.size, timesteps_done);
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
