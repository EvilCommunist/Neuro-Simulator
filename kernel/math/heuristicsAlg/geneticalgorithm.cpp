#include "geneticalgorithm.h"
#include <qrandom.h>

GeneticAlgorithm::GeneticAlgorithm(size_t popSize, float pMute, float pCross):
    populationSize(popSize),
    pMutation(pMute),
    pCrossover(pCross),
    offspring({}), currentGeneration({}), best({})
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
    size_t contestantsAmount = std::min(3, static_cast<int>(currentGeneration.size()));
    QVector<int> usedIndexes{};
    QVector<Individual> chosen{};
    for (size_t i = 0; i < contestantsAmount; i++){
        int index = QRandomGenerator::global()->bounded(currentGeneration.size());
        while(usedIndexes.contains(index))
            index = QRandomGenerator::global()->bounded(currentGeneration.size());
        chosen.append(currentGeneration[index]);
        usedIndexes.append(index);
    }
    return chosen;
}

QVector<Individual> GeneticAlgorithm::tournament(){
    QVector<Individual> selected{};
    for(size_t i = 0; i < currentGeneration.size(); i++){
        QVector<Individual> contestants = randomChoose();
        selected.append(findBest(contestants));
    }
    return selected;
}

void GeneticAlgorithm::initializePopulation(size_t w, size_t h, size_t d, double val){
    for(int i = 0; i < populationSize; i++){
        Individual newIndividual(w, h, d, val);
        newIndividual.generateWeights();
        currentGeneration.append(newIndividual);
    }
}

void GeneticAlgorithm::startIteration(){    // fitness is calculated in NN learn function
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dis(0, 1);

    auto selected = tournament();

    if(!offspring.empty())
        offspring.clear();

    for(size_t i = 0; i < currentGeneration.size() - 1; i+=2){
        if(dis(gen) < pCrossover){
            Individual child1 = selected[i]+selected[i+1], child2 = selected[i+1]+selected[i];
            offspring.append(child1); offspring.append(child2);
        }else{
            offspring.append(selected[i]); offspring.append(selected[i+1]);
        }
    }

    for(size_t i = 0; i < offspring.size(); i++){
        if(dis(gen) < pMutation)
            offspring[i].mutate();
    }
}

void GeneticAlgorithm::completeIteration(){
    auto common = currentGeneration + offspring;
    for(size_t i = 0; i < common.size(); i++){
        for(size_t j = 1; j < common.size()-i; j++){
            if(common[j-1].getFitness() > common[j].getFitness()){
                auto temp = common[j-1];
                common[j-1] = common[j];
                common[j] = temp;
            }
        }
    }

    offspring.clear();
    for(int i = 0; i < currentGeneration.size(); i++){
        currentGeneration[i] = common[i];
    }

    best.append(findBest(currentGeneration));
}
