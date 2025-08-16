#include "neuro.h"

size_t Neuro::qvectorMax(QVector<size_t> data){
    size_t max = data[0];
    foreach(auto element, data){
        if(max < element) max = element;
    }
    return max;
}

Neuro::Neuro(uint16_t l, const QVector<size_t>& nAPL):
    layers(l),
    neuronAmountPerLayer(nAPL),
    neurons(ThreeDimVector<float>(qvectorMax(nAPL),NeuroDataType,l,0)),
    weights(ThreeDimVector<float>(qvectorMax(nAPL),qvectorMax(nAPL),l-1,0))
{/*Every initialisation is in the cunstructor above*/}

