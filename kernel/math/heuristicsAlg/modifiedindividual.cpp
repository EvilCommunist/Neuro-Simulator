#include "modifiedindividual.h"
#include <qrandom.h>

ModifiedIndividual::ModifiedIndividual(size_t w, size_t h, size_t d, double val, Crossover type, MutationStrength strength):
    Individual(w, h, d, val),
    crossType(type),
    mutRateForEach(strength)
{}

void ModifiedIndividual::mutate(){
    std::random_device rd;
    std::mt19937 gen(rd());
    double stddev = sqrt(1.0 / width);
    std::normal_distribution<double> dis(0, stddev);
    std::uniform_real_distribution<double> chance(0, 1);

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
    case ARITHMETICAL:{
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
    }
        break;
    case BLX_ALPHA:{
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
        std::uniform_real_distribution<double> dis(minWeight - I*alpha, maxWeight + I*alpha);
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < this->depth; k++){
                    newData.setValue(i, j, k, dis(gen));
                }
            }
        }
    }
        break;
    case LINEAR:{
        ThreeDimVector<double> newData1(this->width, this->height, this->depth, 0);
        ThreeDimVector<double> newData2(this->width, this->height, this->depth, 0);
        ThreeDimVector<double> newData3(this->width, this->height, this->depth, 0);
        const uint8_t childs = 3;

        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < this->depth; k++){
                    newData1.setValue(i, j, k, 0.5*this->data.getValue(i, j, k) + 0.5*other.data.getValue(i, j, k));
                }
            }
        }
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < this->depth; k++){
                    newData2.setValue(i, j, k, -0.5*this->data.getValue(i, j, k) + 1.5*other.data.getValue(i, j, k));
                }
            }
        }
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < this->depth; k++){
                    newData3.setValue(i, j, k, 1.5*this->data.getValue(i, j, k) - 0.5*other.data.getValue(i, j, k));
                }
            }
        }

        QVector<ThreeDimVector<double>*> kindergarten {&newData1, &newData2, &newData3};
        newData = *kindergarten[QRandomGenerator::global()->bounded(childs)];
    }
        break;
    case ONE_POINT:{
        newData = Individual::operator+(other).getData();
    }
        break;
    case TWO_POINT:{
        size_t crossoverPoint1 = 0, crossoverPoint2 = 0;
        if(this->depth % 2 == 1){
            crossoverPoint1 = this->depth/3 + 1;
            crossoverPoint2 = 2*this->depth/3 + 1;
        }else{
            crossoverPoint1 = this->depth/3;
            crossoverPoint2 = 2*this->depth/3;
        }

        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < crossoverPoint1; k++){
                    newData.setValue(i, j, k, this->data.getValue(i, j, k));
                }
            }
        }
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = crossoverPoint1; k < crossoverPoint2; k++){
                    newData.setValue(i, j, k, other.data.getValue(i, j, k));
                }
            }
        }
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = crossoverPoint2; k < this->depth; k++){
                    newData.setValue(i, j, k, this->data.getValue(i, j, k));
                }
            }
        }
    }
        break;
    case EVEN:{
        QVector<ModifiedIndividual*> pair{this, &other};
        const uint8_t size = 2;
        for(size_t i = 0; i < width; i++){
            for(size_t j = 0; j < height; j++){
                for(uint16_t k = 0; k < this->depth; k++){
                    newData.setValue(i, j, k, pair[QRandomGenerator::global()->bounded(size)]->data.getValue(i,j,k));
                }
            }
        }
    }
        break;
    }
    ModifiedIndividual newInd(this->width, this->height, this->depth, 0, this->crossType, this->mutRateForEach);
    newInd.setData(newData);

    return newInd;
}
