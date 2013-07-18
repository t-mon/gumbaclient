#include "robotarm.h"
#include <math.h>
#include <octave/oct.h>

#define L1 100
#define L2 200
#define L3 200
#define L4 100

RobotArm::RobotArm(QObject *parent) :
    QObject(parent)
{

    homePosition();
    connect (this,SIGNAL(anglesUpdated()),this,SLOT(calculateCurrentPosition()));
    connect (this,SIGNAL(updateTcpPosition(float,float,float,float,float,float)),this,SLOT(transformPositionToAngle(float,float,float,float,float,float)));
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


    m_theta1 = M_PI_2;
    m_theta2 = M_PI_2;
    m_theta3 = M_PI_2;
    m_theta4 = M_PI_2;
    m_theta5 = M_PI_2;
    writeToTerminal("Koordinat-transformation resettet to home position");
}


QMatrix4x4 RobotArm::cutMatrixValues(QMatrix4x4 matrix)
{
    float xx = matrix.column(0).toVector3D().x();
    float xy = matrix.column(0).toVector3D().y();
    float xz = matrix.column(0).toVector3D().z();

    float yx = matrix.column(1).toVector3D().x();
    float yy = matrix.column(1).toVector3D().y();
    float yz = matrix.column(1).toVector3D().z();

    float zx = matrix.column(2).toVector3D().x();
    float zy = matrix.column(2).toVector3D().y();
    float zz = matrix.column(2).toVector3D().z();

    float dx = matrix.column(3).toVector3D().x();
    float dy = matrix.column(3).toVector3D().y();
    float dz = matrix.column(3).toVector3D().z();

    QMatrix4x4 normMatrix((float)((int)(xx*1000))/1000, (float)((int)(yx*1000))/1000, (float)((int)(zx*1000))/1000, (float)((int)(dx*1000))/1000,
                          (float)((int)(xy*1000))/1000, (float)((int)(yy*1000))/1000, (float)((int)(zy*1000))/1000, (float)((int)(dy*1000))/1000,
                          (float)((int)(xz*1000))/1000, (float)((int)(yz*1000))/1000, (float)((int)(zz*1000))/1000, (float)((int)(dz*1000))/1000,
                          0,                            0,                            0,                            1 );
    return normMatrix;
}

void RobotArm::calculateJakobiInv()
{
    float init = 0;
    Matrix jakobi(6,5,init);

    for(int n = 0; n < 6; n++){
        for(int m = 0; m < 5; m++){
            jakobi(n,m) = m_jakobi(n,m);
        }
    }

    Matrix jakobiInv(5,6,init);
    jakobiInv = jakobi.pseudo_inverse();


    for(int n = 0; n < 5; n++){
        for(int m = 0; m < 6; m++){
           m_jakobiInv(n,m) = jakobiInv(n,m);
        }
    }

    //qDebug() << m_jakobiInv*m_jakobi;

}


// Forward transformation Denavit-Hartenberg
void RobotArm::calculateCurrentPosition()
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
    QMatrix4x4 T04=T01*T12*T23*T34;
    QMatrix4x4 T05=T01*T12*T23*T34*T45;

    qDebug() << "\nTransformation-coordinatsystem 0 -> 5: " << cutMatrixValues(T05);

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
    jakobi.fill(0);

    jakobi(0,0) = Jv0.x();
    jakobi(1,0) = Jv0.y();
    jakobi(2,0) = Jv0.z();

    jakobi(0,1) = Jv1.x();
    jakobi(1,1) = Jv1.y();
    jakobi(2,1) = Jv1.z();

    jakobi(0,2) = Jv2.x();
    jakobi(1,2) = Jv2.y();
    jakobi(2,2) = Jv2.z();

    jakobi(0,3) = Jv3.x();
    jakobi(1,3) = Jv3.y();
    jakobi(2,3) = Jv3.z();

    jakobi(0,4) = Jv4.x();
    jakobi(1,4) = Jv4.y();
    jakobi(2,4) = Jv4.z();

    jakobi(3,0) = Z00.x();
    jakobi(4,0) = Z00.x();
    jakobi(5,0) = Z00.z();

    jakobi(3,1) = Z01.x();
    jakobi(4,1) = Z01.x();
    jakobi(5,1) = Z01.z();

    jakobi(3,2) = Z02.x();
    jakobi(4,2) = Z02.x();
    jakobi(5,2) = Z02.z();

    jakobi(3,3) = Z03.x();
    jakobi(4,3) = Z03.x();
    jakobi(5,3) = Z03.z();

    jakobi(3,4) = Z04.x();
    jakobi(4,4) = Z04.x();
    jakobi(5,4) = Z04.z();


    m_jakobi = jakobi;
    qDebug() << m_jakobi;
    calculateJakobiInv();


    float x = T05.column(3).toVector3D().x();
    float y = T05.column(3).toVector3D().y();
    float z = T05.column(3).toVector3D().z();

    float wy = atan2(-T05(2,0),sqrt(pow(T05(0,0),2) + pow(T05(1,0),2)));
    float wz = atan2(T05(1,0)/cos(wy), T05(0,0)/cos(wy));
    float wx = atan2(T05(2,1)/cos(wy), T05(2,2)/cos(wy));

    m_currentTcpPose(0,0) = (float)((int)(x*100))/100;
    m_currentTcpPose(1,0) = (float)((int)(y*100))/100;
    m_currentTcpPose(2,0) = (float)((int)(z*100))/100;
    m_currentTcpPose(3,0) = (float)((int)(wx*100))/100;
    m_currentTcpPose(4,0) = (float)((int)(wy*100))/100;
    m_currentTcpPose(5,0) = (float)((int)(wz*100))/100;

    //qDebug() << "current tcpPose" << m_currentTcpPose;

    emit updateTcpPosition(m_currentTcpPose(0,0), m_currentTcpPose(1,0), m_currentTcpPose(2,0), m_currentTcpPose(3,0), m_currentTcpPose(4,0), m_currentTcpPose(5,0));
}

void RobotArm::transformPositionToAngle(float x, float y, float z, float wx, float wy, float wz)
{
    QGenericMatrix<1,6,float> wantedTcpPose;
    wantedTcpPose(0,0)=x;
    wantedTcpPose(1,0)=y;
    wantedTcpPose(2,0)=z;
    wantedTcpPose(3,0)=wx;
    wantedTcpPose(4,0)=wy;
    wantedTcpPose(5,0)=wz;

    QGenericMatrix<1,5,float> searchedAngles = m_jakobiInv*wantedTcpPose;

    qDebug()<< "searched angles" << searchedAngles << "for tcp pose" << wantedTcpPose;

}

void RobotArm::transformAngleToPosition(float theta1, float theta2, float theta3, float theta4, float theta5)
{

}

void RobotArm::updateAngles(float theta1, float theta2, float theta3, float theta4, float theta5)
{
    m_theta1 = theta1-M_PI_2;
    m_theta2 = theta2-M_PI_2;
    m_theta3 = theta3-M_PI_2;
    m_theta4 = theta4-M_PI_2;
    m_theta5 = theta5-M_PI_2;

    m_currentAngles(0,0) = m_theta1;
    m_currentAngles(1,0) = m_theta2;
    m_currentAngles(2,0) = m_theta3;
    m_currentAngles(3,0) = m_theta4;
    m_currentAngles(4,0) = m_theta5;

    qDebug() << "current angles" << m_currentAngles;
    emit anglesUpdated();
}

