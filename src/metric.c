#include "metric.h"

#define ALPHA 0.05

/*  1 − alfa = 0.95 -> alfa = 0.05
    tstudent(0.025)
*/

SampleMetric createSampleMetric(void){
  SampleMetric sampleMetric;
  
  sampleMetric.lower = 0;
  sampleMetric.upper = 0;
  sampleMetric.sumValuesSample = 0;
  sampleMetric.sumValuesSampleSquare = 0;
  sampleMetric.meanEstimator = 0;
  sampleMetric.varianceEstimator = 0;
  sampleMetric.precisionIC = 0;
  

  return sampleMetric;
}

void sampleEstimator(SampleMetric* sampleMetric, double value, int sizeSample){


    //Define mean Estimator
    double meanEstimator = 0;
    double varianceEstimator = 0;

    sampleMetric->sumValuesSample = sampleMetric->sumValuesSample + value;
    sampleMetric->sumValuesSampleSquare = (sampleMetric->sumValuesSampleSquare + pow(value,2));
    //Só comeca a calcular os estimadores a partir de duas amostras
    if (sizeSample > 1) {

        sampleMetric->meanEstimator = sampleMetric->sumValuesSample/sizeSample;
        sampleMetric->varianceEstimator = sampleMetric->sumValuesSampleSquare/(sizeSample-1) - pow(sampleMetric->sumValuesSample,2)/(sizeSample*(sizeSample-1));

    }
        

}

void meanIC(SampleMetric* sampleMetric, int sizeSample){
    double tStudent = alglib::invstudenttdistribution((double) (sizeSample-1), 1-ALPHA/2);
    printf("tstudent: %f\n", tStudent);
    if(sizeSample > 0){
        sampleMetric->lower = sampleMetric->meanEstimator - tStudent*(sqrt(sampleMetric->varianceEstimator)/sqrt(sizeSample));
        sampleMetric->upper = sampleMetric->meanEstimator + tStudent*(sqrt(sampleMetric->varianceEstimator)/sqrt(sizeSample));

        sampleMetric->precisionIC = (sampleMetric->upper - sampleMetric->lower)/(sampleMetric->upper + sampleMetric->lower);
    
    }
}

void varianceIC(SampleMetric* sampleMetric, int sizeSample){

    /*  A função não retorna o quantil, e sim o valor x que a função P(X >= x) = y
        Pra conseguir o quantil 100(1-alpha/2)% a partir disso fazemos 
        1 - P(X >= x) = P(X <= x) = 1 - y = 1 - alpha/2
        y = alpha/2
        Similarmente 1 - y = alpha/2 <=> y = 1 - alpha/2
    */

    if(sizeSample > 1) {
        double chiSquareUpper = alglib::invchisquaredistribution((double) (sizeSample-1), 1-ALPHA/2);
        double chiSquareLower = alglib::invchisquaredistribution((double) (sizeSample-1), ALPHA/2);

        sampleMetric->lower = (sampleMetric->varianceEstimator*(sizeSample-1))/chiSquareLower;
        sampleMetric->upper = (sampleMetric->varianceEstimator*(sizeSample-1))/chiSquareUpper;
        sampleMetric->precisionIC = (chiSquareLower - chiSquareUpper)/(chiSquareLower + chiSquareUpper);

    }



}