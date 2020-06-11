#-------------------------------------------------
#
# Project created by QtCreator 2020-04-29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Editor
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS



SOURCES += \
        main.cpp \
        mainwindow.cpp \
    codeeditor.cpp \
    highlighter.cpp \
    completelistwidget.cpp \
    console.cpp

HEADERS += \
        mainwindow.h \
    codeeditor.h \
    highlighter.h \
    completelistwidget.h \
    console.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    image.qrc

ICON = icon.icns
