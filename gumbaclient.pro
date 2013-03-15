#-------------------------------------------------
#
# Project created by QtCreator 2013-03-11T16:12:24
#
#-------------------------------------------------

QT       += core gui network multimedia

TARGET = gumbaclient
TEMPLATE = app

LIBS += -lqjson
#-L/home/timon/development/wiic/build/wiicpp/

#INCLUDEPATH += /usr/local/include/wiic/

SOURCES += main.cpp\
    mainwindow.cpp \
    core.cpp \
    tcpclient.cpp \
    wiimote.cpp

HEADERS  += mainwindow.h \
    core.h \
    tcpclient.h \
    wiimote.h

