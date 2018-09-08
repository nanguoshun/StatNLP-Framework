//
// Created by  ngs on 04/09/2018.
//

#include "math_calc.h"

double MathCalc::SquareVector(double *vec, int size) {
    double value = 0.0;
    for(int i = 0; i < size; ++i){
        double weight = vec[i];
        value += weight * weight;
    }
    return value;
}
