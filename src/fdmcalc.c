#include "fdmcalc.h"
#include <stdio.h>
#include <stdlib.h>

double update_cell(double cc, double* cn, double timestep, double coeff, double units) {
    double second_xROC = (cn[1] - 2*cc + cn[0])/units;
    double second_yROC = (cn[3] - 2*cc + cn[2])/units;
    double second_ZROC = (cn[5] - 2*cc + cn[4])/units;
    return timestep * coeff * (second_xROC + second_yROC + second_ZROC) + cc;
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
