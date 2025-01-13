#ifndef FDMCALC_H
#define FDMCALC_H
#include "config.h"
#include "types.h"

/*
update_cell
    returns the new value of a cell given it's neighbors and the timestamp

ARGS
    double cc - selected cell temperature
    double* cn - array of cell neighbors
        it is always a length 6 array.
        the order is (think about a cube in 3d grid)
        [-x, x, -y, y, -z, z]
    double timestep - the timestep
    double coeff - thermal coeff
    double units - unit per cell length

RETURN VALUE
    new temperature of selected cell
*/
double update_cell(double* original, double* coeffs, grid_dimen dimens, vec3i coord);

/*
update_layer
    updates newlayer with the new temperatures for a layer given the temperatures of the layers directly above and below, coefficients, timestep, and units

ARGS
    double * layer - array of layer's temperatures, going from left to right from the top row to the bottom row
    double * above - array of layer directly above's temperatures, going from left to right from the top row to the bottom row
    double * below - array of layer directly below's temperatures, going from left to right from the top row to the bottom row
    int i_max - number of columns in a layer
    int j_max - number of rows in a layer
    double timestep - timestep
    double units - unit per cell length
    double * coeffs - array of thermal diffusivity coefficients associated with each cell in the layer, going from left to right from the top row to the bottom row
    double * newlayer - array of layer with updated temperatures, going from left to right from the top row to the bottom row
    int mode - will edit from row 0 to last row if mode is 0 and will edit in reverse order (from last row to row 0) otherwise
Note: if there is no layer above/below the given row, double ** above or double ** below are double ** with appropriate dimensions full of 0s

RETURN VALUE
    void
*/
void update_layer(double* original, double* next, double* coeffs, grid_dimen dimens, int start, int nend, int order);
#endif