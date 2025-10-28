#include "hiddenlayerconfig.h"
#include "ui_hiddenlayerconfig.h"
#include "./functionMap.h"

HiddenLayerConfig::HiddenLayerConfig(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::HiddenLayerConfig)
    , curNodeVal(1)
{
    ui->setupUi(this);
}

void HiddenLayerConfig::setNumber(size_t num){
    auto prefix = ui->label->text();
    ui->label->setText(prefix + QString::number(num));
}

HiddenLayerConfig::~HiddenLayerConfig()
{
    delete ui;
}

size_t HiddenLayerConfig::getNeuronAmount(){
    return ui->hiddenNeuronAmount->text().toUInt();
}

math_activate::ActivationFunc HiddenLayerConfig::getActivationFunc(){
    return  uiKernelMap[ui->hiddenActivFunc->currentText()];
}

void HiddenLayerConfig::on_hiddenNeuronAmount_valueChanged(int arg1)
{
    if(curNodeVal < arg1){   // send signal of change
        emit signalAddHiddenNode(this);
    } else {
        emit signalRemoveHiddenNode(this);
    }
    curNodeVal = arg1;
}

