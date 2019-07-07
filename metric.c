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
  sampleMetric.meanEstimator = 0;
  sampleMetric.varianceEstimator = 0;
  

  return sampleMetric;
}

double tStudentValue(int grausLiberdade){
    switch (grausLiberdade){
        case 1:
            return 12.706; 
        case 2:
            return 4.303; 
        case 3:
            return 3.182; 
        case 4:
            return 2.776; 
        case 5:
            return 2.571; 
        case 6:
            return 2.447; 
        case 7:
            return 2.365; 
        case 8:
            return 2.306; 
        case 9:
            return 2.262; 
        case 10:
            return 2.228; 
        case 11:
            return 2.201; 
        case 12:
            return 2.179; 
        case 13:
            return 2.160; 
        case 14:
            return 2.145; 
        case 15:
            return 2.131; 
        case 16:
            return 2.120; 
        case 17:
            return 2.110; 
        case 18:
            return 2.101; 
        case 19:
            return 2.093; 
        case 20:
            return 2.086; 
        case 21:
            return 2.080; 
        case 22:
            return 2.074; 
        case 23:
            return 2.069; 
        case 24:
            return 2.064; 
        case 25:
            return 2.060; 
        case 26:
            return 2.056; 
        case 27:
            return 2.052; 
        case 28:
            return 2.048; 
        case 29:
            return 2.045; 
        default:
            return 1.960;
    }
}


void sampleEstimator(SampleMetric* sampleMetric, double value, int sizeSample){

    /* Para n>30 os valores da t-student são assintoticos e iguais a normal, logo para 1-alfa=0.95 
    temos que t-student(1-alfa/2)=z(1-alfa/2)=1.960

    */

    //double tStudent = tStudentValue(sizeSample);

    //Define mean Estimator
    double meanEstimator = 0;
    double varianceEstimator = 0;

    sampleMetric->sumValuesSample = sampleMetric->sumValuesSample + value;
    //printf(" antes: %f -> Depiis: %f\n", value,sampleMetric->sumValuesSample );
    sampleMetric->sumValuesSampleSquare = (sampleMetric->sumValuesSampleSquare + pow(value,2));
    //Só comeca a calcular a variancia e a media a partir de duas coletas
    if (sizeSample > 1) {

        sampleMetric->meanEstimator = sampleMetric->sumValuesSample/sizeSample;
        sampleMetric->varianceEstimator = sampleMetric->sumValuesSampleSquare/(sizeSample-1) - pow(sampleMetric->sumValuesSample,2)/(sizeSample*(sizeSample-1));

    }
        

}

void meanIC(SampleMetric* sampleMetric, double value, int sizeSample){
    double tStudent = tStudentValue(sizeSample);
    sampleMetric->lower = sampleMetric->meanEstimator - tStudent*(sqrt(sampleMetric->varianceEstimator)/sqrt(sizeSample));
    sampleMetric->upper = sampleMetric->meanEstimator + tStudent*(sqrt(sampleMetric->varianceEstimator)/sqrt(sizeSample));
    printf("lower = %lf \n",sampleMetric->lower);
    printf("upper = %lf \n\n",sampleMetric->upper);

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
    //double chiSquareLower = alglib::invchisquaredistribution((double) (sizeSample-1), 1-alpha/2);
    //double chiSquareUpper = alglib::invchisquaredistribution((double) (sizeSample-1), alpha/2);

    //double lower = (varianceEstimator/(sizeSample-1))/chiSquareLower;
    //double upper = (varianceEstimator/(sizeSample-1))/chiSquareUpper;


}