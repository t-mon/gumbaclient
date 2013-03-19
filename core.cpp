#include "core.h"

Core::Core(QObject *parent) :
    QObject(parent)
{
    m_mainwindow = new MainWindow();
    m_mainwindow->show();

    m_wiimote = new QWiiMote(this);

    m_client = new TcpClient(this);

    connect(m_mainwindow,SIGNAL(connectServer(QString,QString)),m_client,SLOT(connectToHost(QString,QString)));
    connect(m_mainwindow,SIGNAL(disconnectServer()),m_client,SLOT(disconnectFromHost()));
    connect(m_mainwindow,SIGNAL(sendCommand(QString,QString)),m_client,SLOT(sendData(QString,QString)));

    connect(m_client,SIGNAL(writeToTerminal(QString)),m_mainwindow,SLOT(writeToTerminal(QString)));
    connect(m_client,SIGNAL(connectionStatusChanged(bool)),m_mainwindow,SLOT(connectionStatusChanged(bool)));

    connect(m_client,SIGNAL(bumperLeftPressd(bool)),m_mainwindow,SLOT(bumperLeftPressed(bool)));
    connect(m_client,SIGNAL(bumperRightPressed(bool)),m_mainwindow,SLOT(bumperRightPressed(bool)));
    connect(m_client,SIGNAL(obstacleLeft(bool)),m_mainwindow,SLOT(obstacleLeft(bool)));
    connect(m_client,SIGNAL(obstacleRight(bool)),m_mainwindow,SLOT(obstacleRight(bool)));

    connect(m_client,SIGNAL(motorLeft(int)),m_mainwindow,SLOT(motorLeft(int)));
    connect(m_client,SIGNAL(motorRight(int)),m_mainwindow,SLOT(motorRight(int)));
    connect(m_client,SIGNAL(lightLeft(int)),m_mainwindow,SLOT(lightLeft(int)));
    connect(m_client,SIGNAL(lightRight(int)),m_mainwindow,SLOT(lightRight(int)));

    connect(m_client,SIGNAL(batteryVoltage(double)),m_mainwindow,SLOT(batteryStatus(double)));

}
