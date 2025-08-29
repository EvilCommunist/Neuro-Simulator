#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <./ui/hiddenlayerconfig.h>
#include <./ui/backpropocoeffs.h>
#include <./ui/enums.h>
#include "./ui/functionMap.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    inputSize(1),
    outputSize(1),
    currentInitFuncCoeffs(nullptr),
    currentLearnFuncCoeffs(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    NN = nullptr;

    ui->learnDataTable->setColumnCount(inputSize+outputSize);
    redrawLearnTable();
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


void MainWindow::redrawLearnTable(){
    for(size_t i = 0; i < inputSize; i++){
        QTableWidgetItem *headerInput = new QTableWidgetItem("X"+QString::number(i+1));
        ui->learnDataTable->setHorizontalHeaderItem(i, headerInput);
    }

    for(size_t i = inputSize; i < outputSize+inputSize; i++){
        QTableWidgetItem *headerOutput = new QTableWidgetItem("D"+QString::number(i+1-inputSize));
        ui->learnDataTable->setHorizontalHeaderItem(i, headerOutput);
    }
}


void MainWindow::on_neuroAmountInput_valueChanged(int arg1)
{
    inputSize = arg1;
    ui->learnDataTable->setColumnCount(inputSize+outputSize);
    redrawLearnTable();
}


void MainWindow::on_neuroAmountOutput_valueChanged(int arg1)
{
    outputSize = arg1;
    ui->learnDataTable->setColumnCount(inputSize+outputSize);
    redrawLearnTable();
}


void MainWindow::on_learnAlgorithm_currentIndexChanged(int index)
{
    if(currentLearnFuncCoeffs){
        ui->learnWeightsLayout->removeWidget(currentLearnFuncCoeffs);
        delete currentLearnFuncCoeffs;
        currentLearnFuncCoeffs = nullptr;
    }
    switch(index){
    case NOTHING: break;
    case BACK_PROPOGATION:{
        backPropoCoeffs* coeffWidget = new backPropoCoeffs;
        currentLearnFuncCoeffs = coeffWidget;
        ui->learnWeightsLayout->insertWidget(0, coeffWidget);
        break;
    }
    }
}


void MainWindow::on_startLearning_clicked()
{
    QVector<size_t> neuronsPerLayer{};
    QVector<math_activate::ActivationFunc> functionPerLayer{};
    neuronsPerLayer.append(inputSize);
    functionPerLayer.append(uiKernelMap[ui->activFuncInput->currentText()]);
    for(auto hidden : hiddenLayersConfig){
        auto hiddenLayer = dynamic_cast<HiddenLayerConfig*>(hidden);
        neuronsPerLayer.append(hiddenLayer->getNeuronAmount());
        functionPerLayer.append(hiddenLayer->getActivationFunc());
    }
    neuronsPerLayer.append(outputSize);
    functionPerLayer.append(uiKernelMap[ui->activFuncOutput->currentText()]);

    ThreeDimVector<double> learnData(inputSize, ui->learnDataTable->rowCount(), 1, 0),
        answers(outputSize, ui->learnDataTable->rowCount(), 1, 0);

    for(size_t i = 0; i < inputSize; i++)
        for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++)
            learnData.setValue(i, j, 0, ui->learnDataTable->itemAt(i, j)->text().toDouble());

    for(size_t i = inputSize; i < (inputSize+outputSize); i++)
        for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++)
            answers.setValue(i-inputSize, j, 0, ui->learnDataTable->itemAt(i, j)->text().toDouble());


    NN = new Neuro(2+hiddenLayersConfig.size(), neuronsPerLayer, functionPerLayer);
    for(size_t e = 0; e < ui->learnIterations->value(); e++){
        for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
            QVector<double> data;
            for(size_t i = 0; i < inputSize; i ++){
                data.append(learnData.getValue(i, j, 0));
            }
            QVector<double> ans;
            for(size_t i = 0; i < outputSize; i ++){
                ans.append(answers.getValue(i, j, 0));
            }
            auto curr = dynamic_cast<backPropoCoeffs*>(currentLearnFuncCoeffs); // test now, will change to switch-case or abstract func later
            NN->learn_backPropogation(data,ans,curr->getSpeedCoeff());
        }
    }
    NN->forwardPropogation({2, 3});
    qDebug() << NN->getRes();
}

