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
    // Temporary button
    //ui->output->setPlainText("Done!");  // placeholder function for neuro-test

    // 3D vector test
    try{
        Neuro net(3, {16, 32, 10});
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
        net.forwardPropogation(test, math_activate::sigmoid);
        auto res = net.getRes();
        QString textRes = "BEFORE LEARNING\n\n\n";
        for (auto value : res){
            textRes += QString::number(value) + ",\n";
        }
        textRes += "\n\n\nAFTER LEARNING\n\n\n";

        net.learn_backPropogation(teach, ans, 0.5, 1000, math_activate::sigmoid);
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
