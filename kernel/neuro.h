#ifndef NEURO_H
#define NEURO_H

#include "threedimvector.h"
#include "./math/activationFunctions.h"

const size_t NeuroDataType = 3, NeuroSignalIndex = 0, NeuroActivateIndex = 1, NeuroErrorIndex = 2;


class Neuro
{
private:
    uint16_t layers;
    QVector<size_t> neuronAmountPerLayer;
    ThreeDimVector<double> neurons;
    ThreeDimVector<double> weights;
    size_t qvectorMax(const QVector<size_t>& data);
    void initWeights();

public:
    Neuro(uint16_t l, const QVector<size_t>& nAPL);
    void forwardPropogation(const QVector<double>& data, math_activate::ActivationFunc func);
    QVector<double> getRes();
    void learn_backPropogation(const QVector<double>& data, const QVector<double>& ans, double learnSpeed, math_activate::ActivationFunc func);

};

#endif // NEURO_H
