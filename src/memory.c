#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "config.h"
#include "memory.h"

vec3i read_fdata(char* path) {
	vec3i out;
	out.i = -1;
	out.j = -1;
	out.k = -1;

	double ti, tf;
	double dt;
	double dd;
	double units;
	int num_materials;

	char buff[255];
	char** constants;
	//open file
	FILE* data = fopen(path, "r");

	//read in constants
	fgets(buff, 255, data);
	constants = split(buff, ",");

	//check if number of constants is correct
	int p_count;
	for (p_count=0; constants[p_count] != NULL; p_count++);
	if (p_count+1 != NUM_PARAMETERS) {
		fprintf(stderr, "Invalid number of arguments %d!\n", p_count);
		return out;
	}

	//read in data
	out.i = atoi(constants[0]);
	out.j = atoi(constants[1]);
	out.k = atoi(constants[2]);
	ti = atof(constants[3]);
	tf = atof(constants[4]);
	dt = atof(constants[5]);
	dd = atof(constants[6]);
	units = atof(constants[7]);
	num_materials = atoi(constants[8]);

	//read in materials


	//allocate shared memory



	//write to shared memory array





	return out;
}

int write_data(char* path, int mode) {
	return 1;
}

int semaphore_setup(int num_subprocesses) {
	return 1;
}

int shared_mem_setup(vec3i size) {
	int tsize = size.x*size.y*size.z;
	int memrets[4];
	memrets[0] = shmget(COEFFKEY, tsize*sizeof(double), IPC_CREAT);
	memrets[1] = shmget(ATEMPKEY, tsize*sizeof(double), IPC_CREAT);
	memrets[2] = shmget(BTEMPKEY, tsize*sizeof(double), IPC_CREAT);
	memrets[3] = shmget(MATKEY, tsize*sizeof(double), IPC_CREAT);
	//Check if all allocation was done successfully
	for (int i=0; i<4; i++) {
		if (memrets[i] == -1) {
			return 0;
		}
	}
	return 1;
}
