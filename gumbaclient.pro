#-------------------------------------------------
#
# Project created by QtCreator 2013-03-11T16:12:24
#
#-------------------------------------------------

QT       += core gui network

TARGET = gumbaclient
TEMPLATE = app

LIBS += -lqjson -L/home/timon/development/wiic/build/wiicpp/ -lwiicpp

INCLUDEPATH +=  /home/timon/development/wiic/src/wiicpp/ \
                /home/timon/development/wiic/src/wiic/ \
                /home/timon/development/wiic/src/log/ \
                /home/timon/development/wiic/src/ml/



SOURCES += main.cpp\
    mainwindow.cpp \
    core.cpp \
    tcpclient.cpp \
    qwiimote.cpp \
    robotarm.cpp

HEADERS  += mainwindow.h \
    core.h \
    tcpclient.h \
    qwiimote.h \
    robotarm.h

