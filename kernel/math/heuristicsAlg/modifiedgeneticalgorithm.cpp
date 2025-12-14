#include "modifiedgeneticalgorithm.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

double sum(const QVector<double>& vector){
    double sum = 0;
    for(auto elem : vector){
        sum += elem;
    }
    return sum;
}

struct ProportionalSelectionHelper{
    size_t index;
    double prob;
    QPair<double, double> interval;

    bool match(double P){
        return P >= interval.first && P <= interval.second;
    }

    static double calculateProbability(double index, const QVector<double>& vector){
        return 1 - vector[index]/sum(vector);
    }
};

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
            QVector<Individual*> part{};
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

QVector<Individual*> ModifiedGeneticAlgorithm::proportionalSelection(){
    // preprocessing for proportional selestion
    QVector<double> fitnesses{};
    QList<QFuture<QVector<double>>> futuresFitness;
    for(int i = 0; i < threadAmount; i ++){
        auto generatorFuture = QtConcurrent::run(QThreadPool::globalInstance(), [&]() {
            QVector<double> part{};
            for(size_t j = i*currentGeneration.size()/threadAmount; j < (i+1)*currentGeneration.size()/threadAmount; j++){
                part.append(currentGeneration[j]->getFitness());
            }
            return part;
        });
        futuresFitness.append(generatorFuture);
    }

    for(auto& future : futuresFitness){
        future.waitForFinished();
        fitnesses.append(future.result());
    }

    QVector<ProportionalSelectionHelper> helper{};

    QList<QFuture<QVector<ProportionalSelectionHelper>>> futures;
    for(int i = 0; i < threadAmount; i ++){
        auto generatorFuture = QtConcurrent::run(QThreadPool::globalInstance(), [&]() {
            QVector<ProportionalSelectionHelper> part{};
            for(size_t j = i*currentGeneration.size()/threadAmount; j < (i+1)*currentGeneration.size()/threadAmount; j++){
                part.append(ProportionalSelectionHelper{j, ProportionalSelectionHelper::calculateProbability(j, fitnesses), {}});
            }
            return part;
        });
        futures.append(generatorFuture);
    }

    for(auto& future : futures){
        future.waitForFinished();
        helper.append(future.result());
    }

    helper[0].interval.first = 0;
    helper[0].interval.second = helper[0].prob;
    for(int i = 1; i < helper.size(); i++){
        helper[i].interval.first = helper[i-1].interval.second;
        helper[i].interval.second = helper[i-1].interval.second + helper[i].prob;
    }
    helper[helper.size()-1].interval.second = 1;


    // selection itself
    QVector<Individual*> selected{};
    QList<QFuture<QVector<Individual*>>> futuresRes;
    for(int i = 0; i < threadAmount; i ++){
        auto generatorFuture = QtConcurrent::run(QThreadPool::globalInstance(), [&]() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::normal_distribution<double> dis(0, 1);
            QVector<Individual*> part;
            for(size_t i = 0; i < currentGeneration.size()/threadAmount; i++){
                auto prob = dis(gen);
                for(auto item: helper){
                    if(item.match(prob)){
                        part.append(currentGeneration[item.index]);
                        break;
                    }
                }
            }
            return part;
        });
        futuresRes.append(generatorFuture);
    }

    for(auto& future : futuresRes){
        future.waitForFinished();
        selected.append(future.result());
    }

    return selected;
}

void ModifiedGeneticAlgorithm::initializePopulation(size_t w, size_t h, size_t d, double val){
    QList<QFuture<QVector<Individual*>>> futures;
    for(int i = 0; i < threadAmount; i ++){
        auto generatorFuture = QtConcurrent::run(QThreadPool::globalInstance(), [&]() {
            QVector<Individual*> part{};
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
        selected = proportionalSelection();
        break;
    }
    case RANGED:{   // TODO
        break;
    }
    }

    if(!offspring.empty()){
        for(auto ind : offspring){
            delete ind;
        }
        offspring.clear();
    }

    QList<QFuture<QVector<Individual*>>> futures;
    for(int i = 0; i < threadAmount; i ++){
        auto generatorFuture = QtConcurrent::run(QThreadPool::globalInstance(), [&]() {
            QVector<Individual*> part{};
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
