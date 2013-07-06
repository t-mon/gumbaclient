#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);

signals:
    void newLineToParse(const QString &line);

    void obstacleLeft(const bool &obstacleState);
    void obstacleRight(const bool &obstacleState);
    void bumperLeftPressd(const bool &bumperState);
    void bumperRightPressed(const bool &bumperState);

    void motorLeft(const int &motorCurrent);
    void motorRight(const int &motorCurrent);
    void speedLeft(const double &speedLeft);
    void speedRight(const double &speedRight);
    void lightLeft(const int &lightIntensity);
    void lightRight(const int &lightIntensity);

    void batteryVoltage(const double &batteryVoltage);

    void writeToTerminal(const QString &terminalString);
    void gumbaDataRecived(const QString &sensorString);
    void connectionStatusChanged(const bool &connectionState);


private:
    QTcpSocket *tcpSocket;
    QString m_tcpBuffer;

private slots:
    void connectedToHost();
    void connectionError(QAbstractSocket::SocketError error);
    void readData();
    void parseLine(const QString &line);
    void readGumbaData(QString sensorString);

public slots:
    void connectToHost(QString ipAddress, QString port);
    void disconnectFromHost();
    void sendData(QString target, QString command);
};


#endif // TCPCLIENT_H
