#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./ui/hiddenlayerconfig.h"
#include "./ui/backpropocoeffs.h"
#include "./ui/enums.h"
#include "./ui/functionMap.h"
#include "./kernel/math/normalization.h"
#include "./kernel/twodimvector.h"

#include <QPair>


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

    redrawLearnTable();
    redrawCheckTable();
    redrawForecastTable();
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
    ui->learnDataTable->setColumnCount(inputSize+outputSize);
    for(size_t i = 0; i < inputSize; i++){
        QTableWidgetItem *headerInput = new QTableWidgetItem("X"+QString::number(i+1));
        ui->learnDataTable->setHorizontalHeaderItem(i, headerInput);
    }

    for(size_t i = inputSize; i < outputSize+inputSize; i++){
        QTableWidgetItem *headerOutput = new QTableWidgetItem("D"+QString::number(i+1-inputSize));
        ui->learnDataTable->setHorizontalHeaderItem(i, headerOutput);
    }
}

void MainWindow::redrawCheckTable(){
    ui->checkLearned->setColumnCount(inputSize+outputSize+outputSize);
    for(size_t i = 0; i < inputSize; i++){
        QTableWidgetItem *headerInput = new QTableWidgetItem("X"+QString::number(i+1));
        ui->checkLearned->setHorizontalHeaderItem(i, headerInput);
    }

    for(size_t i = inputSize; i < outputSize + inputSize; i++){
        QTableWidgetItem *headerOutput = new QTableWidgetItem("D"+QString::number(i+1-inputSize));
        ui->checkLearned->setHorizontalHeaderItem(i, headerOutput);
    }

    for(size_t i = inputSize + outputSize; i < outputSize + inputSize + outputSize; i++){
        QTableWidgetItem *headerOutput = new QTableWidgetItem("E"+QString::number(i+1-inputSize - outputSize));
        ui->checkLearned->setHorizontalHeaderItem(i, headerOutput);
    }
}

void MainWindow::redrawForecastTable(){
    ui->prognosisTable->setColumnCount(inputSize+outputSize);
    for(size_t i = 0; i < inputSize; i++){
        QTableWidgetItem *headerInput = new QTableWidgetItem("X"+QString::number(i+1));
        ui->prognosisTable->setHorizontalHeaderItem(i, headerInput);
    }

    for(size_t i = inputSize; i < outputSize+inputSize; i++){
        QTableWidgetItem *headerOutput = new QTableWidgetItem("D"+QString::number(i+1-inputSize));
        ui->prognosisTable->setHorizontalHeaderItem(i, headerOutput);
    }
}


void MainWindow::on_neuroAmountInput_valueChanged(int arg1)
{
    inputSize = arg1;
    redrawLearnTable();
    redrawCheckTable();
    redrawForecastTable();
}


void MainWindow::on_neuroAmountOutput_valueChanged(int arg1)
{
    outputSize = arg1;
    redrawLearnTable();
    redrawCheckTable();
    redrawForecastTable();
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

    TwoDimVector<double> learnData(inputSize, ui->learnDataTable->rowCount(), 0),
        answers(outputSize, ui->learnDataTable->rowCount(), 0);


    QVector<QPair<double, double>> minMaxInput{};
    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        for(size_t i = 0; i < inputSize; i++){
            learnData.setValue(i, j, ui->learnDataTable->item(j, i)->text().toDouble());
        }
        minMaxInput.append(normalization::findMinMax(learnData.getLine(j)));
    }

    QVector<QPair<double, double>> minMaxOutput{};
    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        for(size_t i = inputSize; i < (inputSize+outputSize); i++){
            answers.setValue(i-inputSize, j, ui->learnDataTable->item(j, i)->text().toDouble());
        }
        minMaxOutput.append(normalization::findMinMax(answers.getLine(j)));
    }

    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        auto data = learnData.getLine(j);
        normalization::normalizeSelection(data, minMaxInput[j].second, minMaxInput[j].first);
        learnData.setLine(data, j);
        auto ans = answers.getLine(j);
        normalization::normalizeSelection(ans, minMaxOutput[j].second, minMaxOutput[j].first);
        answers.setLine(ans, j);
    }


    if (NN){
        delete NN;
        NN = nullptr;
    }
    NN = new Neuro(2+hiddenLayersConfig.size(), neuronsPerLayer, functionPerLayer);
    for(size_t e = 0; e < ui->learnIterations->value(); e++){
        for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
            auto data = learnData.getLine(j);
            auto ans = answers.getLine(j);
            auto curr = dynamic_cast<backPropoCoeffs*>(currentLearnFuncCoeffs); // test now, will change to switch-case or abstract func later
            NN->learn_backPropogation(data,ans,curr->getSpeedCoeff());
        }
    }

    fillCheckTable();
}

