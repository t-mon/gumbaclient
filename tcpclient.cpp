#include <QTcpSocket>
#include <QHostAddress>
#include <QMap>
#include <QVariantMap>

#include <qjson/parser.h>
#include <qjson/serializer.h>
#include "tcpclient.h"


TcpClient::TcpClient(QObject *parent) :
    QObject(parent)
{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(connected()),this, SLOT(connectedToHost()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(this,SIGNAL(newLineToParse(QString)),this,SLOT(parseLine(QString)));


    connect(this,SIGNAL(gumbaDataRecived(QString)),this,SLOT(readGumbaData(QString)));

}

void TcpClient::connectToHost(QString ipAddress, QString port){

    emit writeToTerminal("connecting to " + ipAddress + " over port " + port);
    tcpSocket->connectToHost(QHostAddress(ipAddress), port.toInt());
}

void TcpClient::connectedToHost(){

    emit connectionStatusChanged(true);
    emit writeToTerminal("--->connected to server. OK");
}

void TcpClient::connectionError(QAbstractSocket::SocketError error)
{
    emit connectionStatusChanged(false);
    emit writeToTerminal("---> connection error" + error);
}

void TcpClient::disconnectFromHost(){
    tcpSocket->close();
    emit connectionStatusChanged(false);
    emit writeToTerminal("disconnected from server.");
}

void TcpClient::sendData(QString target, QString command){

    QVariantMap map;
    map.insert("target",target);
    map.insert("command",command);

    QJson::Serializer serializer;
    QByteArray dataOut = serializer.serialize(map);
    //qDebug() << "-->Send Data: " << dataOut;
    tcpSocket->write(dataOut+"\n");
    tcpSocket->flush();
}

void TcpClient::readData(){
    QByteArray dataIn;

    m_tcpBuffer.append(tcpSocket->readAll());
    while(!m_tcpBuffer.isEmpty()){
        int newLinePositionPackage = m_tcpBuffer.indexOf('\n') + 1;
        //qDebug() << "----> data to parse: " << m_tcpBuffer.left(newLinePositionPackage);

        emit newLineToParse(m_tcpBuffer.left(newLinePositionPackage));
        m_tcpBuffer = m_tcpBuffer.right((m_tcpBuffer.length() - newLinePositionPackage));
    }
    //qDebug() << "----> got message: " << dataIn;
}

void TcpClient::parseLine(const QString &line)
{
    QVariantMap map;
    QJson::Parser parser;
    map = parser.parse(line.toAscii()).toMap();
    if(map.value("target").toString() == "Terminal"){
        emit writeToTerminal(map.value("command").toString());
    }
    if(map.value("target").toString() == "GumbaData"){
        emit gumbaDataRecived(map.value("command").toString());
    }

}

void TcpClient::readGumbaData(QString gumbaString)
{
    //qDebug() << "gumbastring: " << gumbaString;
    if (gumbaString.at(0) == '{'){
        QVariantMap sensors;
        QJson::Parser parser;
        sensors = parser.parse(gumbaString.toAscii()).toMap();

        if((gumbaString.at(0) == '{') && (gumbaString.at(1) == '"') && (gumbaString.at(2) == 'e')){
            if(sensors.value("energy").toMap().value("batery").toInt() == 1){
                double ubat = sensors.value("energy").toMap().value("voltage").toDouble();
                double iMotorR = sensors.value("energy").toMap().value("motor_r").toDouble();
                double iMotorL = sensors.value("energy").toMap().value("motor_l").toDouble();
                double lightR = sensors.value("energy").toMap().value("light_r").toDouble();
                double lightL = sensors.value("energy").toMap().value("light_l").toDouble();

                //ubat = ubat/100;

                emit motorLeft(iMotorL);
                emit motorRight(iMotorR);
                emit lightLeft(lightL);
                emit lightRight(lightR);
                emit batteryVoltage(ubat);

            }
        }

        if((gumbaString.at(0) == '{') && (gumbaString.at(1) == '"') && (gumbaString.at(2) == 's')){
            double speedL = sensors.value("speed").toMap().value("left").toDouble();
            double speedR = sensors.value("speed").toMap().value("right").toDouble();

            emit speedLeft(speedL);
            emit speedRight(speedR);
        }

        if((gumbaString.at(0) == '{') && (gumbaString.at(1) == '"') && (gumbaString.at(2) == 'o')){
            if(sensors.value("obstacles").toMap().value("left").toInt() == 0){
                emit obstacleLeft(false);
                qDebug() << "Obstacle Left OFF";
            } if(sensors.value("obstacles").toMap().value("left").toInt() == 1){
                emit obstacleLeft(true);
                qDebug() << "Obstacle Left ON";
            } if(sensors.value("obstacles").toMap().value("right").toInt() == 0){
                emit obstacleRight(false);
                qDebug() << "Obstacle Right OFF";
            } if(sensors.value("obstacles").toMap().value("right").toInt() == 1){
                emit obstacleRight(true);
                qDebug() << "Obstacle Right ON";
            }
        }
        if((gumbaString.at(0) == '{') && (gumbaString.at(1) == '"') && (gumbaString.at(2) == 'b')){
            if(sensors.value("bumper").toMap().value("left").toInt() == 0){
                emit bumperLeftPressd(false);
                qDebug() << "Bumper Left OFF";
            } if(sensors.value("bumper").toMap().value("left").toInt() == 1){
                emit bumperLeftPressd(true);
                qDebug() << "Bumper Left ON";
            } if(sensors.value("bumper").toMap().value("right").toInt() == 0){
                emit bumperRightPressed(false);
                qDebug() << "Bumper Right OFF";
            } if(sensors.value("bumper").toMap().value("right").toInt() == 1){
                emit bumperRightPressed(true);
                qDebug() << "Bumper Right ON";
            }
        }
    }
    if(gumbaString.at(0) == '[' || gumbaString.at(0) == '#' ){
        emit writeToTerminal("          " + gumbaString);
    }
}
