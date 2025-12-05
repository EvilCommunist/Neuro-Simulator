#ifndef MODIFIEDINDIVIDUAL_H
#define MODIFIEDINDIVIDUAL_H

#include "./individual.h"
#include "./GeneticOperatorsEnum.h"

class ModifiedIndividual: public Individual
{
public:
    ModifiedIndividual(size_t w, size_t h, size_t d, double val = 0, Crossover type = ARITHMETICAL);
};

#endif // MODIFIEDINDIVIDUAL_H
