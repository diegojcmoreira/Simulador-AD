#include "metric.h"

/*  1 − alfa = 0.95 -> alfa = 0.05
    tstudent(0.025)
*/


double meanIC(double valuesSample[], int sizeSample){

    /* Para n>30 os valores da t-student são assintoticos e iguais a normal, logo para 1-alfa=0.95 
    temos que t-student(1-alfa/2)=z(1-alfa/2)=1.960

    */

    double tStudent = 1.960;

    //Define mean Estimator
    double meanEstimator = 0;
    double varianceEstimator = 0;

    for(int i=0; i<sizeSample; i++){
        meanEstimator = meanEstimator + valuesSample[i];
    }
    meanEstimator = meanEstimator/sizeSample;

    for(int i=0; i<sizeSample; i++){
        varianceEstimator = varianceEstimator + pow((valuesSample[i]- meanEstimator),2);
    }

    varianceEstimator = varianceEstimator/(sizeSample -1);

    double lower = meanEstimator - tStudent*(sqrt(varianceEstimator)/sqrt(sizeSample));
    double upper = meanEstimator + tStudent*(sqrt(varianceEstimator)/sqrt(sizeSample));

    printf("lower = %lf \n",lower);
    printf("upper = %lf \n",upper);
    

}

double varianceIC(double valuesSample[], int sizeSample, double chiSquareLower, double chiSquareUpper ){
    /*  alfa=0.05
        alfa/2=0.025
        1-alfa/2=0.975
     */

    double meanEstimator = 0;
    double varianceEstimator = 0;

    for(int i=0; i<sizeSample; i++){
        meanEstimator = meanEstimator + valuesSample[i];
    }
    meanEstimator = meanEstimator/sizeSample;

    for(int i=0; i<sizeSample; i++){
        varianceEstimator = varianceEstimator + pow((valuesSample[i]- meanEstimator),2);
    }

    double lower = (varianceEstimator/(sizeSample-1))/chiSquareLower;
    double upper = (varianceEstimator/(sizeSample-1))/chiSquareUpper;


}