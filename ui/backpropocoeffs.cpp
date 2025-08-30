#include "backpropocoeffs.h"
#include "ui_backpropocoeffs.h"

backPropoCoeffs::backPropoCoeffs(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::backPropoCoeffs)
{
    ui->setupUi(this);
}

backPropoCoeffs::~backPropoCoeffs()
{
    delete ui;
}

double backPropoCoeffs::getSpeedCoeff(){
    return ui->learnSpeed->value();
}
