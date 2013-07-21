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
    Matrix jakobi(6,6,init);

    for(int n = 0; n < 6; n++){
        for(int m = 0; m < 6; m++){
            jakobi(n,m) = m_jakobi(n,m);
        }
    }

    Matrix jakobiInv(6,6,init);
    jakobiInv = jakobi.inverse();

    for(int n = 0; n < 6; n++){
        for(int m = 0; m < 6; m++){
           m_jakobiInv(n,m) = jakobiInv(n,m);
        }
    }
}


// Forward transformation Denavit-Hartenberg
void RobotArm::calculateCurrentPosition()
{
    float a1 = 0;
    float a2 = L2;
    float a3 = 0;
    float a4 = 0;
    float a5 = 0;
    float a6 = 0;

    float alpha1 = M_PI_2;
    float alpha2 = 0;
    float alpha3 = M_PI_2;
    float alpha4 = M_PI_2;
    float alpha5 = M_PI_2;
    float alpha6 = 0;

    float d1 = L1;
    float d2 = 0;
    float d3 = 0;
    float d4 = L3;
    float d5 = 0;
    float d6 = L4;

    float theta1 = m_theta1;
    float theta2 = m_theta2+M_PI_2;
    float theta3 = m_theta3;
    float theta4 = m_theta4 + M_PI;
    float theta5 = m_theta5 + M_PI;
    float theta6 = m_theta6;

    QMatrix4x4 T01(cos(theta1),  -sin(theta1)*cos(alpha1),   sin(theta1)*sin(alpha1),   a1*cos(theta1),
                   sin(theta1),   cos(theta1)*cos(alpha1),  -cos(theta1)*sin(alpha1),   a1*sin(theta1),
                   0,             sin(alpha1),               cos(alpha1),               d1,
                   0,             0,                         0,                         1);
    QMatrix4x4 T12(cos(theta2),  -sin(theta2)*cos(alpha2),   sin(theta2)*sin(alpha2),   a2*cos(theta2),
                   sin(theta2),   cos(theta2)*cos(alpha2),  -cos(theta2)*sin(alpha2),   a2*sin(theta2),
                   0,             sin(alpha2),               cos(alpha2),               d2,
                   0,             0,                         0,                         1);
    QMatrix4x4 T23(cos(theta3),  -sin(theta3)*cos(alpha3),   sin(theta3)*sin(alpha3),   a3*cos(theta3),
                   sin(theta3),   cos(theta3)*cos(alpha3),  -cos(theta3)*sin(alpha3),   a3*sin(theta3),
                   0,             sin(alpha3),               cos(alpha3),               d3,
                   0,             0,                         0,                         1);
    QMatrix4x4 T34(cos(theta4),  -sin(theta4)*cos(alpha4),   sin(theta4)*sin(alpha4),   a4*cos(theta4),
                   sin(theta4),   cos(theta4)*cos(alpha4),  -cos(theta4)*sin(alpha4),   a4*sin(theta4),
                   0,             sin(alpha4),               cos(alpha4),               d4,
                   0,             0,                         0,                         1);
    QMatrix4x4 T45(cos(theta5),  -sin(theta5)*cos(alpha5),   sin(theta5)*sin(alpha5),   a5*cos(theta5),
                   sin(theta5),   cos(theta5)*cos(alpha5),  -cos(theta5)*sin(alpha5),   a5*sin(theta5),
                   0,             sin(alpha5),               cos(alpha5),               d5,
                   0,             0,                         0,                         1);
    QMatrix4x4 T56(cos(theta6),  -sin(theta6)*cos(alpha6),   sin(theta6)*sin(alpha6),   a6*cos(theta6),
                   sin(theta6),   cos(theta6)*cos(alpha6),  -cos(theta6)*sin(alpha6),   a6*sin(theta6),
                   0,             sin(alpha6),               cos(alpha6),               d6,
                   0,             0,                         0,                         1);


    QMatrix4x4 T02=T01*T12;
    QMatrix4x4 T03=T01*T12*T23;
    QMatrix4x4 T04=T01*T12*T23*T34;
    QMatrix4x4 T05=T01*T12*T23*T34*T45;
    QMatrix4x4 T06=T01*T12*T23*T34*T45*T56;

    m_transformationMatrix_06 = T06;

    //qDebug() << "\n######### Transformation-coordinatsystem 0 -> 6: " << cutMatrixValues(T06);

    QVector3D P00(0,0,0);
    QVector3D P01(T01.column(3).toVector3D());
    QVector3D P02(T02.column(3).toVector3D());
    QVector3D P03(T03.column(3).toVector3D());
    QVector3D P04(T04.column(3).toVector3D());
    QVector3D P05(T05.column(3).toVector3D());
    QVector3D P06(T06.column(3).toVector3D());

    QVector3D Z00(QVector3D(0,0,1));
    QVector3D Z01(T01.column(2).toVector3D());
    QVector3D Z02(T02.column(2).toVector3D());
    QVector3D Z03(T03.column(2).toVector3D());
    QVector3D Z04(T04.column(2).toVector3D());
    QVector3D Z05(T05.column(2).toVector3D());
    QVector3D Z06(T05.column(2).toVector3D());

    QVector3D Jv0(QVector3D::crossProduct(Z00,(P06-P00)));
    QVector3D Jv1(QVector3D::crossProduct(Z01,(P06-P01)));
    QVector3D Jv2(QVector3D::crossProduct(Z02,(P06-P02)));
    QVector3D Jv3(QVector3D::crossProduct(Z03,(P06-P03)));
    QVector3D Jv4(QVector3D::crossProduct(Z04,(P06-P04)));
    QVector3D Jv5(QVector3D::crossProduct(Z05,(P06-P05)));

    m_jakobi(0,0) = Jv0.x();
    m_jakobi(1,0) = Jv0.y();
    m_jakobi(2,0) = Jv0.z();

    m_jakobi(0,1) = Jv1.x();
    m_jakobi(1,1) = Jv1.y();
    m_jakobi(2,1) = Jv1.z();

    m_jakobi(0,2) = Jv2.x();
    m_jakobi(1,2) = Jv2.y();
    m_jakobi(2,2) = Jv2.z();

    m_jakobi(0,3) = Jv3.x();
    m_jakobi(1,3) = Jv3.y();
    m_jakobi(2,3) = Jv3.z();

    m_jakobi(0,4) = Jv4.x();
    m_jakobi(1,4) = Jv4.y();
    m_jakobi(2,4) = Jv4.z();

    m_jakobi(0,5) = Jv5.x();
    m_jakobi(1,5) = Jv5.y();
    m_jakobi(2,5) = Jv5.z();

    m_jakobi(3,0) = Z00.x();
    m_jakobi(4,0) = Z00.y();
    m_jakobi(5,0) = Z00.z();

    m_jakobi(3,1) = Z01.x();
    m_jakobi(4,1) = Z01.y();
    m_jakobi(5,1) = Z01.z();

    m_jakobi(3,2) = Z02.x();
    m_jakobi(4,2) = Z02.y();
    m_jakobi(5,2) = Z02.z();

    m_jakobi(3,3) = Z03.x();
    m_jakobi(4,3) = Z03.y();
    m_jakobi(5,3) = Z03.z();

    m_jakobi(3,4) = Z04.x();
    m_jakobi(4,4) = Z04.y();
    m_jakobi(5,4) = Z04.z();

    m_jakobi(3,5) = Z05.x();
    m_jakobi(4,5) = Z05.y();
    m_jakobi(5,5) = Z05.z();

    calculateJakobiInv();

    float x = T06.column(3).toVector3D().x();
    float y = T06.column(3).toVector3D().y();
    float z = T06.column(3).toVector3D().z();

    float wy = atan2(-T06(2,0),sqrt(pow(T06(0,0),2) + pow(T06(1,0),2)));
    float wz = atan2(T06(1,0)/cos(wy), T06(0,0)/cos(wy));
    float wx = atan2(T06(2,1)/cos(wy), T06(2,2)/cos(wy));

    m_currentTcpPose(0,0) = (float)((int)(x*1000))/1000;
    m_currentTcpPose(1,0) = (float)((int)(y*1000))/1000;
    m_currentTcpPose(2,0) = (float)((int)(z*1000))/1000;
    m_currentTcpPose(3,0) = (float)((int)(wx*1000))/1000;
    m_currentTcpPose(4,0) = (float)((int)(wy*1000))/1000;
    m_currentTcpPose(5,0) = (float)((int)(wz*1000))/1000;

    //qDebug() << "current tcpPose" << m_currentTcpPose;

    emit updateTcpPosition(m_currentTcpPose(0,0), m_currentTcpPose(1,0), m_currentTcpPose(2,0), m_currentTcpPose(3,0), m_currentTcpPose(4,0), m_currentTcpPose(5,0));
}

