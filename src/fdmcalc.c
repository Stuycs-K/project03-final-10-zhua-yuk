#include <stdio.h>
#include <stdlib.h>

#include "fdmcalc.h"


double update_cell(double cc, double* cn, double timestep, double coeff, double units) {
    double second_xROC = (cn[1] - 2*cc + cn[0])/units;
    double second_yROC = (cn[3] - 2*cc + cn[2])/units;
    double second_ZROC = (cn[5] - 2*cc + cn[4])/units;
    return timestep * coeff * (second_xROC + second_yROC + second_ZROC) + cc;
}

void * update_row(double * original, double * plusy, double * minusy, double * plusz, double * minusz, double * coeffs, int i_max, double timestep, double units, double * newrow){
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
}

void ** update_layer(double ** layer, double ** above, double ** below, int i_max, int j_max, double timestep, double units, double ** coeffs, double ** newlayer, int mode){
    double * allzerorow = (double *) calloc(i_max, sizeof(double));
    double * allzerocol = (double *) calloc(j_max, sizeof(double));
    for(int row = 0; row < j_max; row++){
        int i = j_max-1-row;
        if(mode ==0){
            i=row;
        }
        double * currrow = newlayer[i];
        double * plusy = allzerorow;
        double * minusy = allzerorow;
        ifg(row!=0){
            plusy = layer[i-1];
        }
        if(row!=j_max-1){
            minusy=layer[i+1];
        }
        update_row(layer[i], plusy, minusy, above[i], below[i], coeffs[i], i_max, timestep, units, newlayer[i]);
    }
}
