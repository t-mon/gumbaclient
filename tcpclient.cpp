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

    connect(this,SIGNAL(gumbaDataRecived(QString)),this,SLOT(readGumbaData(QString)));

}

void TcpClient::connectToHost(QString ipAddress, QString port){

    qDebug() << "connecting to" << ipAddress << " over port " << port;
    emit writeToTerminal("connecting to " + ipAddress + " over port " + port);
    tcpSocket->connectToHost(QHostAddress(ipAddress), port.toInt());
}

void TcpClient::connectedToHost(){

    qDebug() << "-->connected to server. OK";
    emit connectionStatusChanged(true);
    emit writeToTerminal("-->connected to server. OK");
}

void TcpClient::connectionError(QAbstractSocket::SocketError error)
{
    qDebug() << "connection error" << error;
    emit connectionStatusChanged(false);
    emit writeToTerminal("-->connection error.");
}

void TcpClient::disconnectFromHost(){
    tcpSocket->close();
    qDebug() << "disconnected from server.";
    emit connectionStatusChanged(false);
    emit writeToTerminal("disconnected from server.");
}

void TcpClient::sendData(QString target, QString command){

    QVariantMap map;
    map.insert("target",target);
    map.insert("command",command);

    QJson::Serializer serializer;
    QByteArray dataOut = serializer.serialize(map);
    qDebug() << "-->Send Data: " << dataOut;
    tcpSocket->write(dataOut);
}

void TcpClient::readData(){
    QByteArray dataIn;

    dataIn = tcpSocket->readAll();

    QVariantMap map;
    QJson::Parser parser;
    map = parser.parse(dataIn).toMap();
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

                ubat = ubat/100;
//                if (ubat != 0){
//                    if(ubat > 5.8){
//                        ui->bateryLineEdit->setStyleSheet("background-color:green;");
//                    }else if (ubat <= 5.8){
//                        ui->bateryLineEdit->setStyleSheet("background-color:red;");
//                    }
//                    ui->bateryLineEdit->clear();
//                    ui->bateryLineEdit->setText(QString::number(ubat) +"V");
//                }

                emit motorLeft(iMotorL);
                emit motorRight(iMotorR);
                emit lightLeft(lightL);
                emit lightRight(lightR);
                emit batteryVoltage(ubat);

//                qDebug() << "Batery Voltage: " << ubat;
//                qDebug() << "Motor right: " << iMotorR;
//                qDebug() << "Motor left: " << iMotorL;
//                qDebug() << "Light right: " << lightR;
//                qDebug() << "Light left: " << lightL;
            }
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
    }
    if(gumbaString.at(0) == '[' || gumbaString.at(0) == '#' ){

        emit writeToTerminal("          " + gumbaString);
        qDebug() << "            " + gumbaString;
    }
}
