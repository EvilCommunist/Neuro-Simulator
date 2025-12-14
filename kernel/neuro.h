#ifndef NEURO_H
#define NEURO_H

#include "threedimvector.h"
#include "twodimvector.h"
#include "./math/activationFunctions.h"
#include "./math/heuristicsAlg/GeneticOperatorsEnum.h"

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
    void learn_geneticAlgorithm(const TwoDimVector<double>& data, const TwoDimVector<double>& ans, size_t epochs, size_t popSize, float pMute, float pCross);
    void learn_modifiedGeneticAlgorithm(const TwoDimVector<double>& data, const TwoDimVector<double>& ans, size_t epochs, size_t popSize, float pMute, float pCross,
                                        Crossover crossType, MutationStrength mutStrength, Selection selType, uint8_t workers);

    float learnChartHelper();

    inline ThreeDimVector<double> getWeights(){return weights;}
    inline void setWeights(ThreeDimVector<double> weights){this->weights = weights;}
};

#endif // NEURO_H
