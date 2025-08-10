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
    ThreeDimVector<int> drei(5,5,5,0);
    drei.setValue(2,1,4,100);
    ui->output->setPlainText(QString::number(drei.getValue(2,1,4)));
}

