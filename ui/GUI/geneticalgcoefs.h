#ifndef GENETICALGCOEFS_H
#define GENETICALGCOEFS_H

#include <QFrame>

namespace Ui {
class GeneticAlgCoefs;
}

class GeneticAlgCoefs : public QFrame
{
    Q_OBJECT

public:
    explicit GeneticAlgCoefs(QWidget *parent = nullptr);
    ~GeneticAlgCoefs();

private:
    Ui::GeneticAlgCoefs *ui;
};

#endif // GENETICALGCOEFS_H
