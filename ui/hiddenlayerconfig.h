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

private:
    Ui::HiddenLayerConfig *ui;
};

#endif // HIDDENLAYERCONFIG_H
