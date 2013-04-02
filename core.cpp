#include "core.h"
#include <math.h>

Core::Core(QObject *parent) :
    QObject(parent)
{
    m_mainwindow = new MainWindow();
    m_mainwindow->show();

    m_wiiMoteThread = new QThread(this);
    m_client = new TcpClient(this);

    m_arm = new RobotArm(this);
    m_arm->updateAngles(0,0,0,0,0);

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

    connect(m_mainwindow,SIGNAL(stopWiiMote()),this,SLOT(stopWiiProcess()));
    connect(m_mainwindow,SIGNAL(startWiiMote()),this,SLOT(startWiiProcess()));
    connect(m_wiiMoteThread,SIGNAL(finished()),this,SLOT(wiiProcessFinished()));
}

void Core::startWiiProcess()
{
    QWiiMote *m_wiimote = new QWiiMote();
    m_wiimote->moveToThread(m_wiiMoteThread);
    m_wiiMoteThread->start();

    connect(m_wiiMoteThread,SIGNAL(started()),m_wiimote,SLOT(startWiiMotesLoop()));
    connect(m_wiimote,SIGNAL(stopProcess()),this,SLOT(stopWiiProcess()));

    connect(m_wiimote,SIGNAL(writeToTerminal(QString)),m_mainwindow,SLOT(writeToTerminal(QString)));
    connect(m_wiimote,SIGNAL(orientationWiiMoteChanged(float,float,float)),m_mainwindow,SLOT(updateWiiMoteOrientation(float,float,float)));
    connect(m_wiimote,SIGNAL(orientationNunChuckChanged(float,float,float)),m_mainwindow,SLOT(updateNunchuckOrientation(float,float,float)));

    connect(m_wiimote,SIGNAL(nunchukJoystickChanged(float,float)),m_mainwindow,SLOT(updateNunchuckJoyStickData(float,float)));
    connect(m_wiimote,SIGNAL(button_AB_pressed(bool)),m_mainwindow,SLOT(wiiMoteABChanged(bool)));

}

void Core::stopWiiProcess()
{
    if(m_wiiMoteThread->isRunning()){
        qDebug() << "Wii process is running...";
        m_wiiMoteThread->quit();
        m_wiiMoteThread->terminate();
        m_wiiMoteThread->exit();
    }else{
        qDebug() << "Wii process is not running...";
    }
}

void Core::wiiProcessFinished()
{
    qDebug() << "Wii process killed...";
}

