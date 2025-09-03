#include "adaptlearndatadialog.h"
#include "ui_adaptlearndatadialog.h"

AdaptLearnDataDialog::AdaptLearnDataDialog(const QString& headerData, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AdaptLearnDataDialog)
    , changeConfig(false)
{
    ui->setupUi(this);
    ui->infoArea->setPlainText(QString("ВНИМАНИЕ: В исходном файле обнаружен заголовок с информацией о количестве входных и выходных нейронах:\n"
                                       "%1\n"
                                       "Изменить конфигурацию нейросети, для соответствия данным в файле?\n\n"
                                       "*Если отклонить изменения, данные будут считаны, при условии, что сеть сопоставима по общему числу входов/"
                                       "выходов с количеством столбцов в файле.")
                                   .arg(headerData));
}

AdaptLearnDataDialog::~AdaptLearnDataDialog()
{
    delete ui;
}

void AdaptLearnDataDialog::on_buttonBox_accepted()
{
    changeConfig = true;
}

