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

grid_dimen read_fdata(char* path, char* opath) {
	grid_dimen out;
	out.size.i = -1;

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
	for (p_count=0; constants[p_count] != NULL; p_count++) {
		printf("%d: %s\n", p_count, constants[p_count]);
	}
	printf("done reading\n");
	if (p_count != NUM_PARAMETERS) {
		fprintf(stderr, "Invalid number of arguments %d!\n", p_count);
		return out;
	}

	//read in data
	out.size.i = atoi(constants[0]);
	out.size.j = atoi(constants[1]);
	out.size.k = atoi(constants[2]);
	out.ti = atof(constants[3]);
	out.tf = atof(constants[4]);
	out.dt = atof(constants[5]);
	out.units = atof(constants[6]);
	out.num_materials = atoi(constants[7]);
	size = out.size.i*out.size.j*out.size.k;
	free(constants);

	//read in materials
	out.matcoeffs = (double*)malloc(sizeof(double)*out.num_materials);
	for (int i=0; i<out.num_materials; i++) {
		fgets(buff, FILE_BUFF_SIZE, data);

		//write it to the new file
		fputs(buff, data_out);

		constants = split(buff, ",");

		//Check if correct number of args
		for (p_count=0; constants[p_count] != NULL; p_count++);
		if (p_count != 3) {
			fprintf(stderr, "Invalid number of material arguments %d!\n", p_count);
			out.size.i = -1;
			return out;
		}

		out.matcoeffs[atoi(constants[0])] = atof(constants[1]);
		printf("%d: %lf\n", i, out.matcoeffs[i]);
		free(constants);
	}

	//allocate shared memory and attach to it
	if (!shared_mem_setup(out.size)) {
		fprintf(stderr, "Error with allocating shared memory: %s\n", strerror(errno));
	}

	//attach to shared memory
	atemp = shmat(shmget(ATEMPKEY,0, 0), 0, 0);
	coeffs = shmat(shmget(COEFKEY,0, 0), 0, 0);
	mats = shmat(shmget(MATKEY, 0, 0), 0, 0);

	printf("attached\n");
	//write to shared memory arrays
	//material arrays
	for (int i=0; i<size; i++) {
		if (fscanf(data, "%d", (mats+i)) == EOF) {
			fprintf(stderr, "Not enough entries in file\n");
			out.size.i = -1;
			return out;
		}

		//write to out file
		fprintf(data_out, "%d", mats[i]);
		if ((i > 0) && ((i+1) % out.size.j == 0)) {
			fprintf(data_out, "\n");
		}
		else {
			fprintf(data_out, ",");
		}
		printf("%d\n", mats[i]);
		//write to mats array
		coeffs[i] = out.matcoeffs[mats[i]];
	}
	printf("read mats\n");
	fprintf(data_out, "\n");
	for (int i=0; i<size; i++) {
		if (fscanf(data, "%lf", (atemp+i)) == EOF) {
			fprintf(stderr, "Not enough entries in file\n");
			out.size.i = -1;
			return out;
		}
		//write to out file
		fprintf(data_out, "%lf", atemp[i]);
		if ((i > 0) && ((i+1) % out.size.j == 0)) {
			fprintf(data_out, "\n");
		}
		else {
			fprintf(data_out, ",");
		}
	}
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
		if ((i > 0) && (i % size.j == 0)) fprintf(out, "\n");
		else fprintf(out, ",");
	}

	return 1;
}

int semaphore_setup(int num_subprocesses) {
	return (semget(SEMKEY, num_subprocesses, IPC_CREAT | IPC_EXCL | 0644) != -1);
}

int shared_mem_setup(vec3i size) {
	int tsize = size.i*size.j*size.k;
	if (shmget(COEFKEY, tsize*sizeof(double), IPC_CREAT | 0640) == -1) return 0;
	if (shmget(ATEMPKEY, tsize*sizeof(double), IPC_CREAT | 0640) == -1) return 0;
	if (shmget(BTEMPKEY, tsize*sizeof(double), IPC_CREAT | 0640) == -1) return 0;
	if (shmget(MATKEY, tsize*sizeof(int), IPC_CREAT | 0640) == -1) return 0;
	return 1;
}

int remove_shared_mem() {
	shmctl(COEFKEY, IPC_RMID, 0);
	shmctl(ATEMPKEY, IPC_RMID, 0);
	shmctl(BTEMPKEY, IPC_RMID, 0);
	shmctl(MATKEY, IPC_RMID, 0);
}
