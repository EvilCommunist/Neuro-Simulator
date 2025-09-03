#ifndef ADAPTLEARNDATADIALOG_H
#define ADAPTLEARNDATADIALOG_H

#include <QDialog>

namespace Ui {
class AdaptLearnDataDialog;
}

class AdaptLearnDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdaptLearnDataDialog(const QString& headerData, QWidget *parent = nullptr);
    ~AdaptLearnDataDialog();

    inline getChangeConfig() { return changeConfig; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AdaptLearnDataDialog *ui;
    bool changeConfig;
};

#endif // ADAPTLEARNDATADIALOG_H
