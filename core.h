#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "mainwindow.h"
#include "tcpclient.h"
#include "qwiimote.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);

private:
    MainWindow *m_mainwindow;
    TcpClient * m_client;
    QWiiMote *m_wiimote;
signals:
    
private slots:

    
};

#endif // CORE_H
