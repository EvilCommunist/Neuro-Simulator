#ifndef MODIFIEDINDIVIDUAL_H
#define MODIFIEDINDIVIDUAL_H

#include "./individual.h"
#include "./GeneticOperatorsEnum.h"

class ModifiedIndividual: public Individual
{
private:
    Crossover crossType;
    MutationStrength mutRateForEach;
public:
    ModifiedIndividual(size_t w, size_t h, size_t d, double val = 0, Crossover type = ARITHMETICAL, MutationStrength strength = WEAK);

    void mutate() override;
    ModifiedIndividual operator+(ModifiedIndividual& other) override; // Crossover operation
};

#endif // MODIFIEDINDIVIDUAL_H
