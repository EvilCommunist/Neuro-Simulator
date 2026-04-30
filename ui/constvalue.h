#ifndef CONSTVALUE_H
#define CONSTVALUE_H

#include <QFrame>

namespace Ui {
class ConstValue;
}

class ConstValue : public QFrame
{
    Q_OBJECT

public:
    explicit ConstValue(QWidget *parent = nullptr);
    ~ConstValue();
    float getConstValue();

private:
    Ui::ConstValue *ui;
};

#endif // CONSTVALUE_H
