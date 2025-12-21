#ifndef TWODIMVECTOR_H
#define TWODIMVECTOR_H

#include <QVector>

template <typename T>
class TwoDimVector
{
private:
    size_t width, height;  // i, j
    QVector<T> data;

    inline size_t recalculateIndex(size_t i, size_t j) const{
        return i+width*j;
    }

    inline void checkIndex(size_t i, size_t j) const{
        if(i >= width || j >= height){throw std::out_of_range("Attention! Index out of range");}
    }
public:
    TwoDimVector(size_t w, size_t h, T val)
        : width(w), height(h)
    {
        data.fill(val, w*h);
    }

    inline void setValue(size_t i, size_t j, T value){
        checkIndex(i, j);
        data[recalculateIndex(i, j)] = value;
    }

    inline T getValue(size_t i, size_t j) const{
        checkIndex(i, j);
        return data[recalculateIndex(i, j)];
    }

    QVector<T> getLine(size_t index) const {
        QVector<T> line{};
        for(size_t i = 0; i < width; i++){
            line.append(data[recalculateIndex(i, index)]);
        }
        return line;
    }

    void setLine(const QVector<T>& data, size_t index){
        for(size_t i = 0; i < width; i++){
            this->data[recalculateIndex(i, index)] = data[i];
        }
    }

    QVector<T> getRow(size_t index) const {
        QVector<T> row{};
        for(size_t i = 0; i < height; i++){
            row.append(data[recalculateIndex(index, i)]);
        }
        return row;
    }

    void setRow(const QVector<T>& data, size_t index){
        for(size_t i = 0; i < height; i++){
            this->data[recalculateIndex(index, i)] = data[i];
        }
    }

    inline size_t getWidth() const { return width; }
    inline size_t getHeight() const { return height; }
};

#endif // TWODIMVECTOR_H
