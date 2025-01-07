#include "fdmcalc.h"
#include <stdio.h>
#include <stdlib.h>

/*
args: double cc is current cell temperature, double cl is temperature of cell to its left, double cr is temperature of cell to its right, double cu is temperature of cell above, double cd is temperature of cell below, double coeff is the thermal diffusivity coefficient for that cell, double timestep is the amount of time that passed during this change, double units is the unit per cell
note: if there are no cells to the left/right or above/below a cell, the value of the temperature is left as 0
return: new temperature of cell after the timestep given the initial conditions
*/

double update_cell(double cc, double cl, double cr, double cu, double cd, double coeff, double timestep, double units){
    double second_xROC = (cr-2*cc+cl)/units;
    double second_yROC = (cu-2*cc+cd)/units;
    return timestep*coeff*(second_xROC+second_yROC)-cc;
}

/*
args: double * original is an array of the temperatures of a given row, double * above is an array of the temperatures of the row above the given row, double * below is an array of the temperatures of the row below a given row, double * coeffs is an array of the thermal diffusivity coefficients that correspond to each cell of the row, int i_max is the number of cells in that row, double timestep is the amount of time that passed during this change, double units is the unit per cell 
note: if there is no row above/bellow the given row, double * above or double * below will be an array full of 0s
return: double array of row with updated temperatures
*/
double * update_row(double * original, double * above, double * below, double * coeffs, int i_max, double timestep, double units){
    double * newrow = malloc(sizeof(double)*i_max);
    for(int i = 0; i<i_max; i++){
        double cl = 0;
        double cr = 0;
        if(i!=0){
            cl = *(original[i-1]);
        }
        if(i!=i_max-1){
            cr = *(original[i+1]);
        }
        *newrow[i]=update_cell(*original[i],cl, cr, *above[i], *below[i], *coeffs[i], timestep, units);
    }
    return newrow;
}
