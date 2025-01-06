#include "config.h"
#ifndef FDMCALC_H
#define FDMCALC_H
double update_cell(double cc, double cl, double cr, double cu, double cd, double coeff);
double * update_row(double * original, double * above, double * below, double * coeffs, int i_max);
#endif