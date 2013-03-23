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
    int m_theta1;
    int m_theta2;
    int m_theta3;
    int m_theta4;
    int m_theta5;

signals:
    void moveServo();
    void anglesUpdated();

private slots:
    QMatrix4x4 transform05();
    void transformPositionToAngle();
    void transformAngleToPosition();

public slots:
    void updateAngles(int theta1, int theta2, int theta3, int theta4, int theta5);
    
};

#endif // ROBOTARM_H
