#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <math.h>
#include <errno.h>

#include "config.h"
#include "types.h"
#include "utils.h"
#include "shared_memory.h"

grid_dimen read_fdata(char* path, char* opath) {
	grid_dimen out;
	out.size.i = -1;

	char* buff = (char*)malloc(sizeof(char)*FILE_BUFF_SIZE);
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

	//write to shared memory arrays
	//material arrays
	free(buff);
	buff = (char*)malloc(sizeof(char)*out.size.i*20);
	for (int i=0; i<out.size.k*out.size.j; i++) {
		//Read in line and split
		fgets(buff, out.size.i*20, data);
		constants = split(buff, ",");

		//parse split data
		for (int y=0; constants[y] != NULL; y++) {
			if (!strcmp(constants[y], "\n")) {
				break;
			}
			int mat = atoi(constants[y]);
			mats[y + i*out.size.i] = mat;
			coeffs[y + i*out.size.i] = out.matcoeffs[mat];

			//write to out file
			fprintf(data_out, "%d", mat);
			if (constants[y+1] == NULL) {
				fprintf(data_out, "\n");
			}
			else {
				fprintf(data_out, ",");
			}
		}

		free(constants);
	}

	//read in initial temperatures
	for (int i=0; i<out.size.k*out.size.j; i++) {
		//Read in line and split
		fgets(buff, out.size.i*20, data);
		constants = split(buff, ",");

		//parse split data
		for (int y=0; constants[y] != NULL; y++) {
			if (!strcmp(constants[y], "\n")) {
				break;
			}
			float temp = atof(constants[y]);
			atemp[y + i*out.size.i] = temp;
			
			//write to out file
			fprintf(data_out, "%lf", temp);
			if (constants[y+1] == NULL) {
				fprintf(data_out, "\n");
			}
			else {
				fprintf(data_out, ",");
			}
		}

		free(constants);
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
	FILE* out = fopen(path, "a");
	double* data;
	if (mode) {
		data = shmat(shmget(ATEMPKEY, 0, 0), 0, 0);
	}
	else {
		data = shmat(shmget(BTEMPKEY, 0, 0), 0, 0);
	}

	for (int i=0; i<size.i*size.j*size.k; i++) {
		fprintf(out, "%lf", data[i]);
		printf("%lf", data[i]);
		if ((i > 0) && ((i-1) % size.j == 0)) {
			fprintf(out, "\n");
			printf("\n");
		}
		else {
			fprintf(out, ",");
			printf(",");
		}
	}
	fclose(out);

	return 1;
}

int semaphore_setup() {
	union semun semDATA;
	semDATA.val = 0;
	int semDes = semget(SEMKEY, 1, IPC_CREAT | 0640);
	semctl(semDes, 0, SETVAL, semDATA);
	return semDes;
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
	if (shmctl(shmget(COEFKEY, 0, 0), IPC_RMID, 0) == -1) return 0;
	if (shmctl(shmget(ATEMPKEY, 0, 0), IPC_RMID, 0) == -1) return 0;
	if (shmctl(shmget(BTEMPKEY, 0, 0), IPC_RMID, 0) == -1) return 0;
	if (shmctl(shmget(MATKEY, 0, 0), IPC_RMID, 0) == -1) return 0;
	return 1;
}

int remove_semaphores() {
	if (semctl(semget(SEMKEY, 0, 0), 0, IPC_RMID) == 0) return 0;
	return 1;
}
