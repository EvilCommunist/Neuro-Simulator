#include "individual.h"
#include <random>
#include "../../neuro.h"

Individual::Individual(size_t w, size_t h, size_t d, double val):
    width(w),
    height(h),
    depth(d)
{
    data = new ThreeDimVector(w, h, d, val);
}

Individual::~Individual(){
    delete data;
}

void Individual::generateWeights(){
    std::random_device rd;
    std::mt19937 gen(rd());
    double stddev = sqrt(1.0 / width);
    std::normal_distribution<double> dis(-3*stddev, 3*stddev);

    for(size_t i = 0; i < width; i++){
        for(size_t j = 0; j < width; j++){
            for(uint16_t k = 0; k < depth; k++){
                weights.setValue(i, j, k, dis(gen));
            }
        }
    }
}

void Individual::calculateFitness(Neuro *network,  const QVector<double>& data){
    network->setWeights(*this->data);
    network->forwardPropogation(data);
}
