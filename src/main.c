#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "config.h"
#include "shared_memory.h"
#include "subprocess.h"
#include "fdmcalc.h"


grid_dimen DIMENSIONS;
int START, NEND, ORDER;

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Not enough arguments!");
        return 1;
    }
    else if (argc < 3) {
        char* buff = (char*)malloc(sizeof(char)*strlen(argv[1]) + 256);
        strcpy(buff, "out_");
        strcat(buff, argv[1]);
        DIMENSIONS = read_fdata(argv[1], buff);
        free(buff);
    }
    else {
        DIMENSIONS = read_fdata(argv[1], argv[2]);      
    }
    int n_timesteps = ceil(DIMENSIONS.tf/DIMENSIONS.dt);
    //Spawn subprocesses
    int n_subprocesses = 0;
    for (int i=0; i<n_subprocesses; i++) {
        
    }
    for (int i=0; i<n_timesteps; i++) {
    }

}
