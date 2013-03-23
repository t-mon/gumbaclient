#include "robotarm.h"
#include <math.h>

#define L1 100
#define L2 100
#define L3 100
#define L4 100

RobotArm::RobotArm(QObject *parent) :
    QObject(parent)
{

    homePosition();
    connect (this,SIGNAL(anglesUpdated()),this,SLOT(transformPositionToAngle()));

}

void RobotArm::homePosition()
{
    m_theta1 = 0;
    m_theta2 = 0;
    m_theta3 = 0;
    m_theta4 = 0;
    m_theta5 = 0;
    qDebug() << "Koordinat-transformation resettet to home position";
}

QMatrix4x4 RobotArm::transform05()
{
    QMatrix4x4 T01(cos(m_theta1), 0, sin(m_theta1), 0,
                   sin(m_theta1), 0, -cos(m_theta1), 0,
                   0, 1, 0, L1,
                   0, 0, 0, 1);
    QMatrix4x4 T12(cos(m_theta2+M_PI_2), -sin(m_theta2+M_PI_2), 0, L2*cos(m_theta2+M_PI_2),
                   sin(m_theta2+M_PI_2), cos(m_theta2+M_PI_2), 0, L2*sin(m_theta2+M_PI_2),
                   0, 0, 1, 0,
                   0, 0, 0, 1);
    QMatrix4x4 T23(cos(m_theta3), -sin(m_theta3), 0, L3*sin(m_theta3),
                   sin(m_theta3), cos(m_theta3), 0, L3*cos(m_theta3),
                   0, 0, 1, 0,
                   0, 0, 0, 1);
    QMatrix4x4 T34(cos(m_theta4), 0, sin(m_theta4), 0,
                   sin(m_theta4), 0, -cos(m_theta4), 0,
                   0, 1, 0, 0,
                   0, 0, 0, 1);
    QMatrix4x4 T45(cos(m_theta5-M_PI_2), 0, sin(m_theta5-M_PI_2), 0,
                   sin(m_theta5-M_PI_2), 0, -cos(m_theta5-M_PI_2), 0,
                   0, 1, 0, L4,
                   0, 0, 0, 1);

    return T01 * T12 * T23 * T34 * T45;
}

void RobotArm::transformPositionToAngle()
{
    QMatrix4x4 T05 = transform05();
    qDebug() << "Transformation-coordinatsystem 0 -> 5: " << T05;
}

void RobotArm::transformAngleToPosition()
{
}

void RobotArm::updateAngles(int theta1, int theta2, int theta3, int theta4, int theta5)
{
    m_theta1 = theta1;
    m_theta2 = theta2;
    m_theta3 = theta3;
    m_theta4 = theta4;
    m_theta5 = theta5;
    emit anglesUpdated();
}

