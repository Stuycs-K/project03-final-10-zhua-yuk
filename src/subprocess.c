#include "memory.h"
#include "fdmcalc.h"
#include "config.h"
#include "subprocess.h"
#include <unistd.h>

int spawn_subprocess(vec3i size, int ub, int lb, int order, int mode) {
    int writeTo=4807;
    int readFrom=11707;
    if(mode ==0){
        writeTo=11707;
        readFrom=4807;
    }
    int f = fork();
    if(f==0){
        int sem_des = (1535, 1, 0);
        struct mysembuf; 
        mysembuf.sem_op = -1;
        mysembuf.sem_num = 0; 
        semop(sem_des, mysembuf, 1);
        int readID = shmget(readFrom, sizeof(double)*size.i*size.j*size.k, IPC_RDONLY);
        int writeID = shmget(readFrom, sizeof(double)*size.i*size.j*size.k, IPC_WRONLY);
    }
    else{
        return f;
    }
}
