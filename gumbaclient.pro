#-------------------------------------------------
#
# Project created by QtCreator 2013-03-11T16:12:24
#
#-------------------------------------------------

QT       += core gui network opengl

TARGET = gumbaclient
TEMPLATE = app

LIBS += -lqjson -L/usr/local/lib/ -lwiicpp -lGL -lGLU -lglut

INCLUDEPATH += /usr/local/include/wiic/


SOURCES += main.cpp\
    mainwindow.cpp \
    core.cpp \
    tcpclient.cpp \
    qwiimote.cpp \
    robotarm.cpp \
    robotvisualisation.cpp \
    gumbamovement.cpp

HEADERS  += mainwindow.h \
    core.h \
    tcpclient.h \
    qwiimote.h \
    robotarm.h \
    robotvisualisation.h \
    gumbamovement.h

