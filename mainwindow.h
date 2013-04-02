#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QSlider>
#include <QPropertyAnimation>
#include <QGroupBox>

#include "robotvisualisation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);

private:
    RobotVisualisation *m_visualisation;

    QGroupBox *createGumbaConnectionGroupBox();
    QGroupBox *createGumbaSensorDataGroupBox();
    QGroupBox *createGumbaServerConnectionGroupBox();
    QGroupBox *createServoControllGroupBox();
    QGroupBox *createTerminalGroupBox();
    QGroupBox *createWiiMoteGroupBox();


    int convertPwmToDegreeBig(int pwm);
    int convertPwmToDegreeSmall(int pwm);

    int convertDegreeToPwmBig(int degree);
    int convertDegreeToPwmSmall(int degree);

    QPropertyAnimation *animationServo0;
    QPropertyAnimation *animationServo1;
    QPropertyAnimation *animationServo2;

    QTextEdit *terminalView;
    QSlider *velocitySlider;
    QLabel *velocityLabel;

    QSlider *servo0Slider;
    QLabel *servo0Label;

    QSlider *servo1Slider;
    QLabel *servo1Label;

    QSlider *servo2Slider;
    QLabel *servo2Label;

    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLabel *connectionStatusLabel;

    QLabel *bumperLeftLabel;
    QLabel *bumperRightLabel;
    QLabel *obstacleLeftLabel;
    QLabel *obstacleRightLabel;
    QLabel *motorLeftLabel;
    QLabel *motorRightLabel;
    QLabel *lightLeftLabel;
    QLabel *lightRightLabel;

    QLabel *batteryLabel;

    QLabel *wiiMoteRoll;
    QLabel *wiiMotePitch;
    QLabel *wiiMoteYaw;

    QLabel *nunchukRoll;
    QLabel *nunchukPitch;
    QLabel *nunchukYaw;

    QLabel *nunchukAngle;
    QLabel *nunchukMagnitude;

    bool m_wiiMoteABstate;

private slots:
    void connectServerClicked();
    void connectGumbaClicked();
    void disconnectGumbaClicked();
    void startGumbaApplicationClicked();
    void toggleRTSClicked();

    void velocitySliderChanged(int pos);

    void servo0SliderChanged(const int &pos);
    void servo0animationClicked();
    void servo0PowerOffClicked();
    void servo1SliderChanged(const int &pos);
    void servo1animationClicked();
    void servo1PowerOffClicked();
    void servo2SliderChanged(const int &pos);
    void servo2animationClicked();
    void servo2PowerOffClicked();

    void initServoClicked();

    void updateWiiMoteOrientation(const float &roll, const float &pitch, const float &yaw);
    void updateNunchuckOrientation(const float &roll, const float &pitch, const float &yaw);
    void updateNunchuckJoyStickData(const float &angle, const float &magnitude);

signals:
    void connectServer(const QString &ipAddress, const QString &port);
    void sendCommand(QString,QString);

    void angle1Changed();
    void angle2Changed();

    void startWiiMote();
    void stopWiiMote();

    void disconnectServer();

public slots:
    void writeToTerminal(const QString &terminalString);
    void connectionStatusChanged(const bool &connectionState);
    void bumperLeftPressed(const bool &bumperState);
    void bumperRightPressed(const bool &bumperState);
    void obstacleLeft(const bool &obstacleState);
    void obstacleRight(const bool &obstacleState);

    void motorLeft(int motorCurrent);
    void motorRight(int motorCurrent);
    void lightLeft(int lightIntensity);
    void lightRight(int lightIntensity);
    void batteryStatus(double battery);

    void wiiMoteABChanged(const bool &pressedState);

    void moveServo(const int &servoNumber, const int &start, const int &end, const int &period);
    void allSevosPwmOff();

};

#endif // MAINWINDOW_H
