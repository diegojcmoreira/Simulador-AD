#include "metric.h"

#define ALPHA 0.05

/*  1 − alfa = 0.95 -> alfa = 0.05
    tstudent(0.025)
*/

SampleMetric createSampleMetric(void){
  SampleMetric sampleMetric;
  
  sampleMetric.sumValuesSample = 0;
  sampleMetric.sumValuesSampleSquare = 0;
  sampleMetric.meanEstimator = 0;
  sampleMetric.varianceEstimator = 0;
  

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

void tStudentIC(double* sample, int sizeSample, double* lower, double* upper, double* precision, double* centerIC){
    double tStudent = 0;
    double meanEstimator = 0;
    double varianceEstimator = 0;
    if(sizeSample > 1){
        tStudent = alglib::invstudenttdistribution((double) (sizeSample-1), 1-ALPHA/2);
        
        //Calculando o estimador da media da rodadas
        for (int i=0; i< sizeSample; i++){
            meanEstimator += sample[i];
        }
        meanEstimator = meanEstimator/sizeSample;
        
        //Calcular o estimador da variancia
        for (int i=0; i< sizeSample; i++){
            varianceEstimator += pow(sample[i] - meanEstimator,2);
            
            
        }
        varianceEstimator = varianceEstimator/(sizeSample-1);

        *lower = meanEstimator - tStudent*(sqrt(varianceEstimator)/sqrt(sizeSample));
        *upper = meanEstimator + tStudent*(sqrt(varianceEstimator)/sqrt(sizeSample));

        *centerIC = (*upper + *lower)/2;

        *precision = ((*upper - *lower)/(*upper + *lower))*100;

    }
}

void chiSquareIC(double* sample, int sizeSample, int sizeRound, double* lower, double* upper, double* precision, double* centerIC){
    double meanEstimator = 0;
    double varianceEstimator = 0;
    
    

    if(sizeSample > 1) {
        /*  A função não retorna o quantil, e sim o valor x que a função P(X >= x) = y
        Pra conseguir o quantil 100(1-alpha/2)% a partir disso fazemos 
        1 - P(X >= x) = P(X <= x) = 1 - y = 1 - alpha/2
        y = alpha/2
        Similarmente 1 - y = alpha/2 <=> y = 1 - alpha/2
        */

        double chiSquareUpper = alglib::invchisquaredistribution((double) (sizeSample-1), 1-ALPHA/2);
        double chiSquareLower = alglib::invchisquaredistribution((double) (sizeSample-1), ALPHA/2);

        for (int i=0; i< sizeSample; i++){
            meanEstimator += sample[i];
            //Calcular o estimador da media
        }
        meanEstimator = meanEstimator/sizeSample;
        
        for (int i=0; i< sizeSample; i++){
        
            varianceEstimator += pow(sample[i] - meanEstimator,2);
            //Calcular o estimador da media
        }
        varianceEstimator = varianceEstimator/(sizeSample-1);


        *lower = (meanEstimator*(sizeSample-1))/chiSquareLower;
        *upper = (meanEstimator*(sizeSample-1))/chiSquareUpper;
        *centerIC = (*upper + *lower)/2;

        *precision = ((chiSquareLower - chiSquareUpper)/(chiSquareLower + chiSquareUpper))*100;


    }
}

int valueIsInsideInterval(double lower, double upper, double value){
    return (value >= lower && value <= upper);
}