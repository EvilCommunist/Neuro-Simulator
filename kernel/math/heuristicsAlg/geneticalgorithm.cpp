#include "geneticalgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(size_t popSize = 500, float pMute = 0.3, float pCross = 0.7):
    populationSize(popSize),
    pMutation(pMute),
    pCrossover(pCross)
{}
