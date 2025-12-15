#include "modifiedgacoefdialog.h"
#include "ui_modifiedgacoefdialog.h"

ModifiedGACoefDialog::ModifiedGACoefDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModifiedGACoefDialog)
{
    ui->setupUi(this);
}

ModifiedGACoefDialog::~ModifiedGACoefDialog()
{
    delete ui;
}
