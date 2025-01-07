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
args: double * original is an array of the temperatures of a given row, double * above is an array of the temperatures of the row above the given row, double * below is an array of the temperatures of the row below a given row, double * coeffs is an array of the thermal diffusivity coefficients that correspond to each cell of the row, int i_max is the number of cells in that row, double timestep is the amount of time that passed during this change, double units is the unit per cell 
note: if there is no row above/bellow the given row, double * above or double * below will be an array full of 0s
return: double array of row with updated temperatures
*/
double * update_row(double * original, double * above, double * below, double * coeffs, int i_max, double timestep, double units);
#endif