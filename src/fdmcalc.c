#include <stdio.h>
#include <stdlib.h>

#include "fdmcalc.h"


double update_cell(double cc, double* cn, double timestep, double coeff, double units) {
    double second_xROC = (cn[1] - 2*cc + cn[0])/units;
    double second_yROC = (cn[3] - 2*cc + cn[2])/units;
    double second_ZROC = (cn[5] - 2*cc + cn[4])/units;
    return timestep * coeff * (second_xROC + second_yROC + second_ZROC) + cc;
}

double * update_row(double * original, double * plusy, double * minusy, double * plusz, double * minusz, double * coeffs, int i_max, double timestep, double units){
    double * newrow = malloc(sizeof(double)*i_max);
    for(int i = 0; i<i_max; i++){
        double * cn = malloc(sizeof(double)*6);
        *(cn[0]) = 0;
        *(cn[1]) = 0;
        if(i!=0){
            *(cn[0]) = *(original[i-1]);
        }
        if(i!=i_max-1){
            *(cn[1]) = *(original[i+1]);
        }
        *(cn[2])=*minusy[i];
        *(cn[3])=*plusy[i];
        *(cn[4])=*plusz[i];
        *(cn[5])=*minusz[i];
        *newrow[i]=update_cell(*original[i], cn, timestep, *coeffs[i], units);
        free(cn);
    }
    return newrow;
}

double ** update_layer(double ** layer, double ** above, double ** below, int i_max, int j_max, double timestep, double units, double ** coeffs){
    double ** newlayer = malloc(sizeof(double)*j_max);
    for(int row = 0; row < j_max; row++){
        
    }
}
