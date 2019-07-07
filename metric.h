#ifndef METRIC_H
#define METRIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "alglib/specialfunctions.h"

// Struct para o intervalo de confianca, seja ele da media ou da variancia
typedef struct {
  double lower; //limiar minimo do intervalo de confiança
  double upper; //limiar maximo do intervalo de confiança
  double sumValuesSample; // somatorio das amostras até agora
  double sumValuesSampleSquare; // (somatorio das amostras até agora)^2
  double meanEstimator;
  double varianceEstimator;


} SampleMetric;


SampleMetric createSampleMetric(void);
void meanIC(SampleMetric* sampleMetric, int sizeSample);
void varianceIC(SampleMetric* sampleMetric, double value, int sizeSample);
double tStudentValue(int grausLiberdade);
void sampleEstimator(SampleMetric* sampleMetric, double value, int sizeSample);

#endif