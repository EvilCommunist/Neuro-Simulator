#ifndef MODIFIEDGENETICALGORITHM_H
#define MODIFIEDGENETICALGORITHM_H

#include "./geneticalgorithm.h"
#include "./GeneticOperatorsEnum.h"
#include "./modifiedindividual.h"

class ModifiedGeneticAlgorithm: public GeneticAlgorithm
{
private:
    Crossover crossoverType;
    MutationStrength mutationStrength;
    Selection selectionType;
    uint8_t threadAmount;

    QVector<ModifiedIndividual*> tournamentWithNoReturn();
    QVector<ModifiedIndividual*> proportionalSelection();
    QVector<ModifiedIndividual*> rangedSelection();

public:
    ModifiedGeneticAlgorithm(size_t popSize = 500, float pMute = 0.3, float pCross = 0.7,
                            Crossover crossType = ARITHMETICAL, MutationStrength mutStrength = WEAK,
                            Selection selType = PROPORTIONAL, uint8_t workers = 4);

    void initializePopulation(size_t w, size_t h, size_t d, double val = 0) override;
    void startIteration() override;
    void completeIteration() override;
};

#endif // MODIFIEDGENETICALGORITHM_H
