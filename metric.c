#include "metric.h"

/*  1 − alfa = 0.95 -> alfa = 0.05
    tstudent(0.025)
*/


double meanIC(double sumValuesSample, double sumValuesSampleSquare, int sizeSample, double value){

    /* Para n>30 os valores da t-student são assintoticos e iguais a normal, logo para 1-alfa=0.95 
    temos que t-student(1-alfa/2)=z(1-alfa/2)=1.960

    */

    std::student_t_distribution<double> distribution(double(sizeSample));

    double tStudent = distribution(0.95);

    printf("tStudent: %f",tStudent);

    //Define mean Estimator
    double meanEstimator = 0;
    double varianceEstimator = 0;

    meanEstimator = (value + sumValuesSample)/sizeSample;

    varianceEstimator = (sumValuesSampleSquare + pow(value,2))/(sizeSample-1) - pow(value + sumValuesSample,2)/(sizeSample*(sizeSample-1));

    double lower = meanEstimator - tStudent*(sqrt(varianceEstimator)/sqrt(sizeSample));
    double upper = meanEstimator + tStudent*(sqrt(varianceEstimator)/sqrt(sizeSample));

    printf("lower = %lf \n",lower);
    printf("upper = %lf \n",upper);
    

}

double varianceIC(double sumValuesSample, double sumValuesSampleSquare, int sizeSample, double value){
    /*  alfa=0.05
        alfa/2=0.025
        1-alfa/2=0.975
     */

    double meanEstimator = 0;
    double varianceEstimator = 0;

    meanEstimator = (value + sumValuesSampleSquare)/sizeSample;
    
    varianceEstimator = (sumValuesSampleSquare + pow(value,2))/(sizeSample-1) - pow(value + sumValuesSample,2)/(sizeSample*(sizeSample-1));


    //double lower = (varianceEstimator/(sizeSample-1))/chiSquareLower;
    //double upper = (varianceEstimator/(sizeSample-1))/chiSquareUpper;


}