#include "modifiedindividual.h"

ModifiedIndividual::ModifiedIndividual(size_t w, size_t h, size_t d, double val, Crossover type, MutationStrength strength):
    Individual(w, h, d, val),
    crossType(type),
    mutRateForEach(strength)
{}

void ModifiedIndividual::mutate(){
    std::random_device rd;
    std::mt19937 gen(rd());
    double stddev = sqrt(1.0 / width);
    std::normal_distribution<double> dis(-stddev, stddev);
    std::normal_distribution<double> chance(0, 1);

    switch(mutRateForEach){
    case WEAK:
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < depth; k++){
                    if(chance(gen) < WEAK_MUTATION)
                        data.setValue(i, j, k, data.getValue(i, j, k)+dis(gen));
                }
            }
        }
        break;
    case NORMAL:
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < depth; k++){
                    if(chance(gen) < NORMAL_MUTATION)
                        data.setValue(i, j, k, data.getValue(i, j, k)+dis(gen));
                }
            }
        }
        break;
    case STRONG:
        Individual::mutate();
        break;
    }
}

ModifiedIndividual ModifiedIndividual::operator+(ModifiedIndividual& other){
    ThreeDimVector<double> newData(this->width, this->height, this->depth, 0);

    switch(crossType){
    case ARITHMETICAL:
        const float partOfPartner = 0.5;
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < this->depth; k++){
                    newData.setValue(i, j, k,
                                     this->data.getValue(i, j, k)*partOfPartner +
                                                  other.data.getValue(i, j, k)*(1-partOfPartner));
                }
            }
        }
        break;
    case BLX_ALPHA:
        double minWeight = INT16_MAX, maxWeight = INT16_MIN;
        const float alpha = 0.5;
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < this->depth; k++){
                    if(this->data.getValue(i, j, k) < minWeight){
                        minWeight = this->data.getValue(i, j, k);
                    }
                    else if (this->data.getValue(i, j, k) > maxWeight){
                        maxWeight = this->data.getValue(i, j, k);
                    }
                    if(other.data.getValue(i, j, k) < minWeight){
                        minWeight = other.data.getValue(i, j, k);
                    }
                    else if (other.data.getValue(i, j, k) > maxWeight){
                        maxWeight = other.data.getValue(i, j, k);
                    }
                }
            }
        }

        double I = maxWeight - minWeight;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dis(minWeight - I*alpha, maxWeight + I*alpha);
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < this->depth; k++){
                    newData.setValue(i, j, k, dis(gen));
                }
            }
        }
        break;
    case LINEAR:
        break;
    case ONE_POINT:
        newData = Individual::operator+(other).getData();
        break;
    case TWO_POINT:
        break;
    case EVEN:
        break;
    }

    for(size_t i = 0; i < width; i++){
        for(size_t j = 0; j < height; j++){
            for(uint16_t k = 0; k < crossoverPoint; k++){
                newData.setValue(i, j, k, this->data.getValue(i, j, k));
            }
        }
    }
    for(size_t i = 0; i < width; i++){
        for(size_t j = 0; j < height; j++){
            for(uint16_t k = crossoverPoint; k < this->depth; k++){
                newData.setValue(i, j, k, other.data.getValue(i, j, k));
            }
        }
    }
    Individual newInd(this->width, this->height, this->depth);
    newInd.setData(newData);

    return newInd;
}
