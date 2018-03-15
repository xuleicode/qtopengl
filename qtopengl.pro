#-------------------------------------------------
#
# Project created by QtCreator 2018-03-14T17:23:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += opengl
#LIBS += glut.lib glut32.lib
TARGET = qtopengl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h

FORMS    += mainwindow.ui
