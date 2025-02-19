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
#include <signal.h>
#include <errno.h>
#include <string.h>

void calculate_once(int mode) {
        //Attach to shared memory segments
        double* atemp = shmat(shmget(ATEMPKEY, 0, 0), 0, 0);
        double* btemp = shmat(shmget(BTEMPKEY, 0, 0), 0, 0);
        double* coeffs = shmat(shmget(COEFKEY, 0, 0), 0, 0);

        if (mode) {
            update_layers(btemp, atemp, coeffs, DIMENSIONS, START, NEND, ORDER);
        }
        else {
            update_layers(atemp, btemp, coeffs, DIMENSIONS, START, NEND, ORDER);
        }

        //detach from shared memory
        shmdt(atemp);
        shmdt(btemp);
        shmdt(coeffs);
        
        //up semaphore
        struct sembuf operation; 
        operation.sem_num = 0; 
        operation.sem_op = 1;
        semop(semget(SEMKEY, 1, 0), &operation, 1); 
}

int spawn_subprocess(int start, int nend, int order, int pipe) { 
    //edit static vars
    START = start;
    NEND = nend;
    ORDER = order;

    int f = fork();

    if (f) {// return pid if parent
        return f;
    }
     
    else {
        //Up the semaphore
        struct sembuf operation; 
        int semid = semget(SEMKEY, 0, 0);
        operation.sem_op = 1;
        operation.sem_num = 0; 
        semop(semid, &operation, 1); 
        
        int command = 0;
        //infinite loop until killed
        while (1) {
            //wait for message
            if (read(pipe, &command, sizeof(int)) > 0) {
                switch (command) {
                    case ACALCB:
                        calculate_once(0);
                        break;
                    case BCALCA:
                        calculate_once(1);
                        break;
                    case QUIT:
                        exit(0);
                        break;
                    default: 
                        break;
                }
            }
        }
        exit(0);
    }
}