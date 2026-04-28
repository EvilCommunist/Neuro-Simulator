#include "neuro.h"
#include "../ui/chartprocessor.h"
#include <random>
#include "./math/heuristicsAlg/geneticalgorithm.h"
#include "./math/heuristicsAlg/modifiedgeneticalgorithm.h"

#include <QDebug> // for debug

#define SIGMOID "sigmoid"
#define LINEAR "linear"
#define RELU "reLu"
#define LEAKYRELU "leakyReLu"
#define TANHHYP "tanhHyp"

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

void Neuro::initWeights(int initType, float constant){
    switch(initType){
    case XAVIER:{
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
        break;
    }
    case RAND:{
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(-1, 1);

        for(size_t i = 0; i < qvectorMax(neuronAmountPerLayer); i++){
            for(size_t j = 0; j < qvectorMax(neuronAmountPerLayer); j++){
                for(uint16_t k = 0; k < layers - 1; k++){
                    weights.setValue(i, j, k, dis(gen));
                }
            }
        }
        break;
    }
    case CONST:{
        for(size_t i = 0; i < qvectorMax(neuronAmountPerLayer); i++){
            for(size_t j = 0; j < qvectorMax(neuronAmountPerLayer); j++){
                for(uint16_t k = 0; k < layers - 1; k++){
                    weights.setValue(i, j, k, constant);
                }
            }
        }
        break;
    }
    }
}

