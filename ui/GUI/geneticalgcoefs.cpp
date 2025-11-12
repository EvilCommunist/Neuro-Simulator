#include "geneticalgcoefs.h"
#include "ui_geneticalgcoefs.h"

GeneticAlgCoefs::GeneticAlgCoefs(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::GeneticAlgCoefs)
{
    ui->setupUi(this);
}

GeneticAlgCoefs::~GeneticAlgCoefs()
{
    delete ui;
}
