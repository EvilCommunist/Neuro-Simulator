#include "actiovationFunctions.h"

double math_activate::sigmoid(double x){
    return 1/(1+exp(-x));
}

double math_activate::linear(double x){
    return x;
}

double math_activate::get_derivative(ActivationFunc func, double x){
    return (func(x+Delta)-func(x))/Delta;
}
