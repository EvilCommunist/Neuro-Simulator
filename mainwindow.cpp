#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
   try{
        Neuro net(3, {16, 32, 10}, {math_activate::sigmoid, math_activate::sigmoid, math_activate::sigmoid});
        QVector<double> teach = {
                                    1, 0, 1, 0,
                                    1, 1, 1, 0,
                                    0, 0, 1, 0,
                                    0, 0, 1, 0
                                },
            test = {
                        1, 0, 0, 1,
                        1, 1, 1, 1,
                        0, 0, 0, 1,
                        0, 0, 0, 1
                    },
            ans = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
        net.forwardPropogation(test);
        auto res = net.getRes();
        QString textRes = "BEFORE LEARNING\n\n\n";
        for (auto value : res){
            textRes += QString::number(value) + ",\n";
        }
        textRes += "\n\n\nAFTER LEARNING\n\n\n";

        for (int i = 0; i < 1000; i++)
            net.learn_backPropogation(teach, ans, 0.5);
        res = net.getRes();
        for (auto value : res){
            textRes += QString::number(value) + ",\n";
        }
        ui->output->setPlainText(textRes);
    }
    catch(std::exception Ex){
        ui->output->setPlainText(Ex.what());
    }
}
