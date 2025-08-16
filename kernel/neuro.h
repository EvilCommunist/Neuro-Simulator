#ifndef NEURO_H
#define NEURO_H

#include "threedimvector.h"

const int NeuroDataType = 3;


class Neuro
{
private:
    uint16_t layers;
    QVector<size_t> neuronAmountPerLayer;
    ThreeDimVector<float> neurons;
    ThreeDimVector<float> weights;
    size_t qvectorMax(QVector<size_t> data);

public:
    Neuro(uint16_t l, const QVector<size_t>& nAPL);
};

#endif // NEURO_H
