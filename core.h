#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "mainwindow.h"
#include "tcpclient.h"
#include "wiimote.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);

private:
    MainWindow *m_mainwindow;
    TcpClient * m_client;
    WiiMote *m_wiimote;
signals:
    
private slots:

    
};

#endif // CORE_H
