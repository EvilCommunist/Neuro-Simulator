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

QVector<Individual*> ModifiedGeneticAlgorithm::tournament(){
    QVector<Individual*> selected{};
    QList<QFuture<QVector<Individual*>>> futures;
    for(int i = 0; i < threadAmount; i ++){
        auto generatorFuture = QtConcurrent::run(QThreadPool::globalInstance(), [&]() {
            QVector<Individual*> part;
            for(size_t i = 0; i < currentGeneration.size(); i++){
                QVector<Individual*> contestants = randomChoose();
                part.append(findBest(contestants));
            }
            return part;
        });
        futures.append(generatorFuture);
    }

    for(auto& future : futures){
        future.waitForFinished();
        selected.append(future.result());
    }

    return selected;
}

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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dis(0, 1);

    QVector<Individual*> selected{};

    switch(selectionType){
    case TOURNAMENT:{
        selected = tournament();
        break;
    }
    case PROPORTIONAL:{
        break;
    }
    case RANGED:{
        break;
    }
    }

    tournament();

    if(!offspring.empty()){
        for(auto ind : offspring){
            delete ind;
        }
        offspring.clear();
    }

    QList<QFuture<QVector<Individual*>>> futures;
    for(int i = 0; i < threadAmount; i ++){
        auto generatorFuture = QtConcurrent::run(QThreadPool::globalInstance(), [&]() {
            QVector<Individual*> part;
            for(size_t i = 0; i < currentGeneration.size()/threadAmount; i+=2){
                ModifiedIndividual* current1 = dynamic_cast<ModifiedIndividual*>(selected[i]);
                ModifiedIndividual* current2 = dynamic_cast<ModifiedIndividual*>(selected[i+1]);
                if(dis(gen) < pCrossover){
                    ModifiedIndividual* child1 = new ModifiedIndividual(*current1+*current2);
                    ModifiedIndividual* child2 = new ModifiedIndividual(*current2+*current1);
                    part.append(child1); part.append(child2);
                }else{
                    ModifiedIndividual* copy1 = new ModifiedIndividual(*current1);
                    ModifiedIndividual* copy2 = new ModifiedIndividual(*current2);
                    part.append(copy1); part.append(copy2);
                }
            }
            return part;
        });
        futures.append(generatorFuture);
    }

    for(auto& future : futures){
        future.waitForFinished();
        offspring.append(future.result());
    }

    QList<QFuture<void>> futuresMutation;
    for(int i = 0; i < threadAmount; i ++){
        auto generatorFuture = QtConcurrent::run(QThreadPool::globalInstance(), [&]() {
            for(size_t i = 0; i < offspring.size()/threadAmount; i++){
                if(dis(gen) < pMutation)
                    offspring[i]->mutate();
            }
        });
        futuresMutation.append(generatorFuture);
    }

    for(auto& future : futuresMutation){
        future.waitForFinished();
    }
}

void ModifiedGeneticAlgorithm::completeIteration(){

}
