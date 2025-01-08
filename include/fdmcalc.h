#ifndef FDMCALC_H
#define FDMCALC_H
#include "config.h"
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
double update_cell(double cc, double* cn, double timestep, double coeff, double units);

/*
update_row
    updates newrow with new temperatures for a row given the neighboring rows' temperatures, coefficients, timestep, and units

ARGS
    double * original - row of array temperatures
    double * plusy - row of temperatures above in same layer (+1 y)
    double * minusy - row of temperatures below in same layer (-1 y)
    double * plusz - row of temperatures in row of layer above (+1 z)
    double * minusz - row of temperatures in row of layer below (-1 z)
    double * coeffs - array of thermal coefficients for the row
    int i_max - number of cells in row
    double timestep - timestep
    double units - unit per cell length
    double * newrow - row of updated temperatures
Note: if there is no row above/below the given row, double * above or double * below will be an array full of 0s

RETURN VALUE 
    void
*/
void update_row(double * original, double * plusy, double * minusy, double * plusz, double * minusz, double * coeffs, int i_max, double timestep, double units, double * newrow);
/*
update_layer
    updates newlayer with the new temperatures for a layer given the temperatures of the layers directly above and below, coefficients, timestep, and units

ARGS
    double ** layer - 2D array of layer's temperatures
    double ** above - 2D array of layer directly above's temperatures
    double ** below - 2D array of layer directly below's temperatures
    int i_max - number of columns in a layer
    int j_max - number of rows in a layer
    double timestep - timestep
    double units - unit per cell length
    double ** coeffs - 2D array of thermal diffusivity coefficients associated with each cell in the layer
    double ** newlayer - 2D array of layer with updated temperatures
    int mode - will edit from row 0 to last row if mode is 0 and will edit in reverse order (from last row to row 0) otherwise
Note: if there is no layer above/below the given row, double ** above or double ** below are double ** with appropriate dimensions full of 0s

RETURN VALUE
    void
*/
void update_layer(double ** layer, double ** above, double ** below, int i_max, int j_max, double timestep, double units, double ** coeffs, double ** newlayer, int mode);
#endif