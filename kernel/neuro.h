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
    void backPropogation(const QVector<double>& ans);

public:
    Neuro(uint16_t l, const QVector<size_t>& nAPL, const QVector<math_activate::ActivationFunc>& aFfL);
    void forwardPropogation(const QVector<double>& data);
    QVector<double> getRes();
    void learn_backPropogation(const TwoDimVector<double>& data, const TwoDimVector<double>& ans, double learnSpeed, size_t epochs);
    void learn_resilentPropogation(const TwoDimVector<double>& data, const TwoDimVector<double>& ans, size_t epochs); // RPROP

    float learnChartHelper();

    inline ThreeDimVector<double> getWeights(){return weights;}
    inline void setWeights(ThreeDimVector<double> weights){this->weights = weights;}
};

#endif // NEURO_H
