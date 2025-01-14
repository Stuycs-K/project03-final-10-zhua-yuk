#include "memory.h"
#include "fdmcalc.h"
#include "config.h"
#include "subprocess.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <config.h>

int spawn_subprocess(grid_dimen dimens, int start, int nend, int order, int mode) {
    //Fork process
    int f = fork();

    if (f == 0) {
        printf("subprocess %d\n", getpid());

        //Attach and down semaphore
        struct sembuf operation; 
        operation.sem_op = -1;
        operation.sem_num = 0; 
        semop(semget(SEMKEY, 1, 0), &operation, 1); 

        //Attach to shared memory segments
        double* atemp = shmat(shmget(ATEMPKEY, 0, 0), 0, 0);
        double* btemp = shmat(shmget(BTEMPKEY, 0, 0), 0, 0);
        double* coeffs = shmat(shmget(COEFKEY, 0, 0), 0, 0);

        if (mode) {
            update_layers(btemp, atemp, coeffs, dimens, start, nend, order);
        }
        else {
            update_layers(atemp, btemp, coeffs, dimens, start, nend, order);
        }

        //detach from shared memory
        shmdt(atemp);
        shmdt(btemp);
        shmdt(coeffs);
        
        //up semaphore
        operation.sem_op = 1;
        semop(semget(SEMKEY, 1, 0), &operation, 1); // up semaphore
        exit(1);
    }
    else{
        return f;
    }
}
