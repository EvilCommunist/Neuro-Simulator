#ifndef BACKPROPOCOEFFS_H
#define BACKPROPOCOEFFS_H

#include <QFrame>

namespace Ui {
class backPropoCoeffs;
}

class backPropoCoeffs : public QFrame
{
    Q_OBJECT

public:
    explicit backPropoCoeffs(QWidget *parent = nullptr);
    ~backPropoCoeffs();

    double getSpeedCoeff();

private:
    Ui::backPropoCoeffs *ui;
};

#endif // BACKPROPOCOEFFS_H
