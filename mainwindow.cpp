#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./ui/hiddenlayerconfig.h"
#include "./ui/backpropocoeffs.h"
#include "./ui/enums.h"
#include "./ui/functionMap.h"
#include "./kernel/math/normalization.h"
#include "./kernel/twodimvector.h"
#include "./kernel/files/csvprocessor.h"
#include "./ui/adaptlearndatadialog.h"
#include "./ui/chartprocessor.h"
#include "./ui/geneticalgcoefs.h"
#include "./kernel/math/heuristicsAlg/GeneticOperatorsEnum.h"
#include "./ui/modifgacoefs.h"

#include <QPair>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <algorithm>  // test


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    inputSize(1),
    outputSize(1),
    currentInitFuncCoeffs(nullptr),
    currentLearnFuncCoeffs(nullptr),
    ui(new Ui::MainWindow),
    normMin({}), normMax({})
{
    ui->setupUi(this);
    NN = nullptr;
    currentLearnChart = nullptr;
    setWindowTitle("Neuro Simulator [BETA]");

    redrawLearnTable();
    redrawCheckTable();
    redrawForecastTable();
    on_learnAlgorithm_currentIndexChanged(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recalculateScrollAreaHeight(){
    int totalHeight = 0;
    for (auto layer : hiddenLayersConfig) {
        totalHeight += layer->height() + ui->hiddenLayersLayout->spacing();
    }
    totalHeight += ui->hiddenLayersLayout->contentsMargins().top() +
                   ui->hiddenLayersLayout->contentsMargins().bottom() + 20; // 20 px for not making hidden layers partly shown
    ui->scrollAreaWidgetContents_2->setFixedHeight(totalHeight);
}

void MainWindow::on_addLayer_clicked()
{
    HiddenLayerConfig* hLC = new HiddenLayerConfig;
    hiddenLayersConfig.append(hLC);
    hLC->setNumber(hiddenLayersConfig.size());
    ui->hiddenLayersLayout->addWidget(hLC);
    ui->neuroGraphicsView->addLayer();

    recalculateScrollAreaHeight();

    connect(hLC, &HiddenLayerConfig::signalAddHiddenNode,
            this, &MainWindow::addHiddenNode);
    connect(hLC, &HiddenLayerConfig::signalRemoveHiddenNode,
            this, &MainWindow::removeHiddenNode);
}


void MainWindow::on_removeLayer_clicked()
{
    if (hiddenLayersConfig.isEmpty())
        return;
    auto hLC = hiddenLayersConfig.takeLast();
    ui->hiddenLayersLayout->removeWidget(hLC);
    delete hLC;
    ui->neuroGraphicsView->removeLayer();

    recalculateScrollAreaHeight();
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
    int times = abs(arg1-static_cast<int>(inputSize));
    if(inputSize < arg1){
        for(int i = 0; i < times; i++)
            ui->neuroGraphicsView->addNode(0);
    } else {
        for(int i = 0; i < times; i++)
            ui->neuroGraphicsView->removeNode(0);
    }
    inputSize = arg1;
    redrawLearnTable();
    redrawCheckTable();
    redrawForecastTable();
}


void MainWindow::on_neuroAmountOutput_valueChanged(int arg1)
{
    int times = abs(arg1-static_cast<int>(outputSize));
    if(outputSize < arg1){
        for(int i = 0; i < times; i++)
            ui->neuroGraphicsView->addNode(ui->neuroGraphicsView->getNeuroLen()-1);
    } else {
        for(int i = 0; i < times; i++)
            ui->neuroGraphicsView->removeNode(ui->neuroGraphicsView->getNeuroLen()-1);
    }
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
    case BACK_PROPOGATION:{
        backPropoCoeffs* coeffWidget = new backPropoCoeffs;
        currentLearnFuncCoeffs = coeffWidget;
        ui->learnWeightsLayout->insertWidget(0, coeffWidget);
        break;
    }
    case RESILENT_PROPOGATION:{
        break;
    }
    case GENETIC_ALGORITHM:{
        GeneticAlgCoefs* geneticWidget = new GeneticAlgCoefs;
        currentLearnFuncCoeffs = geneticWidget;
        ui->learnWeightsLayout->insertWidget(0, geneticWidget);
        break;
    }
    case MODIFIED_GA:{
        ModifGACoefs* modifiedGeneticWidget = new ModifGACoefs;
        currentLearnFuncCoeffs = modifiedGeneticWidget;
        ui->learnWeightsLayout->insertWidget(0, modifiedGeneticWidget);
        break;
    }
    }
}

void MainWindow::normalizeData(TwoDimVector<double> &data){
    for(int i = 0; i < data.getWidth(); i++){
        double min = INT16_MAX, max = INT16_MIN;
        auto res = normalization::findMinMax(data.getRow(i));
        if(min > res.first)
            min = res.first;
        if(max < res.second)
            max = res.second;
        auto row = data.getRow(i);
        normalization::normalizeSelection(row, max, min);
        data.setRow(row, i);
        normMin.append(min);
        normMax.append(max);
    }
}

void MainWindow::normalizeExample(QVector<double> &example){
    for(int i = 0; i < example.size(); i++){
        example[i] = normalization::normalize(example[i], normMax[i], normMin[i]);
    }
}

void MainWindow::denormalizeAns(QVector<double> &answer){
    for(int i = inputSize-1; i < inputSize+outputSize-1; i++){ // brokes here
        answer[i-inputSize + 1] = normalization::normalize(answer[i-inputSize + 1], normMax[i], normMin[i]);
    }
}

void MainWindow::on_startLearning_clicked()
{
    normMax = {};
    normMin = {};
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


    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        for(size_t i = 0; i < inputSize; i++){
            learnData.setValue(i, j, ui->learnDataTable->item(j, i)->text().toDouble());
        }
    }

    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        for(size_t i = inputSize; i < (inputSize+outputSize); i++){
            answers.setValue(i-inputSize, j, ui->learnDataTable->item(j, i)->text().toDouble());
        }
    }


    /*_______________________NORMALIZE DATA__________________________*/
    normalizeData(learnData);
    normalizeData(answers);
    /*_______________________NORMALIZE DATA__________________________*/

    if (NN){
        delete NN;
        NN = nullptr;
    }
    NN = new Neuro(2+hiddenLayersConfig.size(), neuronsPerLayer, functionPerLayer);
    chartProcessor* cp = new chartProcessor;
    switch(ui->learnAlgorithm->currentIndex()){
    case BACK_PROPOGATION:{
        auto curr = dynamic_cast<backPropoCoeffs*>(currentLearnFuncCoeffs);
        NN->learn_backPropogation(learnData,answers,curr->getSpeedCoeff(), ui->learnIterations->value());
        break;
    }
    case RESILENT_PROPOGATION:{
        NN->learn_resilentPropogation(learnData, answers, ui->learnIterations->value());
        break;
    }
    case GENETIC_ALGORITHM:{
        auto curr = dynamic_cast<GeneticAlgCoefs*>(currentLearnFuncCoeffs);
        NN->learn_geneticAlgorithm(learnData, answers, ui->learnIterations->value(), curr->getPopSize(), curr->getMutationProb(), curr->getCrossoverProb());
        break;
    }
    case MODIFIED_GA:{
        auto curr = dynamic_cast<ModifGACoefs*>(currentLearnFuncCoeffs);
        NN->learn_modifiedGeneticAlgorithm(learnData, answers, ui->learnIterations->value(), curr->getPopSize(), curr->getMutationProb(),
                                           curr->getCrossoverProb(), curr->getCrossType(), curr->getMutStrength(), curr->getSelType(), curr->getWorkers());
        break;
    }
    }

    qDebug() << "NormMax: " << normMax << "\nnormMin: " << normMin;

    fillCheckTable();
    if(currentLearnChart){
        ui->chartLayout->removeWidget(currentLearnChart);
        delete currentLearnChart;
    }
    currentLearnChart = cp->makeChart();
    ui->chartLayout->addWidget(currentLearnChart);
    delete cp;
}

