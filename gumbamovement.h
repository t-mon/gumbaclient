#ifndef GUMBAMOVEMENT_H
#define GUMBAMOVEMENT_H

#include <QObject>

class GumbaMovement : public QObject
{
    Q_OBJECT
public:
    explicit GumbaMovement(QObject *parent = 0);

private:
    unsigned int m_state;

signals:
    void writeToTerminal(const QString &terminalString);
    void sendCommand(QString,QString);

public slots:
    void parseJoystick(const float &angle, const float &magnitude);

};

#endif // GUMBAMOVEMENT_H
