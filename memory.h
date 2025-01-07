#include "config.h"
#ifndef MEMORY_H
#define MEMORY_H
struct grid_dimens read_fdata(char* path, int mode);
int write_data(char* path, int mode);
int semaphore_setup(int num_subprocesses);
int shared_mem_setup(int i_max, int j_max, int k_max);
#endif