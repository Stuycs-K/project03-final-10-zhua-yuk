#include <stdio.h>
#include <stdlib.h>

#include "fdmcalc.h"
#include "types.h"
#include "utils.h"


double update_cell(double* original, double* coeffs, grid_dimen dimens, vec3i coord) {
    //x
    double xl = (coord.i > 0) ? original[getindex(coord.i-1, coord.j, coord.k, dimens.size)] : 0;
    double xr = (coord.i < dimens.size.i-1) ? original[getindex(coord.i+1, coord.j, coord.k, dimens.size)] : 0;
    //y
    double yl = (coord.j > 0) ? original[getindex(coord.i, coord.j-1, coord.k, dimens.size)] : 0;
    double yr = (coord.j < dimens.size.j-1) ? original[getindex(coord.i, coord.j+1, coord.k, dimens.size)] : 0;
    //z
    double zl = (coord.k > 0) ? original[getindex(coord.i, coord.j, coord.k-1, dimens.size)] : 0;
    double zr = (coord.k < dimens.size.k-1) ? original[getindex(coord.i, coord.j, coord.k+1, dimens.size)] : 0;

    double cc = original[getindex(coord.i, coord.j, coord.k, dimens.size)];
    double laplacian = ((xr - 2*cc + xl) + (yr - 2*cc + yl) + (zr - 2*cc + zl))/(dimens.units*dimens.units);
    printf("coef: %lf\n", coeffs[getindex(coord.i, coord.j, coord.k, dimens.size)]);
    printf("lap: %lf\n", laplacian);
    printf("units: %lf\n", dimens.units);
    return dimens.dt*coeffs[getindex(coord.i, coord.j, coord.k, dimens.size)]*laplacian + cc;
}

void update_layers(double* original, double* next, double* coeffs, grid_dimen dimens, int start, int nend, int order) {
    vec3i coord;
    if (order) {
        for (coord.i=start; coord.i<nend; coord.i++) {
            for (coord.j=0; coord.j<dimens.size.j; coord.j++) {
                for (coord.k=0; coord.k<dimens.size.k; coord.k++) {
                    next[getindex(coord.i, coord.j, coord.k, dimens.size)] = update_cell(original, coeffs, dimens, coord);
                }
            }
        }
    }
    else {
        for (coord.i=nend-1; coord.i>=0; coord.i--) {
            for (coord.j=dimens.size.j-1; coord.j>=0; coord.j--) {
                for (coord.k=dimens.size.k-1; coord.k>=0; coord.k--) {
                    //printf("C: %lf, (%d, %d, %d)\n", original[getindex(coord.i, coord.j, coord.k, dimens.size)], coord.i, coord.j, coord.k);
                    next[getindex(coord.i, coord.j, coord.k, dimens.size)] = update_cell(original, coeffs, dimens, coord);
                }
            }
        }
    }
}

