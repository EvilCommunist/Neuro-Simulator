#include "modifgacoefs.h"
#include "ui_modifgacoefs.h"
#include "./modifiedgacoefdialog.h"

ModifGACoefs::ModifGACoefs(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::ModifGACoefs)
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

