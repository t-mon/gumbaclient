#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QThread>
#include "mainwindow.h"
#include "tcpclient.h"
#include "qwiimote.h"
#include "robotarm.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);

private:
    MainWindow *m_mainwindow;
    TcpClient * m_client;
    QThread *m_wiiMoteThread;
    RobotArm *m_arm;

signals:
    
private slots:
    void startWiiProcess();
    void stopWiiProcess();
    void wiiProcessFinished();
    
};

#endif // CORE_H
