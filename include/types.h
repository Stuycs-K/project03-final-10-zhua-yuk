#ifndef TYPES_H
#define TYPES_H
#define NUM_MATERIALS 256   // max number of materials

/*
vec3i
    stores coordinates/dimensions for a cell/grid
*/
typedef struct {
    int i, j, k;    // row, col, layer
} vec3i;

/*
grid_dimensions
    stores coordinates/dimensions for a cell/grid
*/
typedef struct {
    vec3i size;                         // grid dimensions
    double ti, tf;                      // starting and ending time
    double dt;                          // timestep difference for calculations
    double units;                       // measured difference in units between adjacent cells
    double matcoeffs[NUM_MATERIALS];    // coefficients for materials
    int num_materials;                  // number of materials in grid
} grid_dimen;

/*
union semun
    stores semaphore data
*/
union semun {
    int val;                            // value for SETVAL
    struct semid_ds *buf;    
    unsigned short  *array;  
    struct seminfo  *__buf;  
};
#endif
