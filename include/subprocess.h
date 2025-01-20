#ifndef SUBPROCESS_H
#define SUBPROCESS_H
#include "types.h"

extern grid_dimen DIMENSIONS;
extern int START, NEND, ORDER;

/*
calculate_once
    performs FDM calculations on a specific range
    of data in shared memory. ups the semaphore relating to the 
    shared memory.

ARGS
    int mode - which shared memory array is being written to
        0 -> write to shared memory with ATEMPKEY
        1 -> write to shared memory with BTEMPKEY
        
RETURN VALUE
    void
*/
void calculate_once(int mode);

/*
spawn_subprocess
    edits static variables for calculations, forks, and has child perform calcuations when parent sends messages through pipe to calculate 
ARGS
    int start - lower bound layer index (inclusive)
    int nend - upper bound layer index (not inclusive)
    int order - what order to approach the array from (to minimize race conditions)
        0 -> from the start (0, 0, 0)
        1 -> from the end (i, j, k)
    int pipe - read end file descriptor of subprocess' specific pipe
RETURN VALUE
    -1 (fail) something went wrong
    PID of subprocess
*/

int spawn_subprocess(int start, int nend, int order, int pipe);

#endif