void MainWindow::fillCheckTable(){
    ui->checkLearned->setRowCount(ui->learnDataTable->rowCount());

    TwoDimVector<double> learnData(inputSize, ui->learnDataTable->rowCount(), 0),
        answers(outputSize, ui->learnDataTable->rowCount(), 0);


    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        for(size_t i = 0; i < inputSize; i++){
            QTableWidgetItem *dataDuplicate = new QTableWidgetItem(ui->learnDataTable->item(j, i)->text());
            ui->checkLearned->setItem(j, i, dataDuplicate);
            learnData.setValue(i, j, ui->learnDataTable->item(j, i)->text().toDouble());
        }
    }

    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        for(size_t i = inputSize; i < (inputSize+outputSize); i++){
            answers.setValue(i-inputSize, j, ui->learnDataTable->item(j, i)->text().toDouble());
        }
    }

    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        auto data = learnData.getLine(j);
        qDebug() << "Raw: " << data;
        normalizeExample(data);  // normalize data
        qDebug() << "Norm: " << data;
        NN->forwardPropogation(data);
        auto ans = NN->getRes();
        qDebug() << "Raw ans: " << ans;
        denormalizeAns(ans); // denormalize answer
        qDebug() << "Ans: " << ans;
        for(size_t i = inputSize; i < (inputSize+outputSize); i++){
            QTableWidgetItem *neuroAnswer = new QTableWidgetItem(QString::number(ans[i-(inputSize)]));
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
    TwoDimVector<double> testData(inputSize, ui->prognosisTable->rowCount(), 0);
    //QVector<QPair<double, double>> minMaxInput{};
    for(size_t j = 0; j < ui->prognosisTable->rowCount(); j++){
        for(size_t i = 0; i < inputSize; i++){
            testData.setValue(i, j, ui->prognosisTable->item(j, i)->text().toDouble());
        }
    }
    //QPair<double, double> minMaxOutput{};
    QVector<double> answers;
    for(size_t j = 0; j < ui->learnDataTable->rowCount(); j++){
        for(size_t i = inputSize; i < (inputSize+outputSize); i++){
            answers.append(ui->learnDataTable->item(j, i)->text().toDouble());
        }
    }

    for(size_t j = 0; j < ui->prognosisTable->rowCount(); j++){
        auto data = testData.getLine(j);
        normalizeExample(data);
        NN->forwardPropogation(data);
        auto ans = NN->getRes();
        denormalizeAns(ans);
        for(size_t i = inputSize; i < (inputSize+outputSize); i++){
            QTableWidgetItem *neuroAnswer = new QTableWidgetItem(QString::number(ans[i-(inputSize)]));
            ui->prognosisTable->setItem(j, i, neuroAnswer);
        }
    }
}


