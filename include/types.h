#ifndef TYPES_H
#define TYPES_H

typedef struct {
    int i, j, k;
} vec3i;

typedef struct {
    vec3i size;
    double ti, tf;
    double dt;
    double units;
    double* matcoeffs;
    int num_materials;
} grid_dimen;

#endif