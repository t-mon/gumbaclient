#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QSlider>
#include <QPropertyAnimation>
#include <QGroupBox>
#include <QProgressBar>

#include "robotvisualisation.h"

// PWM limits for servos (big servo, small sevo)
// up-limit big     = 243 ms = +90
// down-limit big   = 57  ms = -90
// up-limit small   = 227 ms = +90
// down-limit small = 50  ms = -90
//#define upLimitBig 243
//#define downLimitBig 57
//#define upLimitSmall 227
//#define downLimitSmall 50
#define periodMove 2500

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

    QWidget *createGumbaGroupBox();
    QWidget *createRobotarmGroupBox();
    QWidget *createRobotWidget();

    QGroupBox *createGumbaConnectionGroupBox();
    QGroupBox *createGumbaSensorDataGroupBox();
    QGroupBox *createGumbaServerConnectionGroupBox();
    QGroupBox *createServoControllGroupBox();
    QGroupBox *createServoPositionGroupBox();
    QGroupBox *createServoOrientationGroupBox();
    QGroupBox *createTcpControllGroupBox();
    QGroupBox *createTcpControllRotGroupBox();

    QGroupBox *createTerminalGroupBox();
    QWidget *createWiiMoteGroupBox();

    float m_angle0;
    float m_angle1;
    float m_angle2;
    float m_angle3;
    float m_angle4;
    float m_angle5;

    QLabel *tcp_x_Lable;
    QLabel *tcp_y_Lable;
    QLabel *tcp_z_Lable;
    QLabel *tcp_wx_Lable;
    QLabel *tcp_wy_Lable;
    QLabel *tcp_wz_Lable;

    QPropertyAnimation *animationServo0;
    QPropertyAnimation *animationServo1;
    QPropertyAnimation *animationServo2;
    QPropertyAnimation *animationServo3;
    QPropertyAnimation *animationServo4;
    QPropertyAnimation *animationServo5;

    QTextEdit *terminalView;

    QSlider *servo0Slider;
    QLabel *servo0Label;
    QSlider *servo1Slider;
    QLabel *servo1Label;
    QSlider *servo2Slider;
    QLabel *servo2Label;
    QSlider *servo3Slider;
    QLabel *servo3Label;
    QSlider *servo4Slider;
    QLabel *servo4Label;
    QSlider *servo5Slider;
    QLabel *servo5Label;


    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLabel *connectionStatusLabel;

    QLabel *bumperLeftLabel;
    QLabel *bumperRightLabel;
    QLabel *obstacleLeftLabel;
    QLabel *obstacleRightLabel;
    QLabel *motorLeftLabel;
    QLabel *motorRightLabel;
    QLabel *speedLeftLabel;
    QLabel *speedRightLabel;
    QLabel *lightLeftLabel;
    QLabel *lightRightLabel;

    QProgressBar *gumbaBatteryBar;

    // Wii
    QProgressBar *wiiBatteryBar;
    QLabel *wiiMoteRoll;
    QLabel *wiiMotePitch;
    QLabel *wiiMoteYaw;
    QLabel *nunchukRoll;
    QLabel *nunchukPitch;
    QLabel *nunchukYaw;
    QLabel *nunchukAngle;
    QLabel *nunchukMagnitude;
    QLabel *classicLeftAngle;
    QLabel *classicLeftMagnitude;
    QLabel *classicRightAngle;
    QLabel *classicRightMagnitude;
    QLabel *guitarWhaa;
    QLabel *guitarAngle;
    QLabel *guitarMagnitude;

    bool m_wiiMoteABstate;

private slots:
    void connectServerClicked();
    void connectGumbaClicked();
    void disconnectGumbaClicked();
    void startGumbaApplicationClicked();
    void toggleRTSClicked();

    void servo0SliderChanged(const int &pos);
    void servo0animationClicked();
    void servo0PowerOffClicked();
    void servo1SliderChanged(const int &pos);
    void servo1animationClicked();
    void servo1PowerOffClicked();
    void servo2SliderChanged(const int &pos);
    void servo2animationClicked();
    void servo2PowerOffClicked();
    void servo3SliderChanged(const int &pos);
    void servo3animationClicked();
    void servo3PowerOffClicked();
    void servo4SliderChanged(const int &pos);
    void servo4animationClicked();
    void servo4PowerOffClicked();
    void servo5SliderChanged(const int &pos);
    void servo5animationClicked();
    void servo5PowerOffClicked();

    void updateAllAngles();

    void tcpXPlusClicked();
    void tcpXMinusClicked();
    void tcpYPlusClicked();
    void tcpYMinusClicked();
    void tcpZPlusClicked();
    void tcpZMinusClicked();
    void tcpWXPlusClicked();
    void tcpWXMinusClicked();
    void tcpWYPlusClicked();
    void tcpWYMinusClicked();
    void tcpWZPlusClicked();
    void tcpWZMinusClicked();

    void initServoClicked();
    void servoHomePositionClicked();
    void updateWiiMoteOrientation(const float &roll, const float &pitch, const float &yaw);
    void updateNunchuckOrientation(const float &roll, const float &pitch, const float &yaw);
    void updateNunchuckJoyStickData(const float &angle, const float &magnitude);
    void updateClassicLeftJoyStickData(const float &angle, const float &magnitude);
    void updateClassicRightJoyStickData(const float &angle, const float &magnitude);
    void updateGuitarHeroJoyStickData(const float &angle, const float &magnitude);
    void updateGuitarHeroWhaaData(const float &whaa);
    void updateWiiBattery(const float &battery);


signals:
    void connectServer(const QString &ipAddress, const QString &port);
    void sendCommand(QString,QString);

    void angle0Changed();
    void angle1Changed();
    void angle2Changed();
    void angle3Changed();
    void angle4Changed();
    void angle5Changed();

    void startWiiMote();
    void stopWiiMote();

    void disconnectServer();

    void calculatePosition(float theta1, float theta2, float theta3, float theta4, float theta5, float theta6);
    void moveToPosition(const float &x, const float &y, const float &z, const float &wx, const float &wy, const float &wz);
    void koordinateSystemChanged(const int &system);

    void zoomIn();
    void zoomOut();

public slots:
    void writeToTerminal(const QString &terminalString);
    void connectionStatusChanged(const bool &connectionState);
    void bumperLeftPressed(const bool &bumperState);
    void bumperRightPressed(const bool &bumperState);
    void obstacleLeft(const bool &obstacleState);
    void obstacleRight(const bool &obstacleState);

    void motorLeft(int motorCurrent);
    void motorRight(int motorCurrent);
    void speedLeft(double speedL);
    void speedRight(double speedR);
    void lightLeft(int lightIntensity);
    void lightRight(int lightIntensity);
    void batteryStatus(double battery);

    void wiiMoteABChanged(const bool &pressedState);

    void tcpPositionChanged(const float &x, const float &y, const float &z, const float &wx, const float &wy, const float &wz);

    void moveServo(const int &servoNumber, const int &start, const int &end, const int &period);
    void moveJoints(float theta1, float theta2, float theta3, float theta4, float theta5, float theta6);
    void allSevosPwmOff();

};

#endif // MAINWINDOW_H