void MainWindow::on_loadLearnData_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Открыть файл с обучающим датасетом",
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    "Файлы csv (*.csv)");
    if (filename.isEmpty()) {
        return;
    }

    CSVProcessor csvProc;
    QString data = csvProc.readCSVFile(filename);

    if(csvProc.getHeader() != "\0"){
        AdaptLearnDataDialog* adapt = new AdaptLearnDataDialog(csvProc.getHeader(), this);
        adapt->exec();
        if(adapt->getChangeConfig()){
            // processing header
            QString header = csvProc.getHeader();
            auto headerItems = header.split(";");
            size_t inputCount = 0, outputCount = 0;
            for(auto item : headerItems){
                if(item[0]=='X')
                    inputCount++;
                if(item[0]=='D')
                    outputCount++;
            }
            ui->neuroAmountInput->setValue(inputCount);
            ui->neuroAmountOutput->setValue(outputCount);
            on_neuroAmountInput_valueChanged(inputCount);
            on_neuroAmountOutput_valueChanged(outputCount);
        }else{
            if(ui->neuroAmountInput->value()+ui->neuroAmountOutput->value() != (csvProc.getHeader().size()+1)/2){ //string: 9;9;9
                QMessageBox* message = new QMessageBox(this);
                message->setText("Произошла ошибка!\nДанные не подходят для вашей нейросети.");
                message->setStyleSheet("font-family:\"Garamond\"; font-size:11pt;");
                message->exec();
                delete message;
                return;
            }
        }
        delete adapt;
    }

    auto dataParsed = csvProc.parseFromCSV(data);
    ui->learnDataTable->setRowCount(dataParsed.getHeight());
    for(size_t i = 0; i < dataParsed.getHeight(); i++){
        for(size_t j = 0; j < dataParsed.getWidth(); j++){
            QTableWidgetItem *readedItem = new QTableWidgetItem(QString::number(dataParsed.getValue(j, i)));
            ui->learnDataTable->setItem(i, j, readedItem);
        }
    }
}


