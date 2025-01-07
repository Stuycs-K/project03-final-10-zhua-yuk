#include "fdmcalc.h"
#include <stdio.h>
#include <stdlib.h>

double update_cell(double cc, double cl, double cr, double cu, double cd, double coeff, double timestep, double units){
    double second_xROC = (cr-2*cc+cl)/units;
    double second_yROC = (cu-2*cc+cd)/units;
    return timestep*coeff*(second_xROC+second_yROC)-cc;
}

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
