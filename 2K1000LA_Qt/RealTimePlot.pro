QT       += core gui
QT       += serialport
QT += widgets printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

DESTDIR = ../APP
TARGET = RealTimePlot

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base64.cpp \
    dataparser.cpp \
    hmac_sha1.cpp \
    lora.cpp \
    main.cpp \
    mainwindow.cpp \
    mqttkit.cpp \
    obsbuffer.cpp \
    onenet.cpp \
    qcustomplot.cpp \
    serialport.cpp \
    widgetplot2d.cpp \
    wifi.cpp

HEADERS += \
    base64.h \
    common.h \
    dataparser.h \
    datatypedef.h \
    hmac_sha1.h \
    lora.h \
    mainwindow.h \
    mqttkit.h \
    obsbuffer.h \
    onenet.h \
    qcustomplot.h \
    serialport.h \
    widgetplot2d.h \
    wifi.h

FORMS += \
    mainwindow.ui \
    widgetplot2d.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc \
    image.qrc \
    image.qrc

DISTFILES += \
    RealTimePlot.pro.user \
    RealTimePlot.pro.user.1e58c83.4.9-pre1 \
    RealTimePlot.pro.user.392bdac.22
