#include "neuro.h"
#include <random>

size_t Neuro::qvectorMax(QVector<size_t> data){
    size_t max = data[0];
    for(const auto &element : data){
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

void Neuro::forwardPropogation(QVector<double> data, math_activate::ActivationFunc func){
    size_t index = 0;
    for (const auto &element : data){
        neurons.setValue(index, NeuroSignalIndex, 0, element);
        neurons.setValue(index, NeuroActivateIndex, 0, func(element));
        index++;
    }

    for(size_t l = 1; l < layers; l++){
        for(size_t n = 0; n < neuronAmountPerLayer[l]; n++){
            double input = 0;
            for(size_t prev = 0; prev < neuronAmountPerLayer[l-1]; prev++)
                input += neurons.getValue(prev, NeuroActivateIndex, l-1) * weights.getValue(prev, n, l-1);
            neurons.setValue(n, NeuroSignalIndex, l, input);
            neurons.setValue(n, NeuroActivateIndex, l, func(input));
        }
    }
}

QVector<double> Neuro::getRes(){
    QVector<double> result{};
    for(size_t i = 0; i < neuronAmountPerLayer[layers-1]; i++){
        result.append(neurons.getValue(i, NeuroActivateIndex, layers - 1));
    }
    return result;
}

void Neuro::learn_backPropogation(QVector<double> data, QVector<double> ans, double learnSpeed, size_t epochs){
    //TODO: write function
}

