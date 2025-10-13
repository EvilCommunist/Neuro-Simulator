#include "chartmaker.h"

QChartView* chartProcessing::makeChart(){
    // Создаем серию данных
    QLineSeries *series = new QLineSeries();
    series->setName("График функции y = x²");

    // Заполняем данными (парабола)
    for (int i = 0; i <= 10; ++i) {
        series->append(i, i * i);
    }

    // Создаем график
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Простой график");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Создаем оси
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("X");
    chart->axes(Qt::Vertical).first()->setTitleText("Y");

    // Создаем View для отображения
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}
