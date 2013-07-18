#-------------------------------------------------
#
# Project created by QtCreator 2013-03-11T16:12:24
#
#-------------------------------------------------

QT       += core gui network opengl

TARGET = gumbaclient
TEMPLATE = app

LIBS += -lqjson -L/usr/local/lib/ -lwiicpp -lGL -lGLU -lglut -L/usr/lib/octave/3.6.4/ -loctave

INCLUDEPATH += /usr/local/include/wiic/
INCLUDEPATH += /usr/include/octave-3.6.4/


SOURCES += main.cpp\
    mainwindow.cpp \
    core.cpp \
    tcpclient.cpp \
    qwiimote.cpp \
    robotarm.cpp \
    robotvisualisation.cpp \
    gumbamovement.cpp \
    gumbabehavior.cpp

HEADERS  += mainwindow.h \
    core.h \
    tcpclient.h \
    qwiimote.h \
    robotarm.h \
    robotvisualisation.h \
    gumbamovement.h \
    gumbabehavior.h

