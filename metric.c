#include "metric.h"

/*  1 − alfa = 0.95 -> alfa = 0.05
    tstudent(0.025)
*/

SampleMetric createSampleMetric(void){
  SampleMetric sampleMetric;
  
  sampleMetric.lower = 0;
  sampleMetric.upper = 0;
  sampleMetric.sumValuesSample = 0;
  sampleMetric.sumValuesSampleSquare = 0;
  

  return sampleMetric;
}


void meanIC(SampleMetric* sampleMetric, double value, int sizeSample){

    /* Para n>30 os valores da t-student são assintoticos e iguais a normal, logo para 1-alfa=0.95 
    temos que t-student(1-alfa/2)=z(1-alfa/2)=1.960

    */

    double tStudent = 1.95;

    //Define mean Estimator
    double meanEstimator = 0;
    double varianceEstimator = 0;

    sampleMetric->sumValuesSample = sampleMetric->sumValuesSample + value;
    sampleMetric->sumValuesSampleSquare = (sampleMetric->sumValuesSampleSquare + pow(value,2));
    //Só comeca a calcular a variancia e a media a partir de duas coletas
    if (sizeSample > 1) {

        meanEstimator = sampleMetric->sumValuesSample/sizeSample;
        varianceEstimator = sampleMetric->sumValuesSampleSquare/(sizeSample-1) - pow(sampleMetric->sumValuesSample,2)/(sizeSample*(sizeSample-1));

        printf("Estimador media: %f\n",meanEstimator);
        printf("Estimador variancia: %f\n",varianceEstimator);

        sampleMetric->lower = meanEstimator - tStudent*(sqrt(varianceEstimator)/sqrt(sizeSample));
        sampleMetric->upper = meanEstimator + tStudent*(sqrt(varianceEstimator)/sqrt(sizeSample));
        
        

        printf("lower = %lf \n",sampleMetric->lower);
        printf("upper = %lf \n\n",sampleMetric->upper);
    }
        

}

void varianceIC(SampleMetric* sampleMetric, double value, int sizeSample){
    /*  alfa=0.05
        alfa/2=0.025
        1-alfa/2=0.975
     */
    double alpha = 0.05;

    double meanEstimator = 0;
    double varianceEstimator = 0;

    sampleMetric->sumValuesSample = (sampleMetric->sumValuesSample) + value;
    sampleMetric->sumValuesSampleSquare = ((sampleMetric->sumValuesSampleSquare) + pow(value,2));

    meanEstimator = sampleMetric->sumValuesSample/sizeSample;

    varianceEstimator = sampleMetric->sumValuesSampleSquare/(sizeSample-1) - pow(sampleMetric->sumValuesSample,2)/(sizeSample*(sizeSample-1));

    // A função não retorna o quantil, e sim o valor x que a função P(X >= x) = y
    // Pra conseguir o quantil 100(1-alpha/2)% a partir disso fazemos 
    // 1 - P(X >= x) = P(X <= x) = 1 - y = 1 - alpha/2
    // y = alpha/2
    // Similarmente 1 - y = alpha/2 <=> y = 1 - alpha/2
    double chiSquareLower = alglib::invchisquaredistribution((double) (sizeSample-1), 1-alpha/2);
    double chiSquareUpper = alglib::invchisquaredistribution((double) (sizeSample-1), alpha/2);

    double lower = (varianceEstimator/(sizeSample-1))/chiSquareLower;
    double upper = (varianceEstimator/(sizeSample-1))/chiSquareUpper;


}