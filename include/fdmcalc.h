#ifndef FDMCALC_H
#define FDMCALC_H
#include "config.h"
#include "types.h"

/*
update_cell
    returns the new value of a cell given its neighbors, coefficients, and grid dimensions

ARGS
    vec3i coord - vec3i containing i,j,k location for the current cell
    grid_dimen dimens - grid_dimen containing information for general calculation (dt, start and end times, grid size, units, material coefficients)
    double * coeffs - array containing thermal diffusivity coefficients for each cell of the grid
    double * original - array containing temperatures before calculation of each cell in the grid 

RETURN VALUE
    new temperature of selected cell
*/
double update_cell(double* original, double* coeffs, grid_dimen dimens, vec3i coord);

/*
update_layers
    updates newlayer with the new temperatures for a layer given the temperatures of the layers directly above and below, coefficients, timestep, and units

ARGS
    double * original - array containing temperatures before calculation of each cell in the grid 
    double * next - array to edit with updated temperatures after this calculation
    double * coeffs - array containing thermal diffusivity coefficients for each cell of the grid
    grid_dimen dimens - grid_dimen containing information for general calculation (dt, start and end times, grid size, units, material coefficients)
    int start - layer index to start calculating at 
    int nend - layer index to stop calculating at (don't calculate this layer)
    int order - order in which to do the calculation
        1 -> calculate from layer start to layer nend
        0 -> reverse calculation order so go from layer nend to layer start

RETURN VALUE
    void
*/
void update_layers(double* original, double* next, double* coeffs, grid_dimen dimens, int start, int nend, int order);
#endif