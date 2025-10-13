#include "chartprocessor.h"

QVector<float> chartProcessor::epochErrors = {};

chartProcessor::chartProcessor(){
    epochErrors = {};
}
chartProcessor::~chartProcessor(){
    epochErrors = {};
}

QChartView* chartProcessor::makeChart(){
    QLineSeries *series = new QLineSeries();
    series->setName("График средней ошибки обучения нейросети по эпохам");

    for (int i = 0; i < epochErrors.size(); i++) {
        series->append(i+1, epochErrors[i]);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("График ошибки обучения");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("Эпоха");
    chart->axes(Qt::Vertical).first()->setTitleText("Средняя ошибка");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

void chartProcessor::getCurrentError(float err){
    epochErrors.append(err);
}
