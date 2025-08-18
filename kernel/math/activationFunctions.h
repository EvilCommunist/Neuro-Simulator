#ifndef ACTIVATIONFUNCTIONS_H
#define ACTIVATIONFUNCTIONS_H
#include <cmath>

namespace math_activate{
/*-- Activation functions for neural network --*/
typedef double(*ActivationFunc)(double);
const float Delta = 0.001;

double sigmoid(double x);
double linear(double x);

double get_derivative(ActivationFunc func, double x);
/*-- Activation functions for neural network --*/
}

#endif // ACTIVATIONFUNCTIONS_H
