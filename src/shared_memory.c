#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "config.h"
#include "types.h"
#include "utils.h"
#include "shared_memory.h"

vec3i read_fdata(char* path, char* opath) {
	vec3i out;
	out.i = -1;

	double ti, tf;
	double dt;
	double dd;
	double units;
	double* matcoeffs;
	int num_materials;

	char buff[FILE_BUFF_SIZE];
	char** constants;

	int p_count;

	//shared memory segments
	double* atemp;
	double* coeffs;
	int* mats;

	int size;

	//open files
	FILE* data = fopen(path, "r");
	FILE* data_out = fopen(opath, "w");

	//read in constants
	fgets(buff, FILE_BUFF_SIZE, data);
	constants = split(buff, ",");

	//print it to metadata of out file
	fputs(buff, data_out);

	//check if number of constants is correct
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
	size = out.i*out.j*out.k;
	free(constants);

	//read in materials
	matcoeffs = (double*)malloc(sizeof(double)*num_materials);
	for (int i=0; i<num_materials; i++) {
		fgets(buff, FILE_BUFF_SIZE, data);

		//write it to the new file
		fputs(buff, data_out);

		constants = split(buff, ",");

		//Check if correct number of args
		for (p_count=0; constants[p_count] != NULL; p_count++);
		if (p_count+1 != 3) {
			fprintf(stderr, "Invalid number of material arguments %d!\n", p_count);
			out.i = -1;
			return out;
		}

		matcoeffs[atoi(constants[0])] = atof(constants[1]);
		free(constants);
	}

	//allocate shared memory and attach to it
	if (!shared_mem_setup(out)) {
		fprintf(stderr, "Error with allocating shared memory: %s\n", strerror(errno));
	}

	//attach to shared memory
	atemp = (double*)shmat(ATEMPKEY, 0, 0);
	coeffs = (double*)shmat(COEFKEY, 0, 0);
	mats = (int*)shmat(MATKEY, 0, 0);

	//write to shared memory arrays
	//material arrays
	for (int i=0; i<size; i++) {
		if (fscanf(data, "%d", (mats+i)) == EOF) {
			fprintf(stderr, "Not enough entries in file\n");
			out.i = -1;
			return out;
		}
		//write to out file
		fprintf(data_out, "%d", mats[i]);
		if (i % out.j == 0) fprintf(data_out, "\n");
		else fprintf(data_out, ",");
		
		//write to mats array
		coeffs[i] = matcoeffs[mats[i]];
	}
	fprintf(data_out, "\n");
	for (int i=0; i<size; i++) {
		if (fscanf(data, "%lf", (atemp+i)) == EOF) {
			fprintf(stderr, "Not enough entries in file\n");
			out.i = -1;
			return out;
		}
		//write to out file
		fprintf(data_out, "%lf", atemp[i]);
		if (i % out.j == 0) fprintf(data_out, "\n");
		else fprintf(data_out, ",");
	}
	fprintf(data_out, "\n");

	//close shared memory and files
	shmdt(atemp);
	shmdt(coeffs);
	shmdt(mats);

	fclose(data);
	fclose(data_out);

	return out;
}

int write_data(char* path, vec3i size, int mode) {
	FILE* out = fopen(path, "w");
	double* data;
	if (mode) data = (double*)shmat(BTEMPKEY, 0, 0);
	else data = (double*)shmat(ATEMPKEY, 0, 0);

	for (int i=0; i<size.i*size.j*size.k; i++) {
		fprintf(out, "%lf", data[i]);
		if (i % size.j == 0) fprintf(out, "\n");
		else fprintf(out, ",");
	}

	return 1;
}

int semaphore_setup(int num_subprocesses) {
	return (semget(SEMKEY, num_subprocesses, IPC_CREAT | IPC_EXCL | 0644) != -1);
}

int shared_mem_setup(vec3i size) {
	int tsize = size.i*size.j*size.k;
	if (shmget(COEFKEY, tsize*sizeof(double), IPC_CREAT) == -1) return 0;
	if (shmget(ATEMPKEY, tsize*sizeof(double), IPC_CREAT) == -1) return 0;
	if (shmget(BTEMPKEY, tsize*sizeof(double), IPC_CREAT) == -1) return 0;
	if (shmget(MATKEY, tsize*sizeof(int), IPC_CREAT) == -1) return 0;
	return 1;
}