void RobotArm::transformPositionToAngle(float dx, float dy, float dz, float dwx, float dwy, float dwz)
{

    QGenericMatrix<1,6,float> deltaTcpPose;
    deltaTcpPose(0,0)=dx;
    deltaTcpPose(1,0)=dy;
    deltaTcpPose(2,0)=dz;
    deltaTcpPose(3,0)=dwx;
    deltaTcpPose(4,0)=dwy;
    deltaTcpPose(5,0)=dwz;

    // 0 = Base
    // 1 = TCP
    if(m_koordinateSystem == 1){

    }

    QGenericMatrix<1,6,float> deltaAngles = m_jakobiInv*deltaTcpPose;

    deltaAngles(0,0)=(float)((int)(deltaAngles(0,0)*10000))/10000;
    deltaAngles(1,0)=(float)((int)(deltaAngles(1,0)*10000))/10000;
    deltaAngles(2,0)=(float)((int)(deltaAngles(2,0)*10000))/10000;
    deltaAngles(3,0)=(float)((int)(deltaAngles(3,0)*10000))/10000;
    deltaAngles(4,0)=(float)((int)(deltaAngles(4,0)*10000))/10000;
    deltaAngles(5,0)=(float)((int)(deltaAngles(5,0)*10000))/10000;

    //qDebug()<< "delta angles" << deltaAngles << "for delta tcp pose" << deltaTcpPose;

    emit moveServo(deltaAngles(0,0),deltaAngles(1,0),deltaAngles(2,0),deltaAngles(3,0),deltaAngles(4,0),deltaAngles(5,0));
}

