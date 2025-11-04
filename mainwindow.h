#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include "kernel/neuro.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addLayer_clicked();
    void on_removeLayer_clicked();
    void on_addSelection_clicked();
    void on_removeSelection_clicked();
    void on_neuroAmountInput_valueChanged(int arg1);
    void on_neuroAmountOutput_valueChanged(int arg1);
    void on_learnAlgorithm_currentIndexChanged(int index);
    void on_addTestSelection_clicked();
    void on_deleteTestSelection_clicked();
    void on_calculateTests_clicked();

    void on_startLearning_clicked(); // test button

    void on_loadLearnData_triggered();
    void on_loadPrognosisData_triggered();
    void on_saveLearnData_triggered();
    void on_savePrognosisData_triggered();

public slots:
    void addHiddenNode(QWidget* layer);
    void removeHiddenNode(QWidget* layer);

private:
    Ui::MainWindow *ui;
    Neuro* NN; // neuro network
    QChartView* currentLearnChart;
    QVector<math_activate::ActivationFunc> activFuncPerLayer;
    //QVector<size_t> neuronsAmountPerLayer;
    QVector<QWidget*> hiddenLayersConfig;
    size_t inputSize; // input amount of neurons
    size_t outputSize; // output amount of neurons

    // visual helpers
    QWidget* currentLearnFuncCoeffs;
    QWidget* currentInitFuncCoeffs;


    void recalculateScrollAreaHeight();
    void redrawLearnTable();
    void redrawCheckTable();
    void redrawForecastTable();
    void fillCheckTable();

    // normalization helpers
    double normMin, normMax;
};
#endif // MAINWINDOW_H
