#include "modifiedgeneticalgorithm.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

ModifiedGeneticAlgorithm::ModifiedGeneticAlgorithm(size_t popSize, float pMute, float pCross,
                                                   Crossover crossType, MutationStrength mutStrength,
                                                   Selection selType, uint8_t workers):
    GeneticAlgorithm(popSize, pMute, pCross),
    crossoverType(crossType),
    mutationStrength(mutStrength),
    selectionType(selType),
    threadAmount(workers) {}

void ModifiedGeneticAlgorithm::initializePopulation(size_t w, size_t h, size_t d, double val){
    QList<QFuture<QVector<Individual*>>> futures;
    for(int i = 0; i < threadAmount; i ++){
        auto generatorFuture = QtConcurrent::run(QThreadPool::globalInstance(), [&]() {
            QVector<Individual*> part;
            for(int i = 0; i < populationSize/threadAmount; i++){
                ModifiedIndividual* newIndividual = new ModifiedIndividual(w, h, d, val, crossoverType, mutationStrength);
                newIndividual->generateWeights();
                part.append(newIndividual);
            }
            return part;
        });
        futures.append(generatorFuture);
    }

    for(auto& future : futures){
        future.waitForFinished();
        currentGeneration.append(future.result());
    }
}

void ModifiedGeneticAlgorithm::startIteration(){
    // изменить функцию
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dis(0, 1);

    auto selected = tournament();

    if(!offspring.empty()){
        for(auto ind : offspring){
            delete ind;
        }
        offspring.clear();
    }

    for(size_t i = 0; i < currentGeneration.size() - 1; i+=2){
        if(dis(gen) < pCrossover){
            Individual* child1 = new Individual(*selected[i]+*selected[i+1]); Individual* child2 = new Individual(*selected[i+1]+*selected[i]);
            offspring.append(child1); offspring.append(child2);
        }else{
            Individual* copy1 = new Individual(*selected[i]);
            Individual* copy2 = new Individual(*selected[i+1]);
            offspring.append(copy1); offspring.append(copy2);
        }
    }

    for(size_t i = 0; i < offspring.size(); i++){
        if(dis(gen) < pMutation)
            offspring[i]->mutate();
    }
}

void ModifiedGeneticAlgorithm::completeIteration(){

}
