#ifndef CHARTPROCESSOR_H
#define CHARTPROCESSOR_H

#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include "../kernel/twodimvector.h"

class chartProcessor{
private:
    static QVector<float> epochErrors;
public:
    chartProcessor();
    ~chartProcessor();
    QChartView* makeChart();

    static void getCurrentError(float err);
};

#endif // CHARTPROCESSOR_H
