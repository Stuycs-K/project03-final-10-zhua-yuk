#ifndef TYPES_H
#define TYPES_H
#define NUM_MATERIALS 256

typedef struct {
    int i, j, k;
} vec3i;

typedef struct {
    vec3i size;
    double ti, tf;
    double dt;
    double units;
    double matcoeffs[NUM_MATERIALS];
    int num_materials;
} grid_dimen;

union semun {
    int val;   
    struct semid_ds *buf;    
    unsigned short  *array;  
    struct seminfo  *__buf;  
};
#endif
