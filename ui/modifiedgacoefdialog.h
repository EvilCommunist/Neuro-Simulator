#ifndef MODIFIEDGACOEFDIALOG_H
#define MODIFIEDGACOEFDIALOG_H

#include <QDialog>

namespace Ui {
class ModifiedGACoefDialog;
}

class ModifiedGACoefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifiedGACoefDialog(QWidget *parent = nullptr);
    ~ModifiedGACoefDialog();

private slots:
    void on_buttonBox_accepted();

private:
    QWidget *parent;
    Ui::ModifiedGACoefDialog *ui;
};

#endif // MODIFIEDGACOEFDIALOG_H
