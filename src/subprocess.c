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

extern grid_dimen DIMENSIONS;
extern int START, NEND, ORDER;

void calculate_once(int mode) {
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
            update_layers(btemp, atemp, coeffs, DIMENSIONS, start, nend, order);
        }
        else {
            update_layers(atemp, btemp, coeffs, DIMENSIONS, start, nend, order);
        }

        //detach from shared memory
        shmdt(atemp);
        shmdt(btemp);
        shmdt(coeffs);
        
        //up semaphore
        operation.sem_op = 1;
        semop(semget(SEMKEY, 1, 0), &operation, 1); // up semaphore
}

static void sighandler(int signo){
    if(signo == QUIT){
        exit(0);
    }
    else if (signo == ACALCB){
        calculate_once(0);
    }
    else if (signo == BCALCA){
        calculate_once(1);
    }
}

int spawn_subprocess(int start, int nend, int order) { 
    //edit static vars
    START = start;
    NEND = nend;
    ORDER = order;

    int f = fork();

    if (f) {// return pid if parent
        return f;
    }
     
    else {
        signal(ACALCB, sighandler);
        signal(BCALCA, sighandler);
        signal(QUIT, sighandler);
    }
}
