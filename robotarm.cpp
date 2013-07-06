#include "robotarm.h"
#include <math.h>

#define L1 50
#define L2 100
#define L3 100
#define L4 50

RobotArm::RobotArm(QObject *parent) :
    QObject(parent)
{

    homePosition();
    connect (this,SIGNAL(anglesUpdated()),this,SLOT(transformPositionToAngle()));

}

void RobotArm::homePosition()
{
    //  HOME POSITION on -> 0,0,0,0,0
    //    __L3___ _L4_
    //    |
    //    |_L2
    //    |_L1
    //--------------
    //
    //       0         0         1       150
    // T05 = 0        -1         0         0
    //       1         0         0       150
    //       0         0         0         1
    ///////////////////////


    m_theta1 = 0;
    m_theta2 = 0;
    m_theta3 = 0;
    m_theta4 = 0;
    m_theta5 = 0;
    writeToTerminal("Koordinat-transformation resettet to home position");
}

QMatrix4x4 RobotArm::transform05()
{
    QMatrix4x4 T01(cos(m_theta1),                   0,                  sin(m_theta1),          0,
                   sin(m_theta1),                   0,                  -cos(m_theta1),         0,
                   0,                               1,                  0,                      L1,
                   0,                               0,                  0,                      1);

    QMatrix4x4 T12(cos(m_theta2+M_PI_2),   -sin(m_theta2+M_PI_2),       0,                      L2*cos(m_theta2+M_PI_2),
                   sin(m_theta2+M_PI_2),   cos(m_theta2+M_PI_2),        0,                      L2*sin(m_theta2+M_PI_2),
                   0,                               0,                  1,                      0,
                   0,                               0,                   0,                     1);

    QMatrix4x4 T23(cos(m_theta3-M_PI_2),   -sin(m_theta3-M_PI_2),       0,                      L3*cos(m_theta3-M_PI_2),
                   sin(m_theta3-M_PI_2),   cos(m_theta3-M_PI_2),        0,                      L3*sin(m_theta3-M_PI_2),
                   0,                               0,                  1,                      0,
                   0,                               0,                  0,                      1);

    QMatrix4x4 T34(cos(m_theta4+M_PI_2),            0,                  sin(m_theta4+M_PI_2),   0,
                   sin(m_theta4+M_PI_2),            0,                  -cos(m_theta4+M_PI_2),  0,
                   0,                               1,                  0,                      0,
                   0,                               0,                  0,                      1);

    QMatrix4x4 T45(cos(m_theta5),       -sin(m_theta5),                 0,                      0,
                   sin(m_theta5),       cos(m_theta5),                  0,                      0,
                   0,                   0,                              1,                      L4,
                   0,                   0,                              0,                      1);

    QMatrix4x4 T02=T01*T12;
    QMatrix4x4 T03=T01*T12*T23;
    QMatrix4x4 T04=T01*T12*T23*T04;
    QMatrix4x4 T05=T01*T12*T23*T34*T45;

    QVector3D P00(0,0,0);
    QVector3D P01(T01.column(3).toVector3D());
    QVector3D P02(T02.column(3).toVector3D());
    QVector3D P03(T03.column(3).toVector3D());
    QVector3D P04(T04.column(3).toVector3D());
    QVector3D P05(T05.column(3).toVector3D());

    QVector3D Z00(QVector3D(0,0,1));
    QVector3D Z01(T01.column(2).toVector3D());
    QVector3D Z02(T02.column(2).toVector3D());
    QVector3D Z03(T03.column(2).toVector3D());
    QVector3D Z04(T04.column(2).toVector3D());
    QVector3D Z05(T05.column(2).toVector3D());

    QVector3D Jv0(QVector3D::crossProduct(Z00,(P05-P00)));
    QVector3D Jv1(QVector3D::crossProduct(Z01,(P05-P01)));
    QVector3D Jv2(QVector3D::crossProduct(Z02,(P05-P02)));
    QVector3D Jv3(QVector3D::crossProduct(Z03,(P05-P03)));
    QVector3D Jv4(QVector3D::crossProduct(Z04,(P05-P04)));

    QGenericMatrix<5,6,float> jakobi;

    float x = T05.column(3).toVector3D().x();
    float y = T05.column(3).toVector3D().y();
    float z = T05.column(3).toVector3D().z();

    emit updateTcpPosition((float)((int)(x*1000))/1000,(float)((int)(y*1000))/1000,(float)((int)(z*1000))/1000);

    return T05;
}

void RobotArm::transformPositionToAngle()
{
    QMatrix4x4 T05 = transform05();
    qDebug() << "\nTransformation-coordinatsystem 0 -> 5: " << T05;
}

void RobotArm::transformAngleToPosition()
{
}

void RobotArm::updateAngles(float theta1, float theta2, float theta3, float theta4, float theta5)
{
    m_theta1 = theta1*M_PI/180;
    m_theta2 = theta2*M_PI/180;
    m_theta3 = theta3*M_PI/180;
    m_theta4 = theta4*M_PI/180;
    m_theta5 = theta5*M_PI/180;
    emit anglesUpdated();
}

