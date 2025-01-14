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

int calculate_once (vec3i size, int ub, int lb, int order, int mode, double timestep, double units) {
    int writeKey=BTEMPKEY;
    int readKey=ATEMPKEY;
    if(mode == 1){
        writeKey=ATEMPKEY;
        readKey=BTEMPKEY;
    }
    int f = fork();
    if(f==0){
        int sem_des = semget(SEMKEY, 1, 0);
        struct sembuf mysembuf; 
        mysembuf.sem_op = -1;
        mysembuf.sem_num = 0; 
        semop(sem_des, &mysembuf, 1); // down semaphore
        int readID=shmget(readKey, sizeof(double)*(size.i)*(size.j)*(size.k), IPC_CREAT); 
        int writeID=shmget(writeKey, sizeof(double)*(size.i)*(size.j)*(size.k), 0); 
        int coefID=shmget(COEFKEY, sizeof(double)*(size.i)*(size.j)*(size.k), 0); 
        double * readFrom= shmat(readID, 0,0); // array writing updates to
        double * writeTo = shmat(writeID, 0,0); // array with original temps
        double * coeffs = shmat(coefID, 0,0); // array with coefficients
        double * allzerolayer = (double *) calloc(size.i*size.j, sizeof(double));
        for(int layerindex = lb; layerindex<=ub; layerindex++){ // calculate per layer
            if(layerindex==size.k-1){ // last layer
                update_layer(readFrom+layerindex*size.i*size.j, readFrom+(layerindex-1)*size.i*size.j, allzerolayer, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
            else if (layerindex ==0){ // first layer
                update_layer(readFrom+layerindex*size.i*size.j, allzerolayer, readFrom+(layerindex+1)*size.i*size.j, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
            else{ // middle layers
                update_layer(readFrom+layerindex*size.i*size.j, readFrom+(layerindex-1)*size.i*size.j, readFrom+(layerindex+1)*size.i*size.j, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
        }
        shmdt(readFrom);
        shmdt(writeTo);
        mysembuf.sem_op = 1;
        semop(sem_des, &mysembuf, 1); // up semaphore
        exit(0);
    }
    else{
        return f;
    }
}

static void sighandler(int signo){
    if(signo == QUIT){
        exit(0);
    }
    else if (signo == ACALCB){
    
    }
}

int spawn_subprocess(vec3i size, int ub, int lb, int order, int mode, double timestep, double units) {
    int f = fork();
    if(f!=0){// return pid if parent
        return f;
    }
    else{
        signal(ACALCB, sighandler);
        signal(BCALCA, sighandler);
        signal(QUIT, sighandler);
    }
}