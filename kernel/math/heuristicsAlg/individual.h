#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "../../threedimvector.h"

class Neuro;

class Individual    // the individual is a three dimensional weight matrix
{
private:
    ThreeDimVector<double>* data;
    size_t width, height, depth;
    double fitness;

    void generateWeights();
public:
    Individual(size_t w, size_t h, size_t d, double val = 0);
    ~Individual();

    void calculateFitness(Neuro* network, const QVector<double>& data);
    inline ThreeDimVector<double>* getData() {return data;}
    inline void setData(ThreeDimVector<double>* data) {delete this->data; this->data = data;}
};

#endif // INDIVIDUAL_H
