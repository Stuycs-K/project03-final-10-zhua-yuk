#ifndef SUBPROCESS_H
#define SUBPROCESS_H
#include "types.h"

/*
calculate_once
    performs FDM calculations on a specific range
    of data in shared memory. downs the semaphore relating to the 
    shared memory.

ARGS
    vec3i size - the size of the grid (in cells)
    int ub - upper bound layer index
    int lb - lower bound layer index
    int order - what order to approach the array from (to minimize race conditions)
        0 -> from the start (0, 0, 0)
        1 -> from the end (i, j, k)
    int mode - which shared memory array is being written to
        0 -> write to shared memory with ATEMPKEY
        1 -> write to shared memory with BTEMPKEY
    double timestep - calculation's timestep
    double units - unit per cell length
        
RETURN VALUE
    void
*/
void calculate_once(vec3i size, int ub, int lb, int order, int mode, double timestep, double units);

/*
sighandler
    handles signals subprocesses will receive
ARGS
    int signo - signal number
        QUIT -> exit
        ACALCB -> calculate, reading from ATEMP and editing BTEMP
        BCALCA -> calculate, reading from BTEMP and editing ATEMP
RETURN VALUE
    void
*/
static void sighandler(int signo);

/*
spawn_subprocess
    forks and attaches signals to child subprocess
ARGS
    none
RETURN VALUE
    -1 (fail) something went wrong
    PID of subprocess
*/
int spawn_subprocess();
#endif
