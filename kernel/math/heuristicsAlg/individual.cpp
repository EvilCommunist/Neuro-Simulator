#include "individual.h"
#include <random>

Individual::Individual(size_t w, size_t h, size_t d, double val):
    width(w),
    height(h),
    depth(d),
    data(ThreeDimVector(w, h, d, val))
{}

void Individual::generateWeights(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1, 1);

    for(size_t i = 0; i < width; i++){
        for(size_t j = 0; j < height; j++){
            for(uint16_t k = 0; k < depth; k++){
                data.setValue(i, j, k, dis(gen));
            }
        }
    }
}


void Individual::mutate(){
    std::random_device rd;
    std::mt19937 gen(rd());
    double stddev = sqrt(1.0 / width);
    std::normal_distribution<double> dis(0, stddev);

    for(size_t i = 0; i < width; i++){
        for(size_t j = 0; j < height; j++){
            for(uint16_t k = 0; k < depth; k++){
                data.setValue(i, j, k, data.getValue(i, j, k)+dis(gen));
            }
        }
    }
}

Individual Individual::operator+(Individual& other){
    size_t crossoverPoint = 0;
    if(this->depth % 2 == 1){
        crossoverPoint = this->depth/2 + 1;
    }else{
        crossoverPoint = this->depth/2;
    }

    ThreeDimVector<double> newData(this->width, this->height, this->depth, 0);

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
