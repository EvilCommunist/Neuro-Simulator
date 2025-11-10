#include "geneticalgorithm.h"
#include <qrandom.h>

GeneticAlgorithm::GeneticAlgorithm(size_t popSize, float pMute, float pCross):
    populationSize(popSize),
    pMutation(pMute),
    pCrossover(pCross)
{}

Individual GeneticAlgorithm::findBest(const QVector<Individual>& sample){
    Individual best(1, 1, 1, 0);
    best.setFitness(INT32_MAX);
    for(auto individual: sample){
        if(best.getFitness() > individual.getFitness()){
            best = individual;
        }
    }
    return best;
}

QVector<Individual> GeneticAlgorithm::randomChoose(){
    size_t contestantsAmount = std::min(3, currentGeneration.size());
    QVector<int> usedIndexes{};
    QVector<Individual> chosen{};
    for (int i = 0; i < contestantsAmount; i++){
        int index = QRandomGenerator::global()->bounded(currentGeneration.size());
        while(usedIndexes.contains(index))
            index = QRandomGenerator::global()->bounded(currentGeneration.size());
        chosen.append(currentGeneration[index]);
        usedIndexes.append(index);
    }
    return chosen;
}

QVector<Individual> GeneticAlgorithm::tournament(size_t size){
    QVector<Individual> selected{};
    for(int i = 0; i < size; i++){
        QVector<Individual> contestants = randomChoose();
        selected.append(findBest(contestants));
    }
    return selected;
}

void GeneticAlgorithm::startIteration(){

}
