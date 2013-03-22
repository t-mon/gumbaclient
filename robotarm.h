#ifndef ROBOTARM_H
#define ROBOTARM_H

#include <QObject>

class RobotArm : public QObject
{
    Q_OBJECT
public:
    explicit RobotArm(QObject *parent = 0);
    
private:
    void homePosition();

signals:
    void moveServo();

    
public slots:
    
};

#endif // ROBOTARM_H
