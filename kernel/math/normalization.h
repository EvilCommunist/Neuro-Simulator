#ifndef NORMALIZATION_H
#define NORMALIZATION_H
#include <QVector>
#include <QPair>

namespace normalization{
/*--Functions for normalization--*/
double normalize(double value, double max, double min);
double denormalize(double value, double max, double min);

void normalizeSelection(QVector<double>& data, double max, double min);
void denormalizeSelection(QVector<double>& data, double max, double min);

QPair<double, double> findMinMax(const QVector<double>& data);
/*--Functions for normalization--*/
}

#endif // NORMALIZATION_H
