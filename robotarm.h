#ifndef ROBOTARM_H
#define ROBOTARM_H

#include <QObject>
#include <QMatrix4x4>
#include <octave/oct.h>

class RobotArm : public QObject
{
    Q_OBJECT
public:
    explicit RobotArm(QObject *parent = 0);
    
private:
    void homePosition();

    QGenericMatrix<1,6,float> m_currentAngles;
    QGenericMatrix<1,6,float> m_currentTcpPose;

    float m_theta1;
    float m_theta2;
    float m_theta3;
    float m_theta4;
    float m_theta5;
    float m_theta6;

    QMatrix4x4 m_transformationMatrix_06;

    QMatrix4x4 cutMatrixValues(QMatrix4x4 matrix);

    QGenericMatrix<6,6,float> m_jakobi;
    QGenericMatrix<6,6,float> m_jakobiInv;

    void calculateJakobiInv();

    int m_koordinateSystem;

signals:
    void moveServo(float theta1, float theta2, float theta3, float theta4, float theta5, float theta6);
    void anglesUpdated();
    void updateTcpPosition(const float &x, const float &y, const float &z, const float &wx, const float &wy, const float &wz);

    void writeToTerminal(const QString &terminalString);



private slots:

    void calculateCurrentPosition();

    void transformPositionToAngle(float dx, float dy, float dz, float dwx, float dwy, float dwz);
    void transformAngleToPosition(float theta1, float theta2, float theta3, float theta4, float theta5, float theta6);

public slots:
    void updateAngles(float theta1, float theta2, float theta3, float theta4, float theta5, float theta6);
    void koordinateSystemChanged(const int &system);
};

#endif // ROBOTARM_H
