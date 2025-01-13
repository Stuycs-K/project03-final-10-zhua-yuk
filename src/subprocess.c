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

int spawn_subprocess(vec3i size, int ub, int lb, int order, int mode, double timestep, double units) {
    int writeKey=BTEMPKEY;
    int readKey=ATEMPKEY;
    if(mode ==0){
        writeKey=ATEMPKEY;
        readKey=BTEMPKEY;
    }
    int f = fork();
    if(f==0){
        // printf("child!\n");
        int sem_des = semget(SEMKEY, 1, 0);
        struct sembuf mysembuf; 
        mysembuf.sem_op = -1;
        mysembuf.sem_num = 0; 
        // printf("going to down!\n");
        semop(sem_des, &mysembuf, 1); // down semaphore
        int readID=shmget(readKey, sizeof(double)*(size.i)*(size.j)*(size.k), IPC_CREAT); 
        int writeID=shmget(writeKey, sizeof(double)*(size.i)*(size.j)*(size.k), 0); 
        int coefID=shmget(COEFKEY, sizeof(double)*(size.i)*(size.j)*(size.k), 0); 
        // printf("read, write, coef IDs: %d %d %d\n", readID, writeID, coefID);
        double * readFrom= shmat(readID, 0,0); // array writing updates to
        // for(int ind = 0; ind<8; ind++){
        //     printf("[%d]: %f \n", ind, *(readFrom+ind));
        // }
        double * writeTo = shmat(writeID, 0,0); // array with original temps
        double * coeffs = shmat(coefID, 0,0); // array with coefficients
        double * allzerolayer = (double *) calloc(size.i*size.j, sizeof(double));
        // printf(" lb: %d, ub: %d\n", lb, ub);
        for(int layerindex = lb; layerindex<=ub; layerindex++){ // calculate per layer
            if(layerindex==size.k-1){ // last layer
                // printf("updating last layer\n");
                // double * p = readFrom+layerindex*size.i*size.j;
                // for(int ind = 0; ind<4; ind++){
                //     printf("LL[%d]: %f \nAllZeroLayer: %f\nBelow: %f\n------------------------\n", ind, *(p+ind), *(allzerolayer+ind), *(readFrom+(layerindex+1)*size.i*size.j+ind));
                // }
                update_layer(readFrom+layerindex*size.i*size.j, readFrom+(layerindex-1)*size.i*size.j, allzerolayer, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
            else if (layerindex ==0){ // first layer
                // printf("updating layer 0\n");
                // double * p = readFrom+layerindex*size.i*size.j;
                // for(int ind = 0; ind<4; ind++){
                //     printf("L0[%d]: %f \nAllZeroLayer: %f\nBelow: %f\n------------------------\n", ind, *(p+ind), *(allzerolayer+ind), *(readFrom+(layerindex+1)*size.i*size.j+ind));
                // }
                update_layer(readFrom+layerindex*size.i*size.j, allzerolayer, readFrom+(layerindex+1)*size.i*size.j, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
            else{ // middle layers
                // printf("updating middle layer\n");
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
