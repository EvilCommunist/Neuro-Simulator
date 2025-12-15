#include "modifiedgacoefdialog.h"
#include "ui_modifiedgacoefdialog.h"
#include "./modifgacoefs.h"

ModifiedGACoefDialog::ModifiedGACoefDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModifiedGACoefDialog)
{
    ui->setupUi(this);

    if (auto widget = dynamic_cast<ModifGACoefs*>(parent)){
        this->ui->popSize->setValue(widget->getPopSize());
        this->ui->pMute->setValue(widget->getMutationProb());
        this->ui->pCross->setValue(widget->getCrossoverProb());
        this->ui->crossoverType->setCurrentIndex(widget->getCrossType());
        this->ui->mutStrength->setCurrentIndex(widget->getMutStrength());
        this->ui->selection->setCurrentIndex(widget->getSelType());
        this->ui->threadAmount->setValue(widget->getWorkers());
    }
}

ModifiedGACoefDialog::~ModifiedGACoefDialog()
{
    delete ui;
}