void MainWindow::on_loadPrognosisData_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Открыть файл с датасетом для прогнозов",
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    "Файлы csv (*.csv)");
    if (filename.isEmpty()) {
        return;
    }

    CSVProcessor csvProc;
    QString data = csvProc.readCSVFile(filename);

    auto dataParsed = csvProc.parseFromCSV(data);
    ui->prognosisTable->setRowCount(dataParsed.getHeight());
    for(size_t i = 0; i < dataParsed.getHeight(); i++){
        for(size_t j = 0; j < dataParsed.getWidth(); j++){
            QTableWidgetItem *readedItem = new QTableWidgetItem(QString::number(dataParsed.getValue(j, i)));
            ui->prognosisTable->setItem(i, j, readedItem);
        }
    }
}


void MainWindow::on_saveLearnData_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Сохранить файл с датасетом для обучения",
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/data.csv",
                                                    "Файлы csv (*.csv)");
    if (filename.isEmpty()) {
        return;
    }
    if (!filename.endsWith(".csv", Qt::CaseInsensitive)) {
        filename += ".csv";
    }


    TwoDimVector<double> learnData(ui->learnDataTable->columnCount(), ui->learnDataTable->rowCount(), 0);
    for(size_t j = 0; j < learnData.getHeight(); j++){
        for(size_t i = 0; i < learnData.getWidth(); i++){
            learnData.setValue(i, j, ui->learnDataTable->item(j, i)->text().toDouble());
        }
    }

    CSVProcessor* csvProc = new CSVProcessor;
    QString data = csvProc->parseToCSV(learnData);
    if(!csvProc->writeCSVFile(data, filename)){
        QMessageBox* message = new QMessageBox(this);
        message->setText("Проищошла ошибка!\nДанные не были сохранены.");
        message->setStyleSheet("font-family:\"Garamond\"; font-size:11pt;");
        message->exec();
        delete message;
        return;
    }
    delete csvProc;
}


void MainWindow::on_savePrognosisData_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Сохранить файл с датасетом для прогнозов",
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/data.csv",
                                                    "Файлы csv (*.csv)");
    if (filename.isEmpty()) {
        return;
    }


    TwoDimVector<double> prognData(inputSize, ui->prognosisTable->rowCount(), 0);
    for(size_t j = 0; j < prognData.getHeight(); j++){
        for(size_t i = 0; i < inputSize; i++){
            prognData.setValue(i, j, ui->prognosisTable->item(j, i)->text().toDouble());
        }
    }

    CSVProcessor* csvProc = new CSVProcessor;
    QString data = csvProc->parseToCSV(prognData);
    if(!csvProc->writeCSVFile(data, filename)){
        QMessageBox* message = new QMessageBox(this);
        message->setText("Проищошла ошибка!\nДанные не были сохранены.");
        message->setStyleSheet("font-family:\"Garamond\"; font-size:11pt;");
        message->exec();
        delete message;
        return;
    }
    delete csvProc;
}

void MainWindow::addHiddenNode(QWidget *layer){
    int numLayer = 0;
    for(auto item : hiddenLayersConfig){
        numLayer ++;
        if(layer==item){
            break;
        }
    }
    ui->neuroGraphicsView->addNode(numLayer);
}

void MainWindow::removeHiddenNode(QWidget *layer){
    int numLayer = 0;
    for(auto item : hiddenLayersConfig){
        numLayer ++;
        if(layer==item){
            break;
        }
    }
    ui->neuroGraphicsView->removeNode(numLayer);
}

