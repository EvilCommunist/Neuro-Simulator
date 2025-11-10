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
    QVector<Individual> tournament(size_t size);
    Individual findBest(const QVector<Individual>& sample);
    QVector<Individual> randomChoose();

public:
    GeneticAlgorithm(size_t popSize = 500, float pMute = 0.3, float pCross = 0.7);
    inline QVector<Individual> getBest(){return best;}
    inline QVector<Individual> getCurrent(){return currentGeneration;}
    void startIteration();
};

#endif // GENETICALGORITHM_H
