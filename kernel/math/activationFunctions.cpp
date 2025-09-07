#include "activationFunctions.h"

double math_activate::sigmoid(double x){
    return 1/(1+exp(-x));
}

double math_activate::linear(double x){
    return x;
}

double math_activate::get_derivative(ActivationFunc func, double x){
    return (func(x+Delta)-func(x))/Delta;
}

double math_activate::reLu(double x){
    if (x > 0){
        return x;
    }
    else{
        return 0;
    }
}

double math_activate::leakyReLu(double x){
    if(x > 0.1 * x){
        return x;
    }
    else{
        return 0.1 * x;
    }
}

double math_activate::tanhHyp(double x){
    return tanh(x);
}