Neuro::Neuro(uint16_t l, const QVector<size_t>& nAPL, const QVector<math_activate::ActivationFunc>& aFfL, int initType):
    layers(l),
    neuronAmountPerLayer(nAPL),
    activationFuncForLayer(aFfL),
    neurons(ThreeDimVector<double>(qvectorMax(nAPL)+1,NeuroDataType,l,0)),
    weights(ThreeDimVector<double>(qvectorMax(nAPL)+1,qvectorMax(nAPL)+1,l-1,0))
{
    for (int i = 1; i < nAPL.size()-1; i++)  // adding bias neurons
        neuronAmountPerLayer[i]++;
    initWeights(initType);
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


void Neuro::learn_resilientPropogation(const TwoDimVector<double>& data, const TwoDimVector<double>& ans, size_t epochs){
    // RPROP default parameters
    const double eta_plus  = 1.2;
    const double eta_minus = 0.5;
    const double delta_max = 50.0;
    const double delta_min = 0.000001;
    const double delta0    = 0.1;

    const size_t weightLayers = layers - 1;

    QVector<QVector<QVector<double>>> prevGrad(weightLayers);
    QVector<QVector<QVector<double>>> delta(weightLayers);

    for (size_t l = 0; l < weightLayers; ++l) {
        size_t rows = neuronAmountPerLayer[l];
        size_t cols = neuronAmountPerLayer[l + 1];
        prevGrad[l].resize(rows, QVector<double>(cols, 0.0));
        delta[l].resize(rows, QVector<double>(cols, delta0));
    }

    for (size_t epoch = 0; epoch < epochs; ++epoch) {
        double avgError = 0.0;
        for (size_t sample = 0; sample < data.getHeight(); ++sample) {
            forwardPropogation(data.getLine(sample));
            backPropogation(ans.getLine(sample));

            for (int l = static_cast<int>(weightLayers) - 1; l >= 0; --l) {
                size_t nCount = neuronAmountPerLayer[l];
                size_t pCount = neuronAmountPerLayer[l + 1];

                for (size_t n = 0; n < nCount; ++n) {
                    for (size_t prev = 0; prev < pCount; ++prev) {
                        double gradient = neurons.getValue(n, NeuroActivateIndex, l) *
                                          neurons.getValue(prev, NeuroErrorIndex, l + 1);

                        double& d = delta[l][n][prev];
                        double  pg = prevGrad[l][n][prev];

                        if (pg * gradient > 0) {
                            d = std::min(d * eta_plus, delta_max);
                            double weightChange = (gradient > 0) ? d : -d;
                            double newWeight = weights.getValue(n, prev, l) + weightChange;
                            weights.setValue(n, prev, l, newWeight);
                            prevGrad[l][n][prev] = gradient;
                        }
                        else if (pg * gradient < 0) {
                            d = std::max(d * eta_minus, delta_min);
                            prevGrad[l][n][prev] = 0.0;
                        }
                        else {
                            if (gradient != 0.0) {
                                double weightChange = (gradient > 0) ? d : -d;
                                double newWeight = weights.getValue(n, prev, l) + weightChange;
                                weights.setValue(n, prev, l, newWeight);
                            }
                            prevGrad[l][n][prev] = gradient;
                        }
                    }
                }
            }
            avgError += learnChartHelper();   // for chart
        }
        chartProcessor::getCurrentError(avgError / data.getHeight());
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

void Neuro::learn_modifiedGeneticAlgorithm(const TwoDimVector<double> &data, const TwoDimVector<double> &ans, size_t epochs, size_t popSize, float pMute,
                                           float pCross, Crossover crossType, MutationStrength mutStrength, Selection selType, uint8_t workers){
    ModifiedGeneticAlgorithm GAHelper(popSize, pMute, pCross, crossType, mutStrength, selType, workers);
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
    //QElapsedTimer timer;
    for(int epoch = 1; epoch < epochs; epoch ++){
        GAHelper.startIteration();
        // timer.start();
        auto currentGen = GAHelper.getCurrent() + GAHelper.getOffspring();
        QVector<ModifiedGAThread*> threads{};
        for(int t = 0; t < workers; t++){
            ModifiedGAThread* thread = new ModifiedGAThread(neuronAmountPerLayer, activationFuncForLayer, layers, neurons, data, ans,
                                                            currentGen, t*currentGen.size()/workers, (t+1)*currentGen.size()/workers);
            threads.append(thread);
            thread->start();
        }
        QVector<double> fitnesses{};
        for(auto thread : threads){
            thread->QThread::wait();
            fitnesses.append(thread->getFitnesses());
        }
        for(int i = 0; i < currentGen.size(); i++){
            currentGen[i]->setFitness(fitnesses[i]);
        }
        // qDebug() << timer.elapsed();
        // timer.start();
        // auto currentGen = GAHelper.getCurrent() + GAHelper.getOffspring();
        // for (int i = 0; i < currentGen.size(); i++){
        //     this->weights = currentGen[i]->getData();
        //     float learnAvgErr = 0;
        //     for(int j = 0; j < data.getHeight(); j++){
        //         forwardPropogation(data.getLine(j));
        //         for(size_t n = 0; n < neuronAmountPerLayer[layers-1]; n++){
        //             auto currentNeuron = neurons.getValue(n, NeuroActivateIndex, layers-1);
        //             neurons.setValue(n, NeuroErrorIndex, layers-1, ((ans.getLine(j)[n]-currentNeuron)*math_activate::get_derivative(activationFuncForLayer[layers-1], currentNeuron)));
        //         }
        //         learnAvgErr += learnChartHelper();
        //     }
        //     currentGen[i]->setFitness(learnAvgErr/data.getHeight());
        // }
        // qDebug() << timer.elapsed();
        GAHelper.completeIteration();
        chartProcessor::getCurrentError(GAHelper.getBestOfTheBest()->getFitness());
    }
    this->weights = ThreeDimVector(GAHelper.getBestOfTheBest()->getData());
}

void Neuro::ModifiedGAThread::run(){
    // ERROR CALCULATION______________________________________________________________________________________
    for(int i = startIndex; i < lastIndex; i++){
        auto weights = examinatedIndividuals[i]->getData();
        float learnAvgErr = 0;
        for(int i = 0; i < data.getHeight(); i++){
            auto data = this->data.getLine(i);
            size_t index = 0;
            for (const auto &element : data){
                neurons.setValue(index, NeuroSignalIndex, 0, element);
                neurons.setValue(index, NeuroActivateIndex, 0, activationFuncForLayerLink[0](element));
                index++;
            }

            for(uint16_t l = 1; l < layers; l++){
                for(size_t n = 0; n < neuronAmountPerLayerLink[l]; n++){
                    double input = 0;
                    for(size_t prev = 0; prev < neuronAmountPerLayerLink[l-1]; prev++)
                        input += neurons.getValue(prev, NeuroActivateIndex, l-1) * weights.getValue(prev, n, l-1);
                    neurons.setValue(n, NeuroSignalIndex, l, input);
                    neurons.setValue(n, NeuroActivateIndex, l, activationFuncForLayerLink[l](input));
                }
                if (l < layers -1)
                    neurons.setValue(neuronAmountPerLayerLink[l]-1, NeuroActivateIndex, l, 1);
            }

            for(size_t n = 0; n < neuronAmountPerLayerLink[layers-1]; n++){
                auto currentNeuron = neurons.getValue(n, NeuroActivateIndex, layers-1);
                neurons.setValue(n, NeuroErrorIndex, layers-1, ((ans.getLine(i)[n]-currentNeuron)*math_activate::get_derivative(activationFuncForLayerLink[layers-1], currentNeuron)));
            }

            size_t counter = 0;
            float sumErr = 0;
            for(size_t n = 0; n < neuronAmountPerLayerLink[layers-1]; n++){
                sumErr += abs(neurons.getValue(n, NeuroErrorIndex, layers-1));
                counter++;
            }
            learnAvgErr += sumErr/counter;
        }
        fitnesses.append(learnAvgErr/data.getHeight());
    }
    // ERROR CALCULATION______________________________________________________________________________________
}


QJsonObject Neuro::serialize(){
    QJsonObject data{
        {"layers", static_cast<int>(layers)}
    };

    QJsonArray layerSizes{};
    for(auto &size : neuronAmountPerLayer){
        layerSizes.append(static_cast<qint64>(size));
    }
    data["neuronAmountPerLayer"] = layerSizes;

    QJsonArray layersFunc{};
    for(auto &function : activationFuncForLayer){
        if (function == math_activate::sigmoid){
            layersFunc.append(SIGMOID);
        }else if(function == math_activate::linear){
            layersFunc.append(LINEAR);
        }else if(function == math_activate::reLu){
            layersFunc.append(RELU);
        }else if(function == math_activate::leakyReLu){
            layersFunc.append(LEAKYRELU);
        }else if(function == math_activate::tanhHyp){
            layersFunc.append(TANHHYP);
        }else{
            layersFunc.append("ERROR_AT_PARSING_FUNCTIONS");
        }
    }
    data["activationFuncForLayer"] = layersFunc;

    data["neurons"] = neurons.serialize();
    data["weights"] = weights.serialize();

    return data;
}

bool Neuro::deserialize(QJsonObject data){
    this->layers = static_cast<uint16_t>(data["layers"].toInt());
    this->neuronAmountPerLayer = {};
    this->activationFuncForLayer = {};
    for(auto elem : data["neuronAmountPerLayer"].toArray()){
        this->neuronAmountPerLayer.append(static_cast<size_t>(elem.toInt()));
    }
    bool allFunctionsLoadedRight = true;
    for(auto elem : data["activationFuncForLayer"].toArray()){
        if(elem.toString() == SIGMOID){
            this->activationFuncForLayer.append(math_activate::sigmoid);
        }else if(elem.toString() == LINEAR){
            this->activationFuncForLayer.append(math_activate::linear);
        }else if(elem.toString() == RELU){
            this->activationFuncForLayer.append(math_activate::reLu);
        }else if(elem.toString() == LEAKYRELU){
            this->activationFuncForLayer.append(math_activate::leakyReLu);
        }else if(TANHHYP){
            this->activationFuncForLayer.append(math_activate::tanhHyp);
        }else{
            allFunctionsLoadedRight = false;
            this->activationFuncForLayer.append(math_activate::sigmoid);
        }
    }

    this->neurons.deserialize(data["neurons"].toObject());
    this->weights.deserialize(data["weights"].toObject());

    return allFunctionsLoadedRight;
}
