#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "../../threedimvector.h"

class Individual    // the individual is a three dimensional weight matrix
{
protected:
    ThreeDimVector<double> data;
    size_t width, height, depth;
    double fitness;
public:
    Individual(size_t w, size_t h, size_t d, double val = 0);

    void generateWeights();

    inline void setFitness(double fitness){this->fitness=fitness;}
    inline double getFitness(){return fitness;}
    inline ThreeDimVector<double> getData() {return data;}
    inline void setData(ThreeDimVector<double> data) {this->data = data;}
    inline size_t getDepth(){return depth;}

    void mutate();
    Individual operator+(Individual& other); // Crossover operation
};

#endif // INDIVIDUAL_H
