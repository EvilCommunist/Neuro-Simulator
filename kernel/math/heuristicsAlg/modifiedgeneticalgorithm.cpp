#include "modifiedgeneticalgorithm.h"

ModifiedGeneticAlgorithm::ModifiedGeneticAlgorithm(size_t popSize = 500, float pMute = 0.3, float pCross = 0.7,
                                                   Crossover crossType = ARITHMETICAL, MutationStrength mutStrength = WEAK,
                                                   Selection selType = PROPORTIONAL, uint8_t workers):
    GeneticAlgorithm(popSize, pMute, pCross),
    crossoverType(crossType),
    mutationStrength(mutStrength),
    selectionType(selType),
    threadAmount(workers) {}
