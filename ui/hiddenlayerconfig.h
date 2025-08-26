#ifndef HIDDENLAYERCONFIG_H
#define HIDDENLAYERCONFIG_H

#include <QFrame>

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

private:
    Ui::HiddenLayerConfig *ui;
};

#endif // HIDDENLAYERCONFIG_H
