#ifndef NEURO_H
#define NEURO_H

#include "threedimvector.h"
#include "twodimvector.h"
#include "./math/activationFunctions.h"

const size_t NeuroDataType = 3, NeuroSignalIndex = 0, NeuroActivateIndex = 1, NeuroErrorIndex = 2;


class Neuro
{
private:
    uint16_t layers;
    QVector<size_t> neuronAmountPerLayer;
    QVector<math_activate::ActivationFunc> activationFuncForLayer;
    ThreeDimVector<double> neurons;
    ThreeDimVector<double> weights;
    size_t qvectorMax(const QVector<size_t>& data);
    void initWeights();

public:
    Neuro(uint16_t l, const QVector<size_t>& nAPL, const QVector<math_activate::ActivationFunc>& aFfL);
    void forwardPropogation(const QVector<double>& data);
    QVector<double> getRes();
    void learn_backPropogation(const TwoDimVector<double>& data, const TwoDimVector<double>& ans, double learnSpeed, size_t epochs);

};

#endif // NEURO_H
