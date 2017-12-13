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
           ScrollArea.cpp \
    LayerBrowser.cpp \
    LayerBrowserItem.cpp \
    AddNewLayerDialog.cpp \
    DrawingInformation.cpp \
    misc/misc.cpp

HEADERS  += MainWindow.h \
            ColorPickerView.h \
            ScrollArea.h \
    LayerBrowser.h \
    LayerBrowserItem.h \
    AddNewLayerDialog.h \
    DrawingInformation.h \
    misc/misc.h

FORMS    += MainWindow.ui \
    AddNewLayerDialog.ui

RESOURCES += \
    resource/resource.qrc
