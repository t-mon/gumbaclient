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



signals:
    void moveServo();
    void anglesUpdated();

private slots:

    QMatrix4x4 transform05();


    void transformPositionToAngle();
    void transformAngleToPosition();

public slots:
    void updateAngles(float theta1, float theta2, float theta3, float theta4, float theta5);
    
};

#endif // ROBOTARM_H
