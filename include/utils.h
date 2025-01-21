#ifndef UTILS_H
#define UTILS_H
#include "types.h"

/*
getindex
    gets index of specific cell in grid's array

ARGS
    int i - desired row
    int j - desired column
    int k - desired layer
    vec3i dimens - dimensions of entire grid

RETURN VALUE
    index of cell in overall grid's array
*/
int getindex(int i, int j, int k, vec3i dimens);


/*
split
    splits char * s by delimiter

ARGS
    char * s - string to divide
    char * delim - delimiter to separate s by

RETURN VALUE
    char ** array with original char * s divided by the delimeters
*/
char** split(char* s, char* delim);

void free_doublechar(char** s);

#endif