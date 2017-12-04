#-------------------------------------------------
#
# Project created by QtCreator 2017-11-30T14:52:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoColorPicker
TEMPLATE = app


SOURCES += main.cpp\
           MainWindow.cpp \
           ColorPickerView.cpp \
           DrawLayer.cpp

HEADERS  += MainWindow.h \
            ColorPickerView.h \
            DrawLayer.h

FORMS    += MainWindow.ui
