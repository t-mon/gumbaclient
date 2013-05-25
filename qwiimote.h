#ifndef QWIIMOTE_H
#define QWIIMOTE_H

#include <wiic/wiicpp.h>

#include <QObject>
#include <QThread>
#include <QMutex>

class QWiiMote : public QObject
{
    Q_OBJECT
public:
    explicit QWiiMote(QObject *parent = 0);
    bool m_exit;

private:    
    CWiimote *m_wiiMote;
    void HandleEvent();
    void HandleStatus();

    QMutex m_mutex;

signals:
    // Wii Mote
    void button_A_pressed(const bool &pressedState);
    void button_B_pressed(const bool &pressedState);
    void button_AB_pressed(const bool &pressedState);
    void button_MINUS_pressed();
    void button_PLUS_pressed();
    void button_DOWN_pressed();
    void button_UP_pressed();
    void button_LEFT_pressed();
    void button_RIGHT_pressed();
    void button_ONE_pressed();
    void button_TWO_pressed();
    void button_HOME_pressed();

    // Nunchuk
    void button_NC_C_pressed();
    void button_NC_Z_pressed();
    void nunchukJoystickChanged(const float &angle, const float &magnitude);

    // Classic Controller
    void button_Classic_A_pressed();
    void button_Classic_B_pressed();
    void button_Classic_X_pressed();
    void button_Classic_Y_pressed();
    void button_Classic_LEFT_pressed();
    void button_Classic_RIGHT_pressed();
    void button_Classic_UP_pressed();
    void button_Classic_DOWN_pressed();
    void button_Classic_PLUS_pressed();
    void button_Classic_MINUS_pressed();
    void button_Classic_HOME_pressed();
    void button_Classic_ZL_pressed();
    void button_Classic_ZR_pressed();
    void button_Classic_FULL_L_pressed();
    void button_Classic_FULL_R_pressed();
    void classicJoystickLeftChanged(const float &angle, const float &magnitude);
    void classicJoystickRightChanged(const float &angle, const float &magnitude);

    // Guitar Hero guitar
    void button_GH_STRUM_UP_pressed();
    void button_GH_STRUM_DOWN_pressed();
    void button_GH_YELLOW_pressed();
    void button_GH_GREEN_pressed();
    void button_GH_BLUE_pressed();
    void button_GH_RED_pressed();
    void button_GH_ORANGE_pressed();
    void button_GH_PLUS_pressed();
    void button_GH_MINUS_pressed();
    void guitarHeroWhammyBarChanged(const float &whammyValue);
    void guitarHeroJoystickChanged(const float &angle, const float &magnitude);

    // Orientation
    void orientationWiiMoteChanged(const float &roll, const float &pitch, const float &yaw);
    void orientationNunChuckChanged(const float &roll, const float &pitch, const float &yaw);
    void orientationMotionPlusChanged(const float &roll, const float &pitch, const float &yaw);


    // Wii general
    void wiiBatteryChanged(const float &battery);
    void wiiMoteStatusChanged(const int &wiiMoteNumber, const bool &speakerStatus, const bool &irStatus, const float &battery);
    void wiiLedChanged(const int &led1, const int &led2, const int &led3, const int &led4);
    void updateOrientationWiiMoteKalman(float roll, float pitch, float yaw);


    // General
    void stopProcess();
    void writeToTerminal(const QString &terminalString);

public slots:
    void startWiiMotesLoop();
    void disconnectWiiMote();

};

#endif // QWIIMOTE_H
