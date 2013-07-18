#ifndef ROBOTARM_H
#define ROBOTARM_H

#include <QObject>
#include <QMatrix4x4>

class RobotArm : public QObject
{
    Q_OBJECT
public:
    explicit RobotArm(QObject *parent = 0);
    
private:
    void homePosition();

    QGenericMatrix<1,5,float> m_currentAngles;
    QGenericMatrix<1,6,float> m_currentTcpPose;

    float m_theta1;
    float m_theta2;
    float m_theta3;
    float m_theta4;
    float m_theta5;

    QMatrix4x4 m_koordinateSystem_01;
    QMatrix4x4 m_koordinateSystem_12;
    QMatrix4x4 m_koordinateSystem_23;
    QMatrix4x4 m_koordinateSystem_34;
    QMatrix4x4 m_koordinateSystem_45;

    QMatrix4x4 cutMatrixValues(QMatrix4x4 matrix);

    QGenericMatrix<5,6,float> m_jakobi;
    QGenericMatrix<6,5,float> m_jakobiInv;

    void calculateJakobiInv();


signals:
    void moveServo();
    void anglesUpdated();
    void updateTcpPosition(const float &x, const float &y, const float &z, const float &wx, const float &wy, const float &wz);

    void writeToTerminal(const QString &terminalString);

private slots:

    void calculateCurrentPosition();

    void transformPositionToAngle(float x, float y, float z, float wx, float wy, float wz);
    void transformAngleToPosition(float theta1, float theta2, float theta3, float theta4, float theta5);

public slots:
    void updateAngles(float theta1, float theta2, float theta3, float theta4, float theta5);
    
};

#endif // ROBOTARM_H
