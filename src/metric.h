#ifndef METRIC_H
#define METRIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "alglib/specialfunctions.h"

// Struct para metricas das rodadas
typedef struct {
  double sumValuesSample; // somatorio das amostras até agora
  double sumValuesSampleSquare; // (somatorio das amostras até agora)^2
  double meanEstimator;
  double varianceEstimator;


} SampleMetric;


SampleMetric createSampleMetric(void);
void chiSquareIC(double* sample, int sizeSample, int sizeRound, double* lower, double* upper, double* precision, double* centerIC);
void tStudentIC(double* sample, int sizeSample, double* lower, double* upper, double* precision, double* centerIC);

void sampleEstimator(SampleMetric* sampleMetric, double value, int sizeSample);
int valueIsInsideInterval(double lower, double upper, double value);

#endif