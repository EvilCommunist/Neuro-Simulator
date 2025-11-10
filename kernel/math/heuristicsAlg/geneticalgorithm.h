#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "individual.h"

class GeneticAlgorithm  // this class partly makes GA's functions, fitness will be calculated in the NN's learn function
{
private:
    size_t populationSize;
    float pMutation;
    float pCrossover;
    QVector<Individual> best, currentGeneration, offspring;
    QVector<Individual> tournament();
    Individual findBest(const QVector<Individual>& sample);
    QVector<Individual> randomChoose();

public:
    GeneticAlgorithm(size_t popSize = 500, float pMute = 0.3, float pCross = 0.7);
    inline QVector<Individual>& getBest(){return best;}
    inline QVector<Individual>& getCurrent(){return currentGeneration;}
    inline QVector<Individual>& getOffspring(){return offspring;}

    void initializePopulation(size_t w, size_t h, size_t d, double val = 0);
    void startIteration();
    void completeIteration();
};

#endif // GENETICALGORITHM_H
