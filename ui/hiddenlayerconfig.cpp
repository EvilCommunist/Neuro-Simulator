#include "hiddenlayerconfig.h"
#include "ui_hiddenlayerconfig.h"

HiddenLayerConfig::HiddenLayerConfig(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::HiddenLayerConfig)
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
