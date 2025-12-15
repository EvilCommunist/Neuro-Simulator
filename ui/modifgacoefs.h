#ifndef MODIFGACOEFS_H
#define MODIFGACOEFS_H

#include <QFrame>
#include "../../kernel/math/heuristicsAlg/GeneticOperatorsEnum.h"

namespace Ui {
class ModifGACoefs;
}

class ModifGACoefs : public QFrame
{
    Q_OBJECT

public:
    explicit ModifGACoefs(QWidget *parent = nullptr);
    ~ModifGACoefs();

    inline int getPopSize() {return popSize;}
    inline double getMutationProb() {return mutProb;}
    inline double getCrossoverProb() {return crossProb;}
    inline Selection getSelType() {return selType;}
    inline MutationStrength getMutStrength() {return mutStrength;}
    inline Crossover getCrossType() {return crossType;}

    inline void setPopSize(int value) {this->popSize = value;}
    inline void setMutationProb(double value) {this->mutProb = value;}
    inline void setCrossoverProb(double value) {this->crossProb = value;}
    inline void setSelType(Selection type) {this->selType = type;}
    inline void setMutStrength(MutationStrength strength) {this->mutStrength = strength;}
    inline void setCrossType(Crossover type) {this->crossType = type;}

private slots:
    void on_OpenModifGAWindow_clicked();

private:
    int popSize;
    double mutProb;
    double crossProb;
    Selection selType;
    MutationStrength mutStrength;
    Crossover crossType;

    Ui::ModifGACoefs *ui;
};

#endif // MODIFGACOEFS_H