void RobotArm::transformAngleToPosition(float theta1, float theta2, float theta3, float theta4, float theta5, float theta6)
{
    m_theta1 = theta1-M_PI_2;
    m_theta2 = theta2-M_PI_2;
    m_theta3 = theta3-M_PI_2;
    m_theta4 = theta4-M_PI_2;
    m_theta5 = theta5-M_PI_2;
    m_theta6 = theta6-M_PI_2;

    m_currentAngles(0,0) = m_theta1;
    m_currentAngles(1,0) = m_theta2;
    m_currentAngles(2,0) = m_theta3;
    m_currentAngles(3,0) = m_theta4;
    m_currentAngles(4,0) = m_theta5;
    m_currentAngles(5,0) = m_theta6;

    emit anglesUpdated();
}

void RobotArm::updateAngles(float theta1, float theta2, float theta3, float theta4, float theta5, float theta6)
{
    m_theta1 = theta1-M_PI_2;
    m_theta2 = theta2-M_PI_2;
    m_theta3 = theta3-M_PI_2;
    m_theta4 = theta4-M_PI_2;
    m_theta5 = theta5-M_PI_2;
    m_theta6 = theta6-M_PI_2;

    m_currentAngles(0,0) = m_theta1;
    m_currentAngles(1,0) = m_theta2;
    m_currentAngles(2,0) = m_theta3;
    m_currentAngles(3,0) = m_theta4;
    m_currentAngles(4,0) = m_theta5;
    m_currentAngles(5,0) = m_theta6;

    //qDebug() << "current angles" << m_currentAngles;
    emit anglesUpdated();
}

void RobotArm::koordinateSystemChanged(const int &system)
{
    //qDebug() << "koordinatesystem changed to " << system;
    m_koordinateSystem = system;
}

