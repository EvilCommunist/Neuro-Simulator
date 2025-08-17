#include "neuro.h"
#include <random>

size_t Neuro::qvectorMax(QVector<size_t> data){
    size_t max = data[0];
    foreach(auto element, data){
        if(max < element) max = element;
    }
    return max;
}

void Neuro::initWeights(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1, 1);

    for(size_t i = 0; i < qvectorMax(neuronAmountPerLayer); i++){
        for(size_t j = 0; j < qvectorMax(neuronAmountPerLayer); j++){
            for(uint16_t k = 0; k < layers - 1; k++){
                weights.setValue(i, j, k, dis(gen));
            }
        }
    }
}

Neuro::Neuro(uint16_t l, const QVector<size_t>& nAPL):
    layers(l),
    neuronAmountPerLayer(nAPL),
    neurons(ThreeDimVector<double>(qvectorMax(nAPL),NeuroDataType,l,0)),
    weights(ThreeDimVector<double>(qvectorMax(nAPL),qvectorMax(nAPL),l-1,0))
{ initWeights(); }

void Neuro::forwardPropogation(size_t size, QVector<double> data){
// TODO: write the function
}

