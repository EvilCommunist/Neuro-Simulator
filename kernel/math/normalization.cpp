#include "normalization.h"

#define MIN_INTERVAL_VALUE 0
#define MAX_INTERVAL_VALUE 1

double normalization::normalize_0_1(double value, double max, double min){
    if (max != min)
        return (value - min) / (max - min);
    else
        return MAX_INTERVAL_VALUE;
}

double normalization::denormalize_0_1(double value, double max, double min){
    return value*(max-min)+min;
}


void normalization::normalizeSelection_0_1(QVector<double> &data, double max, double min){
    for(int i = 0; i < data.size(); i++){
        data[i] = normalize_0_1(data[i], max, min);
    }
}


void normalization::denormalizeSelection_0_1(QVector<double> &data, double max, double min){
    for(int i = 0; i < data.size(); i++){
        data[i] = denormalize_0_1(data[i], max, min);
    }
}


QPair<double, double> normalization::findMinMax(const QVector<double> &data){
    double max = data[0], min = data[0];
    for(auto item : data){
        if(max < item)
            max = item;
        if(min > item)
            min = item;
    }
    return {min, max};
}
