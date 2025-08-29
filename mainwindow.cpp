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

/*______________________________________________*/
double normalize(double value, double max, double min){ // later make library
    return (value - min) / (max - min);
}

double denormalize(double value, double max, double min){ // later make library
    return value*(max-min)+min;
}
/*______________________________________________*/

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


    double maxInput = INT16_MIN, minInput = INT16_MAX;
    for(size_t i = 0; i < inputSize; i++){
        for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
            learnData.setValue(i, j, 0, ui->learnDataTable->item(j, i)->text().toDouble());
            if(maxInput < learnData.getValue(i, j, 0)){
                maxInput = learnData.getValue(i, j, 0);
            }
            if(minInput > learnData.getValue(i, j, 0)){
                minInput = learnData.getValue(i, j, 0);
            }
        }
    }

    double maxOutput = INT16_MIN, minOutput = INT16_MAX;
    for(size_t i = inputSize; i < (inputSize+outputSize); i++){
        for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
            answers.setValue(i-inputSize, j, 0, ui->learnDataTable->item(j, i)->text().toDouble());
            if(maxOutput < answers.getValue(i-inputSize, j, 0)){
                maxOutput = answers.getValue(i-inputSize, j, 0);
            }
            if(minOutput > answers.getValue(i-inputSize, j, 0)){
                minOutput = answers.getValue(i-inputSize, j, 0);
            }
        }
    }

    for(size_t i = 0; i < inputSize; i++){
        for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
            learnData.setValue(i, j, 0, normalize(learnData.getValue(i, j, 0), maxInput, minInput));
        }
    }

    for(size_t i = 0; i < outputSize; i++){
        for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
            answers.setValue(i, j, 0, normalize(answers.getValue(i, j, 0), maxOutput, minOutput));
        }
    }


    if (NN){
        delete NN;
        NN = nullptr;
    }
    NN = new Neuro(2+hiddenLayersConfig.size(), neuronsPerLayer, functionPerLayer);
    for(size_t e = 0; e < ui->learnIterations->value(); e++){
        for(size_t j = 0; j < ui->learnDataTable->rowCount()-inputSize; j+=inputSize){
            QVector<double> data;
            for(size_t i = 0; i < inputSize; i ++){
                data.append(learnData.getValue(i, j, 0)); // change to QMatrix and add error processor
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
    qDebug() << denormalize(NN->getRes()[0], maxOutput, minOutput);
}

