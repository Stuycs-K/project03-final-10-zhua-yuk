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
    int num_SP = ceil(DIMENSIONS.size.z/LAYERS_PER_SP); 
    int num_timesteps = (DIMENSIONS.tf)/DIMENSIONS.dt;
    int timesteps_done = 0;
    int * subprocessPIDs = malloc(sizeof(int)*)
    int order = 0;
    int parentPID = getpid();
    for(int layers_done = 0; layers_done <DIMENSIONS.z; layers_done+=LAYERS_PER_SP){
      int subPID = 0; 
      if(layers_done+LAYERS_PER_SP>DIMENSIONS.z+1){
        subPID = spawn_subprocess(layers_done,DIMENSIONS.z+1,order%2);
      }
      else{
        subPID = spawn_subprocess(layers_done,layers_done+LAYERS_PER_SP,order%2);
      }
      if(subPID>0){
        subprocessPIDs[order] = subPID;
      }
      order++;
    }
    sleep(1);
    printf("myPID: %d\n", getpid());
    printf("%d\n",write_data("out.csv", DIMENSIONS.size, 1) );
}
