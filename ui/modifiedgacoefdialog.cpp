#include "modifiedgacoefdialog.h"
#include "ui_modifiedgacoefdialog.h"
#include "./modifgacoefs.h"

ModifiedGACoefDialog::ModifiedGACoefDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModifiedGACoefDialog)
{
    ui->setupUi(this);
    this->parent = parent;
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

void ModifiedGACoefDialog::on_buttonBox_accepted()
{
    if (auto widget = dynamic_cast<ModifGACoefs*>(parent)){
        widget->setPopSize(this->ui->popSize->value());
        widget->setMutationProb(this->ui->pMute->value());
        widget->setCrossoverProb(this->ui->pCross->value());
        widget->setCrossType(static_cast<Crossover>(this->ui->crossoverType->currentIndex()));
        widget->setMutStrength(static_cast<MutationStrength>(this->ui->mutStrength->currentIndex()));
        widget->setSelType(static_cast<Selection>(this->ui->selection->currentIndex()));
        widget->setWorkers(this->ui->threadAmount->value());
    }
}

