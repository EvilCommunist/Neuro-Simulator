#include "neuro.h"
#include <random>

size_t Neuro::qvectorMax(const QVector<size_t>& data){
    size_t max = data[0];
    for(const auto &element : data){
        if(max < element) max = element;
    }
    return max;
}

void Neuro::initWeights(){  // Xavier initializattion
    std::random_device rd;
    std::mt19937 gen(rd());
    double stddev = sqrt(1.0 / qvectorMax(neuronAmountPerLayer));
    std::normal_distribution<double> dis(0.0, stddev);

    for(size_t i = 0; i < qvectorMax(neuronAmountPerLayer); i++){
        for(size_t j = 0; j < qvectorMax(neuronAmountPerLayer); j++){
            for(uint16_t k = 0; k < layers - 1; k++){
                weights.setValue(i, j, k, dis(gen));
            }
        }
    }
}

Neuro::Neuro(uint16_t l, const QVector<size_t>& nAPL, const QVector<math_activate::ActivationFunc>& aFfL):
    layers(l),
    neuronAmountPerLayer(nAPL),
    activationFuncForLayer(aFfL),
    neurons(ThreeDimVector<double>(qvectorMax(nAPL)+1,NeuroDataType,l,0)),
    weights(ThreeDimVector<double>(qvectorMax(nAPL)+1,qvectorMax(nAPL)+1,l-1,0))
{

    for (int i = 1; i < nAPL.size()-1; i++)  // adding bias neurons
        neuronAmountPerLayer[i]++;
    initWeights();
}

void Neuro::forwardPropogation(const QVector<double>& data){
    size_t index = 0;
    for (const auto &element : data){
        neurons.setValue(index, NeuroSignalIndex, 0, element);
        neurons.setValue(index, NeuroActivateIndex, 0, activationFuncForLayer[0](element));
        index++;
    }

    for(uint16_t l = 1; l < layers; l++){
        for(size_t n = 0; n < neuronAmountPerLayer[l]; n++){
            double input = 0;
            for(size_t prev = 0; prev < neuronAmountPerLayer[l-1]; prev++)
                input += neurons.getValue(prev, NeuroActivateIndex, l-1) * weights.getValue(prev, n, l-1);
            neurons.setValue(n, NeuroSignalIndex, l, input);
            neurons.setValue(n, NeuroActivateIndex, l, activationFuncForLayer[l](input));
        }
        if (l < layers -1)
            neurons.setValue(neuronAmountPerLayer[l]-1, NeuroActivateIndex, l, 1);
    }
}

QVector<double> Neuro::getRes(){
    QVector<double> result{};
    for(size_t i = 0; i < neuronAmountPerLayer[layers-1]; i++){
        result.append(neurons.getValue(i, NeuroActivateIndex, layers - 1));
    }
    return result;
}

void Neuro::learn_backPropogation(const QVector<double>& data, const QVector<double>& ans, double learnSpeed){
    forwardPropogation(data);
    for(size_t n = 0; n < neuronAmountPerLayer[layers-1]; n++){
        auto currentNeuron = neurons.getValue(n, NeuroActivateIndex, layers-1);
        neurons.setValue(n, NeuroErrorIndex, layers-1, ((ans[n]-currentNeuron)*math_activate::get_derivative(activationFuncForLayer[layers-1], currentNeuron)));
    }
    for(long l = layers-2; l >= 0; l--){
        for(size_t n = 0; n < neuronAmountPerLayer[l]-1; n++){
            neurons.setValue(n, NeuroErrorIndex, l, 0);
            for(size_t prev = 0; prev < neuronAmountPerLayer[l+1]; prev++){
                neurons.setValue(n, NeuroErrorIndex, l, neurons.getValue(n, NeuroErrorIndex, l) + (neurons.getValue(prev, NeuroErrorIndex, l+1)
                                 * weights.getValue(n, prev, l))
                                );
            }
            neurons.setValue(n, NeuroErrorIndex, l, neurons.getValue(n, NeuroErrorIndex, l)
                             * math_activate::get_derivative(activationFuncForLayer[l], neurons.getValue(n, NeuroActivateIndex, l))
                            );
        }
    }

    for(uint16_t l = layers-2; l > 0; l--){
        for(size_t n = 0; n < neuronAmountPerLayer[l]; n++){
            for(size_t prev = 0; prev < neuronAmountPerLayer[l+1]; prev++){
                weights.setValue(n, prev, l, weights.getValue(n, prev, l)
                                                 + neurons.getValue(n, NeuroActivateIndex, l)
                                                       * neurons.getValue(prev, NeuroErrorIndex, l+1) * learnSpeed
                                 );
            }
        }
    }
}

