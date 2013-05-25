#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QThread>
#include "mainwindow.h"
#include "tcpclient.h"
#include "qwiimote.h"
#include "robotarm.h"
#include "robotvisualisation.h"
#include "gumbamovement.h"

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
    GumbaMovement * m_gumbaMove;
    QWiiMote *m_wiimote;
    QMutex m_mutex;


    float degreeToRadiant(float degree);

signals:
    void writeToTerminal(const QString &terminalString);

private slots:
    void startWiiProcess();
    void stopWiiProcess();
    void wiiProcessFinished();
    void wiiLoopFinised();
    
};

#endif // CORE_H