void MainWindow::fillCheckTable(){
    ui->checkLearned->setRowCount(ui->learnDataTable->rowCount());

    TwoDimVector<double> learnData(inputSize, ui->learnDataTable->rowCount(), 0),
        answers(outputSize, ui->learnDataTable->rowCount(), 0);


    QVector<QPair<double, double>> minMaxInput{};
    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        for(size_t i = 0; i < inputSize; i++){
            QTableWidgetItem *dataDuplicate = new QTableWidgetItem(ui->learnDataTable->item(j, i)->text());
            ui->checkLearned->setItem(j, i, dataDuplicate);
            learnData.setValue(i, j, ui->learnDataTable->item(j, i)->text().toDouble());
            minMaxInput.append(normalization::findMinMax(learnData.getLine(j)));
        }
    }

    QVector<QPair<double, double>> minMaxOutput{};
    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        for(size_t i = inputSize; i < (inputSize+outputSize); i++){
            answers.setValue(i-inputSize, j, ui->learnDataTable->item(j, i)->text().toDouble());
        }
        minMaxOutput.append(normalization::findMinMax(answers.getLine(j)));
    }

    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        auto data = learnData.getLine(j);
        NN->forwardPropogation(data);
        auto ans = NN->getRes();
        for(size_t i = inputSize; i < (inputSize+outputSize); i++){
            QTableWidgetItem *neuroAnswer = new QTableWidgetItem(QString::number(normalization::denormalize(ans[i-(inputSize)], minMaxOutput[j].second, minMaxOutput[j].first)));
            ui->checkLearned->setItem(j, i, neuroAnswer);
        }
    }


    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        for(size_t i = inputSize  + outputSize; i < (inputSize+outputSize+outputSize); i++){
            double diff = abs(ui->learnDataTable->item(j, i-outputSize)->text().toDouble() - ui->checkLearned->item(j, i-outputSize)->text().toDouble());
            QTableWidgetItem *neuroDifference = new QTableWidgetItem(QString::number(diff));
            ui->checkLearned->setItem(j, i, neuroDifference);
        }
    }
}


void MainWindow::on_addTestSelection_clicked()
{
    ui->prognosisTable->setRowCount(ui->prognosisTable->rowCount()+1);
}


void MainWindow::on_deleteTestSelection_clicked()
{
    if(ui->prognosisTable->rowCount() > 0)
        ui->prognosisTable->setRowCount(ui->prognosisTable->rowCount()-1);
}


void MainWindow::on_calculateTests_clicked()
{
    for(size_t j = 0; j < ui->prognosisTable->rowCount(); j++){
        QVector<double> data{};
        double min = ui->prognosisTable->item(j, 0)->text().toDouble(), max = ui->prognosisTable->item(j, 0)->text().toDouble();
        for(size_t i = 0; i < inputSize; i++){
            data.append(ui->prognosisTable->item(j, i)->text().toDouble());
            if(max < ui->prognosisTable->item(j, i)->text().toDouble()){
                max = ui->prognosisTable->item(j, i)->text().toDouble();
            }
            if(min > ui->prognosisTable->item(j, i)->text().toDouble()){
                min = ui->prognosisTable->item(j, i)->text().toDouble();
            }
        }
        for(size_t i = 0; i < data.size(); i++){
            data[i] = normalization::normalize(data[i], max, min);
        }
        NN->forwardPropogation(data);
        auto ans = NN->getRes();
        double minAns = ans[0], maxAns = ans[0];
        for(size_t i = 0; i < outputSize; i++){
            if(maxAns < ans[i]){
                maxAns = ans[i];
            }
            if(minAns > ans[i]){
                minAns = ans[i];
            }
        }
        for(size_t i = inputSize; i < (inputSize+outputSize); i++){
            QTableWidgetItem *neuroAnswer = new QTableWidgetItem(QString::number(normalization::denormalize(ans[i-(inputSize)], max, min))); // вопрос нормализации???
            ui->prognosisTable->setItem(j, i, neuroAnswer);
        }
    }
}

