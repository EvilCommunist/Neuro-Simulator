#include "modifgacoefs.h"
#include "ui_modifgacoefs.h"
#include "./modifiedgacoefdialog.h"

ModifGACoefs::ModifGACoefs(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::ModifGACoefs),
    popSize(200),
    mutProb(0.3),
    crossProb(0.7),
    selType(TOURNAMENT),
    mutStrength(WEAK),
    crossType(ARITHMETICAL),
    workers(4) // initial hard-coded values
{
    ui->setupUi(this);
}

ModifGACoefs::~ModifGACoefs()
{
    delete ui;
}

void ModifGACoefs::on_OpenModifGAWindow_clicked()
{
    ModifiedGACoefDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
    // TODO
    }
}

