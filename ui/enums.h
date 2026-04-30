#ifndef ENUMS_H
#define ENUMS_H

enum functionCoeffs{
    BACK_PROPOGATION,
    RESILENT_PROPOGATION,
    GENETIC_ALGORITHM,
    MODIFIED_GA
};

enum activationFunc{
    SIGMOID,
    LINEAR_F,
    RELU,
    LEAKY_RELU,
    TANH_HYP
};

enum weightInit{
    XAVIER,
    RAND,
    CONST
};


#endif // ENUMS_H
