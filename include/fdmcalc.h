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
Note: if there is no row above/bellow the given row, double * above or double * below will be an array full of 0s

RETURN VALUE 
    double array of row with updated temperatures
*/
double * update_row(double * original, double * plusy, double * minusy, double * plusz, double * minusz, double * coeffs, int i_max, double timestep, double units);
#endif