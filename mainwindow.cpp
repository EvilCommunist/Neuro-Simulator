#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <./ui/hiddenlayerconfig.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    inputSize(1),
    outputSize(1),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    NN = nullptr;

    ui->learnDataTable->setColumnCount(2);
    QTableWidgetItem *headerInput = new QTableWidgetItem("X1");
    QTableWidgetItem *headerOutput = new QTableWidgetItem("D1");
    ui->learnDataTable->setHorizontalHeaderItem(0, headerInput);
    ui->learnDataTable->setHorizontalHeaderItem(1, headerOutput);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addLayer_clicked()
{
    HiddenLayerConfig* hLC = new HiddenLayerConfig;
    hiddenLayersConfig.append(hLC);
    hLC->setNumber(hiddenLayersConfig.size());
    ui->hiddenLayersLayout->addWidget(hLC);
}


void MainWindow::on_removeLayer_clicked()
{
    if (hiddenLayersConfig.isEmpty())
        return;
    auto hLC = hiddenLayersConfig.takeLast();
    ui->hiddenLayersLayout->removeWidget(hLC);
    delete hLC;
}


void MainWindow::on_addSelection_clicked()
{
    ui->learnDataTable->setRowCount(ui->learnDataTable->rowCount()+1);
}


void MainWindow::on_removeSelection_clicked()
{
    if(ui->learnDataTable->rowCount() > 0){
        ui->learnDataTable->setRowCount(ui->learnDataTable->rowCount()-1);
    }
}

