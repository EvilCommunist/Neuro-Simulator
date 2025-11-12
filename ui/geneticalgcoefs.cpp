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


int GeneticAlgCoefs::getPopSize(){
    return ui->popSize->value();
}

double GeneticAlgCoefs::getCrossoverProb(){
    return ui->pCrossover->value();
}

double GeneticAlgCoefs::getMutationProb(){
    return ui->pMutation->value();
}
