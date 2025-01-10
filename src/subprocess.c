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

int spawn_subprocess(vec3i size, int ub, int lb, int order, int mode, double timestep, double units) {
    int writeTo=4807;
    int readFrom=11707;
    if(mode ==0){
        writeTo=11707;
        readFrom=4807;
    }
    int f = fork();
    if(f==0){
        int sem_des = (1535, 1, 0);
        struct sembuf mysembuf; 
        mysembuf.sem_op = -1;
        mysembuf.sem_num = 0; 
        semop(sem_des, &mysembuf, 1);
        int readID = shmget(readFrom, sizeof(double)*size.i*size.j*size.k, SHM_RDONLY);
        int writeID = shmget(writeTo, sizeof(double)*size.i*size.j*size.k, SHM_WRONLY);
        int coefID = shmget(12125, sizeof(double)*size.i*size.j*size.k, SHM_RDONLY);
        int readFrom= shmat(readID, 0,0);
        int writeTo = shmat(writeTo, 0,0);
        int coeffs = shmat(coefID, 0,0);
        double * allzerolayer = (double *) calloc(size.i*size.j, sizeof(double));
        for(int layerindex = lb; layerindex<=ub; layerindex++){
            if(layerindex==size.k){ // last layer
                update_layer(readFrom+layerindex*size.i*size.j, readFrom+(layerindex-1)*size.i*size.j, allzerolayer, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
            else if (layerindex ==1){ // first layer
                update_layer(readFrom+layerindex*size.i*size.j, allzerolayer, readFrom+(layerindex+1)*size.i*size.j, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
            else{
                update_layer(readFrom+layerindex*size.i*size.j, readFrom+(layerindex-1)*size.i*size.j, readFrom+(layerindex+1)*size.i*size.j, size.i, size.j,timestep,units, coeffs+layerindex*size.i*size.j, writeTo+layerindex*size.i*size.j,order);
            }
        }
        shmdt(readFrom);
        shmdt(writeTo);
    }
    else{
        return f;
    }
}
