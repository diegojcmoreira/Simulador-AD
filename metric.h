#ifndef METRIC_H
#define METRIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double meanIC(double valuesSample[], int sizeSample );
double varianceIC(double valuesSample[], int sizeSample, double chiSquareLower, double chiSquareUpper );

#endif