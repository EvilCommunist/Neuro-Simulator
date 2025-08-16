#ifndef NEURO_H
#define NEURO_H

#include "threedimvector.h"

class Neuro
{
private:
    ThreeDimVector<float> neurons;
    ThreeDimVector<float> weights;
    uint16_t layers;
    QVector<size_t> neuronAmountPerLayer;
    void generateWeights();

public:
    Neuro(uint16_t l, QVector<size_t> nAPL);
};

#endif // NEURO_H
