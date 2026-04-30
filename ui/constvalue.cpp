#include "constvalue.h"
#include "ui_constvalue.h"

ConstValue::ConstValue(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::ConstValue)
{
    ui->setupUi(this);
}

ConstValue::~ConstValue()
{
    delete ui;
}

float ConstValue::getConstValue(){
    return static_cast<float>(this->ui->constValue->value());
}
