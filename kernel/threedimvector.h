#ifndef THREEDIMVECTOR_H
#define THREEDIMVECTOR_H

#include <QVector>

template <typename T>
class ThreeDimVector
{
private:
    size_t width, height, depth;  // i, j, k
    QVector<T> data;

    inline size_t recalculateIndex(size_t i, size_t j, size_t k){
        return i+width*j+width*height*k;
    }
public:
    ThreeDimVector(size_t w, size_t h, size_t d, T val)
    : width(w), height(h), depth(d)
    {
        data.fill(val, w*h*d);
    }

    inline void setValue(size_t i, size_t j, size_t k, T value){
        data[recalculateIndex(i, j, k)] = value;
    }

    inline T getValue(size_t i, size_t j, size_t k){
        return data[recalculateIndex(i, j, k)];
    }
};

#endif // THREEDIMVECTOR_H
