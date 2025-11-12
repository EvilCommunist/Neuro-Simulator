#include "neuro.h"
#include "../ui/chartprocessor.h"
#include <random>
#include "./math/heuristicsAlg/geneticalgorithm.h"

#include <QDebug> // for debug

float Neuro::learnChartHelper(){ // for gathering info about AI learning
    size_t counter = 0;
    float sumErr = 0;
    for(size_t n = 0; n < neuronAmountPerLayer[layers-1]; n++){
        sumErr += abs(neurons.getValue(n, NeuroErrorIndex, layers-1));
        counter++;
    }
    return sumErr/counter;
}

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

void Neuro::backPropogation(const QVector<double> &ans){
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
}

QVector<double> Neuro::getRes(){
    QVector<double> result{};
    for(size_t i = 0; i < neuronAmountPerLayer[layers-1]; i++){
        result.append(neurons.getValue(i, NeuroActivateIndex, layers - 1));
    }
    return result;
}

void Neuro::learn_backPropogation(const TwoDimVector<double>& data, const TwoDimVector<double>& ans, double learnSpeed, size_t epochs = 1000){
    for(size_t e = 0; e < epochs; e++){
        float learnAvgErr = 0;  // for chart
        for(size_t selection = 0; selection < data.getHeight(); selection ++){
            auto dataLine = data.getLine(selection);
            auto ansLine = ans.getLine(selection);
            forwardPropogation(dataLine);
            backPropogation(ansLine);

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
            learnAvgErr+=learnChartHelper();    // for chart
        }
        chartProcessor::getCurrentError(learnAvgErr/data.getHeight());    // for chart
    }
}


void Neuro::learn_resilentPropogation(const TwoDimVector<double>& data, const TwoDimVector<double>& ans, size_t epochs){
    // RPROP parameters, might be initialized as values calculated based on selections
    const double Delta0 = 0.01;
    const double DeltaMin = 0.0000001;
    const double DeltaMax = 50;
    const double EtaMinus = 0.5;
    const double EtaPlus = 1.2;
    const double EpsStop = 0.01;

    ThreeDimVector<double> deltas(qvectorMax(neuronAmountPerLayer), qvectorMax(neuronAmountPerLayer), layers-1, Delta0);
    ThreeDimVector<double> prevGrads(qvectorMax(neuronAmountPerLayer), qvectorMax(neuronAmountPerLayer), layers-1, 0);

    for(size_t e = 0; e < epochs; e++) {
        double totalGradNorm = 0;
        float learnAvgErr = 0;  // for chart
        for(size_t selection = 0; selection < data.getHeight(); selection++) {
            auto dataLine = data.getLine(selection);
            auto ansLine = ans.getLine(selection);
            forwardPropogation(dataLine);
            backPropogation(ansLine);
            for(uint16_t l = 0; l < layers-1; l++) {
                for(size_t n = 0; n < neuronAmountPerLayer[l]; n++) {
                    for(size_t next = 0; next < neuronAmountPerLayer[l+1]; next++) {
                        double grad = neurons.getValue(n, NeuroActivateIndex, l) *
                                      neurons.getValue(next, NeuroErrorIndex, l+1);
                        double prevGrad = prevGrads.getValue(n, next, l);
                        double delta = deltas.getValue(n, next, l);

                        if(grad * prevGrad > 0) {
                            delta = std::min(delta * EtaPlus, DeltaMax);
                        } else if(grad * prevGrad < 0) {
                            delta = std::max(delta * EtaMinus, DeltaMin);
                            grad = 0;
                        }

                        deltas.setValue(n, next, l,  delta);
                        prevGrads.setValue(n, next, l, grad);

                        if(grad == 0){
                            continue;
                        }
                        double weightChange = -std::copysign(delta, grad);
                        weights.setValue(n, next, l,
                                        weights.getValue(n, next, l) + weightChange);

                        totalGradNorm += grad * grad;
                    }
                }
            }
            learnAvgErr+=learnChartHelper();    // for chart
        }
        chartProcessor::getCurrentError(learnAvgErr/data.getHeight());  // for chart
        if(sqrt(totalGradNorm) < EpsStop) {
            break;
        }
    }
}


void Neuro::learn_geneticAlgorithm(const TwoDimVector<double> &data, const TwoDimVector<double> &ans, size_t epochs,
                                   size_t popSize, float pMute, float pCross){
    GeneticAlgorithm GAHelper(popSize, pMute, pCross);
// Initial iteration
    GAHelper.initializePopulation(qvectorMax(neuronAmountPerLayer), qvectorMax(neuronAmountPerLayer), layers-1);

    auto currentGen = GAHelper.getCurrent();
    for (int i = 0; i < currentGen.size(); i++){
        this->weights = currentGen[i]->getData();
        float learnAvgErr = 0;
        for(int j = 0; j < data.getHeight(); j++){
            forwardPropogation(data.getLine(j));
            for(size_t n = 0; n < neuronAmountPerLayer[layers-1]; n++){
                auto currentNeuron = neurons.getValue(n, NeuroActivateIndex, layers-1);
                neurons.setValue(n, NeuroErrorIndex, layers-1, ((ans.getLine(j)[n]-currentNeuron)*math_activate::get_derivative(activationFuncForLayer[layers-1], currentNeuron)));
            }
            learnAvgErr += learnChartHelper();
        }
        currentGen[i]->setFitness(learnAvgErr/data.getHeight());
    }
    GAHelper.findInitialBest();
    chartProcessor::getCurrentError(GAHelper.getBest()[0]->getFitness());
// Initial iteration

    for(int epoch = 1; epoch < epochs; epoch ++){
        GAHelper.startIteration();
        auto currentGen = GAHelper.getCurrent() + GAHelper.getOffspring();
        for (int i = 0; i < currentGen.size(); i++){
            this->weights = currentGen[i]->getData();
            float learnAvgErr = 0;
            for(int j = 0; j < data.getHeight(); j++){
                forwardPropogation(data.getLine(j));
                for(size_t n = 0; n < neuronAmountPerLayer[layers-1]; n++){
                    auto currentNeuron = neurons.getValue(n, NeuroActivateIndex, layers-1);
                    neurons.setValue(n, NeuroErrorIndex, layers-1, ((ans.getLine(j)[n]-currentNeuron)*math_activate::get_derivative(activationFuncForLayer[layers-1], currentNeuron)));
                }
                learnAvgErr += learnChartHelper();
            }
            currentGen[i]->setFitness(learnAvgErr/data.getHeight());
        }
        GAHelper.completeIteration();
        chartProcessor::getCurrentError(GAHelper.getBestOfTheBest()->getFitness());
    }
    this->weights = ThreeDimVector(GAHelper.getBestOfTheBest()->getData());
}

