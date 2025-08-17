#ifndef NEURO_H
#define NEURO_H

#include "threedimvector.h"
#include "./math/actiovationFunctions.h"

const int NeuroDataType = 3;


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
    void forwardPropogation(size_t size, QVector<double> data);
};

#endif // NEURO_H
