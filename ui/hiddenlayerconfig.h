#ifndef HIDDENLAYERCONFIG_H
#define HIDDENLAYERCONFIG_H

#include <QFrame>
#include "../kernel/math/activationFunctions.h"

namespace Ui {
class HiddenLayerConfig;
}

class HiddenLayerConfig : public QFrame
{
    Q_OBJECT

public:
    explicit HiddenLayerConfig(QWidget *parent = nullptr);
    void setNumber(size_t num);
    ~HiddenLayerConfig();

    // getters
    size_t getNeuronAmount();
    math_activate::ActivationFunc getActivationFunc();

private slots:
    void on_hiddenNeuronAmount_valueChanged(int arg1);

signals:
    void signalAddHiddenNode(QWidget* layer);
    void signalRemoveHiddenNode(QWidget* layer);

private:
    Ui::HiddenLayerConfig *ui;
    int curNodeVal;
};

#endif // HIDDENLAYERCONFIG_H
