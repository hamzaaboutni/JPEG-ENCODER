#ifndef DCT_H
#define DCT_H

#include <math.h>

#define PI 3.14159265358979323846
#define N 8

void dct(int **block, int **dctBlock) ;
void prepcalcul_cste();
void applique_dct_unidimentionnel(const double *input, double *output) ;

#endif
