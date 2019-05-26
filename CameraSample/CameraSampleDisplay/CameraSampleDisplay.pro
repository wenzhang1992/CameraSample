#-------------------------------------------------
#
# Project created by QtCreator 2019-05-12T09:28:54
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CameraSampleDisplay
TEMPLATE = app
LIBS += -lwinmm
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    CycleQueue.cpp \
    showwidget.cpp \
    PerformanceTimer.cpp \
    ImageProcessWarpper.cpp

CONFIG(debug,debug|release){
    DESTDIR =$$PWD/../../xdebug
}

HEADERS += \
        mainwindow.h \
    ../CameraDevice/CameraDevice.h \
    CycleQueue.h \
    showwidget.h \
    PerformanceTimer.h \
    ../ImageProcessLib/ImageProcessLib.h \
    ImageProcessWarpper.h

FORMS += \
        mainwindow.ui

win32: LIBS += -L$$PWD/../../xdebug/ -lCameraDevice
win32: LIBS += -L$$PWD/../../xdebug/ -lImageProcessLib
INCLUDEPATH += $$PWD/../../xdebug
DEPENDPATH += $$PWD/../../xdebug
