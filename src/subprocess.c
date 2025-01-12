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
        printf("child!\n");
        int sem_des = semget(SEMKEY, 1, 0);
        struct sembuf mysembuf; 
        mysembuf.sem_op = -1;
        mysembuf.sem_num = 0; 
        printf("going to down!\n");
        semop(sem_des, &mysembuf, 1); // down semaphore
        int * readID;
        printf("here! readKey: %d\n", readKey);
        * readID = shmget(readKey, sizeof(double)*(size.i+1)*(size.j+1)*(size.k+1), IPC_CREAT); 
        printf("here!\n");
        int * writeID;
        * writeID = shmget(writeKey, sizeof(double)*(size.i+1)*(size.j+1)*(size.k+1), 0); 
        int * coefID;
        * coefID = shmget(COEFKEY, sizeof(double)*(size.i+1)*(size.j+1)*(size.k+1), 0); 
        double * readFrom= shmat(*readID, 0,0); // array writing updates to
        double * writeTo = shmat(*writeID, 0,0); // array with original temps
        double * coeffs = shmat(*coefID, 0,0); // array with coefficients
        double * allzerolayer = (double *) calloc(size.i*size.j, sizeof(double));
        printf(" lb: %d, ub: %d\n", lb, ub);
        for(int layerindex = lb; layerindex<=ub; layerindex++){ // calculate per layer
            if(layerindex==size.k-1){ // last layer
            printf("updating last layer\n");
                update_layer(readFrom+layerindex*size.i*size.j, readFrom+(layerindex-1)*size.i*size.j, allzerolayer, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
            else if (layerindex ==0){ // first layer
                printf("updating layer 0\n");
                update_layer(readFrom+layerindex*size.i*size.j, allzerolayer, readFrom+(layerindex+1)*size.i*size.j, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
            else{ // middle layers
            printf("updating middle layer\n");
                update_layer(readFrom+layerindex*size.i*size.j, readFrom+(layerindex-1)*size.i*size.j, readFrom+(layerindex+1)*size.i*size.j, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
        }
        shmdt(readFrom);
        shmdt(writeTo);
        mysembuf.sem_op = 1;
        semop(sem_des, &mysembuf, 1); // up semaphore
    }
    else{
        return f;
    }
}
