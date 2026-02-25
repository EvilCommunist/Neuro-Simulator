#ifndef NORMALIZATION_H
#define NORMALIZATION_H
#include <QVector>
#include <QPair>

namespace normalization{
enum Range{
    ZERO_ONE,
    MINUS_ONE_ONE
};

/*--Functions for normalization--*/
typedef double(*Normilize)(double, double, double);
typedef double(*Denormilize)(double, double, double);

typedef void(*NormilizeSelection)(QVector<double>&, double, double);
typedef void(*DenormilizeSelection)(QVector<double>&, double, double);


double normalize_0_1(double value, double max, double min);
double denormalize_0_1(double value, double max, double min);

void normalizeSelection_0_1(QVector<double>& data, double max, double min);
void denormalizeSelection_0_1(QVector<double>& data, double max, double min);

QPair<double, double> findMinMax(const QVector<double>& data);
/*--Functions for normalization--*/
}

#endif // NORMALIZATION_H
