#ifndef FUNCTIONMAP_H
#define FUNCTIONMAP_H

#include <QMap>
#include "../kernel/math/activationFunctions.h"

inline QMap<QString, math_activate::ActivationFunc> uiKernelMap = {
    {"Сигмоида", math_activate::sigmoid},
    {"Линейная", math_activate::linear},
    {"ReLu", math_activate::reLu},
    {"Текучая ReLu", math_activate::leakyReLu},
    {"Тангенс-гиперболическая", math_activate::tanhHyp}
};

#endif // FUNCTIONMAP_H
