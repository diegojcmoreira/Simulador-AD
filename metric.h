#ifndef METRIC_H
#define METRIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <random>

double meanIC(double sumValuesSample, double sumValuesSampleSquare, int sizeSample, double value);
double varianceIC(double sumValuesSample, double sumValuesSampleSquare, int sizeSample, double value);

#endif