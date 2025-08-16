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
        Neuro NN(3, {2, 3, 1});
        ui->output->setPlainText("Done!");
    }
    catch(std::exception Ex){
        ui->output->setPlainText(Ex.what());
    }
}

