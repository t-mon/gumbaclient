#include <QString>
#include <QDebug>
#include <math.h>

#include "gumbamovement.h"

#define FWD         1
#define BWD         2
#define STOP        3
#define ROTLEFT     4
#define ROTRIGHT    5

GumbaMovement::GumbaMovement(QObject *parent) :
    QObject(parent)
{
    m_state = STOP;
}

void GumbaMovement::parseJoystick(const float &angle, const float &magnitude)
{
    QString gumbaMovementCommand;
    int speedLeft;
    int speedRight;

    int speed = magnitude*100;
    if(speed > 100){
        speed = 100;
    }

    if(magnitude > 0.13){
        int upDown      = magnitude*100*cos(angle*M_PI/180);
        int leftRight   = magnitude*100*sin(angle*M_PI/180);

        // ROTATE ######################
        if(upDown <=10 && upDown >= -10){
            if(leftRight < 0){
                gumbaMovementCommand = "l:" + QString::number(speed) ;
            }
            if(leftRight > 0){
                gumbaMovementCommand = "r:" + QString::number(speed) ;
            }
        }else{
            // MOVE STRAIGHT ######################
            if(leftRight <=10 && leftRight >=-10){
                if(upDown < 0){
                    gumbaMovementCommand = "b:" + QString::number(speed) + ";" + QString::number(speed) ;

                }
                if(upDown > 0){
                    gumbaMovementCommand = "f:" + QString::number(speed) + ";" + QString::number(speed) ;
                }
            }else{
                // MOVE CURVE BACKWARDS ######################
                if(upDown < 0){
                    // Left CURVE ######################
                    if(leftRight < 0){
                        speedLeft = speed - abs(speed*leftRight/100);
                        speedRight = speed;
                        gumbaMovementCommand = "b:" + QString::number(speedLeft) + ";" + QString::number(speedRight) ;
                    }
                    // RIGHT CURVE ######################
                    if(leftRight > 0){
                        speedLeft = speed;
                        speedRight = speed - abs(speed*leftRight/100);;
                        gumbaMovementCommand = "b:" + QString::number(speedLeft) + ";" + QString::number(speedRight) ;
                    }
                }
                // MOVE CURVE FORWARDS ######################
                if(upDown > 0){
                    // Left CURVE ######################
                    if(leftRight < 0){
                        speedLeft = speed - abs(speed*leftRight/100);
                        speedRight = speed;
                        gumbaMovementCommand = "f:" + QString::number(speedLeft) + ";" + QString::number(speedRight) ;
                    }
                    // RIGHT CURVE ######################
                    if(leftRight > 0){
                        speedLeft = speed;
                        speedRight = speed - abs(speed*leftRight/100);;
                        gumbaMovementCommand = "f:" + QString::number(speedLeft) + ";" + QString::number(speedRight) ;
                    }
                }
            }
        }

        if(((angle >= 270) || (angle <= 90)) && (m_state != FWD)){
            //writeToTerminal("Robotmovement FWD");
            m_state = FWD;
        }
        if((angle < 270) && (angle > 90) && (m_state != BWD)){
            //writeToTerminal("Robotmovement BWD");
            m_state = BWD;
        }

        //qDebug() << gumbaMovementCommand;
        emit sendCommand("RoboterMovement",gumbaMovementCommand);

    }else{
        if(m_state != STOP){
            //writeToTerminal("Robotmovement STOP");
            gumbaMovementCommand = "s\n";
            //qDebug() << gumbaMovementCommand;
            emit sendCommand("RoboterMovement",gumbaMovementCommand);
            m_state = STOP;
        }
    }
}
