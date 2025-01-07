#include "fdmcalc.h"
#include <stdio.h>
#include <stdlib.h>

double update_cell(double cc, double cl, double cr, double cu, double cd, double coeff, double timestep, double units){
    double second_xROC = (cr-2*cc+cl)/units;
    double second_yROC = (cu-2*cc+cd)/units;
    return timestep*coeff*(second_xROC+second_yROC)-cc;
}
