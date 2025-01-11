#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "utils.h"
#include "types.h"

int getindex(int i, int j, int k, vec3i dimens) {
    return (i*dimens.i + j*dimens.j + k*dimens.k);
}

char** split(char* s, char* delim) {
	int size=0;
	char** out;
	char* token;
	char* buff = (char*)malloc(strlen(s)*sizeof(char));

	strcpy(buff, s);
	for(; token=strsep(&buff, delim); size++);
	out = (char**)malloc((size+1)*sizeof(char*));
	free(buff);
	buff = (char*)malloc(strlen(s)*sizeof(char));
	strcpy(buff, s);

	for (int i=0; (token=strsep(&buff, delim)) && (i<size); i++) {
		out[i] = (char*)malloc(strlen(token)*sizeof(char));
		strcpy(out[i], token);
	}
	
	out[size-1][strlen(out[size-1])] = '\0';
	out[size] = NULL;
	free(buff);
	return out;
}