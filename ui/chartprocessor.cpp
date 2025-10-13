#include "chartprocessor.h"

QVector<float> chartProcessor::epochErrors = {};

chartProcessor::chartProcessor(){
    epochErrors = {};
}
chartProcessor::~chartProcessor(){
    epochErrors = {};
}

QChartView* chartProcessor::makeChart(){
    qDebug() << epochErrors;
    QLineSeries *series = new QLineSeries();
    series->setName("График средней ошибки обучения нейросети по эпохам");

    for (int i = 0; i <= epochErrors.size(); i++) {
        series->append(i, epochErrors[i]);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("График ошибки обучения");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("X");
    chart->axes(Qt::Vertical).first()->setTitleText("Y");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

void chartProcessor::getCurrentError(float err){
    epochErrors.append(err);
}
