#ifndef MEMORY_H
#define MEMORY_H


#define FILE_BUFF_SIZE 256
#define OUT_FILE_PATH_SIZE 512

#include "types.h"
/*
read_fdata
    reads in initial conditions .csv file and allocates shared memory
    and writes to it. Always writes to array A in shared memory.
    Also creates the writing file and writes initial metadata to it.

ARGS
    char* path - the path to the .csv file
    char* opath - the path to the output .csv file

RETURN VALUE
    vec3i struct containing dimensions of the grid
    values are all -1 if an error occured
*/
grid_dimen read_fdata(char* path, char* opath);

/*
write_data
    writes data from an array in shared memory to an output
    .csv file. If the file doesn't exist, it creates it, then appends.

ARGS
    char* path - the path of the output .csv file
    int mode - which shared memory array to choose from
        0 -> array A
        1 -> array B

RETURN VALUE
    1 - success
    0 - fail
*/
int write_data(char* path, vec3i size, int mode);

/*
semaphore_setup
    creates the semaphore that all the subproceeses share.
    inits the value to the num_subprocesses argument

ARGS
    int num_processes - number of subprocesses to create (initial semaphore value)

RETURN VALUE
    1 - success
    0 - fail
*/
int semaphore_setup(int num_subprocesses);

/*
shared_mem_setup
    allocates the shared memory. Keys are predetermined.
    Allocates array A, B and the coefficients.

ARGS
    vec3i size - size of space.

RETURN VALUE
    1 - success
    0 - fail
*/
int shared_mem_setup(vec3i size);


int remove_shared_mem();

#endif
