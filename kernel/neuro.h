#ifndef NEURO_H
#define NEURO_H

#include "threedimvector.h"
#include "./math/actiovationFunctions.h"

const size_t NeuroDataType = 3, NeuroSignalIndex = 0, NeuroActivateIndex = 1, NeuroErrorIndex = 2;


class Neuro
{
private:
    uint16_t layers;
    QVector<size_t> neuronAmountPerLayer;
    ThreeDimVector<double> neurons;
    ThreeDimVector<double> weights;
    size_t qvectorMax(QVector<size_t> data);
    void initWeights();

public:
    Neuro(uint16_t l, const QVector<size_t>& nAPL);
    void forwardPropogation(QVector<double> data, math_activate::ActivationFunc func);
    QVector<double> getRes();
    void learn_backPropogation(QVector<double> data, QVector<double> ans, double learnSpeed, size_t epochs);

};

#endif // NEURO_H
