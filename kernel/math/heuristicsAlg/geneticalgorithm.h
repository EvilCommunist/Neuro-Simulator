#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "individual.h"

class GeneticAlgorithm
{
private:
    size_t populationSize;
    float pMutation;
    float pCrossover;
    QVector<Individual> best, currentGeneration;

public:
    GeneticAlgorithm(size_t popSize = 500, float pMute = 0.3, float pCross = 0.7);
};

#endif // GENETICALGORITHM_H
