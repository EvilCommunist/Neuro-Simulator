#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    Ui::MainWindow *ui;
    Neuro* NN; // neuro network
    QVector<math_activate::ActivationFunc> activFuncPerLayer;
    QVector<size_t> neuronsAmountPerLayer;
    QVector<QWidget*> hiddenLayersConfig;
    size_t inputSize; // input amount of neurons
    size_t outputSize; // output amount of neurons

    void redrawLearnTable();
};
#endif // MAINWINDOW_H
