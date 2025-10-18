QT       += core gui
QT       += charts
QT       += charts quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    kernel/files/csvprocessor.cpp \
    kernel/math/activationFunctions.cpp \
    kernel/math/normalization.cpp \
    main.cpp \
    mainwindow.cpp \
    kernel/neuro.cpp \
    ui/GUI/neuroview.cpp \
    ui/adaptlearndatadialog.cpp \
    ui/backpropocoeffs.cpp \
    ui/chartprocessor.cpp \
    ui/hiddenlayerconfig.cpp

HEADERS += \
    kernel/files/csvprocessor.h \
    kernel/math/activationFunctions.h \
    kernel/math/normalization.h \
    kernel/twodimvector.h \
    mainwindow.h \
    kernel/neuro.h \
    kernel/threedimvector.h \
    ui/GUI/neuroview.h \
    ui/adaptlearndatadialog.h \
    ui/backpropocoeffs.h \
    ui/chartprocessor.h \
    ui/enums.h \
    ui/functionMap.h \
    ui/hiddenlayerconfig.h

FORMS += \
    mainwindow.ui \
    ui/adaptlearndatadialog.ui \
    ui/backpropocoeffs.ui \
    ui/hiddenlayerconfig.ui

TRANSLATIONS += \
    neuro-simulator_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
