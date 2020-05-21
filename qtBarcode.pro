#-------------------------------------------------
#
# Project created by QtCreator 2020-05-20T20:00:36
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtBarcode
TEMPLATE = app

LIBS += -lpng -ljpeg

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    QRCode/QrCode.cpp \
    Barcode/functii.cpp \
    quirc/lib/decode.c \
    quirc/lib/identify.c \
    quirc/lib/quirc.c \
    quirc/lib/version_db.c

HEADERS += \
        mainwindow.h \
    QRCode/QrCode.hpp \
    QRCode/QrCodeGeneratorDemo.h \
    Barcode/functii.h \
    Barcode/scanner.h \
    quirc/lib/quirc.h \
    quirc/lib/quirc_internal.h \
    quirc/tests/dbgutil.h \
    quirc/tests/inspect.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
