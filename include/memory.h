#ifndef MEMORY_H
#define MEMORY_H
#include "config.h"


struct point3i read_fdata(char* path, int mode);

int write_data(char* path, int mode);

int semaphore_setup(int num_subprocesses);

int shared_mem_setup(struct point3i size);

#endif
