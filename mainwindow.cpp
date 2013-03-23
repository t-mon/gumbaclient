#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>
#include <QColor>
#include <QDebug>
#include <QKeyEvent>
#include <QIcon>


// up-limit     = 249 ms = +90
// down-limit   = 57  ms = -90
#define upLimitBig 243
#define downLimitBig 57
#define upLimitSmall 227
#define downLimitSmall 50
#define periodMove 5000

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_wiiMoteABstate = false;

    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    setWindowTitle("Gumba Client");
    setWindowIcon(QIcon("gumbaclient80.png"));

    QGridLayout *mainGridLayout = new QGridLayout();
    centralWidget->setLayout(mainGridLayout);

    mainGridLayout->addWidget(createGumbaServerConnectionGroupBox(),0,0);
    mainGridLayout->addWidget(createGumbaConnectionGroupBox(),0,1);
    mainGridLayout->addWidget(createGumbaSensorDataGroupBox(),0,2);
    mainGridLayout->addWidget(createTerminalGroupBox(),1,0,1,2,Qt::AlignCenter);
    mainGridLayout->addWidget(createServoControllGroupBox(),1,2);
    mainGridLayout->addWidget(createWiiMoteGroupBox(),2,2);

}

void MainWindow::keyPressEvent( QKeyEvent *event){
    switch (event->key()){
    case Qt::Key_W:
        emit sendCommand("RoboterMovement","forward");
        break;
    case Qt::Key_A:
        emit sendCommand("RoboterMovement","left");
        break;
    case Qt::Key_S:
        emit sendCommand("RoboterMovement","backward");
        break;
    case Qt::Key_D:
        emit sendCommand("RoboterMovement","right");
        break;
    case Qt::Key_Q:
        emit sendCommand("RoboterMovement","turnLeft");
        break;
    case Qt::Key_E:
        emit sendCommand("RoboterMovement","turnRight");
        break;
    case Qt::Key_X:
        emit sendCommand("RoboterMovement","stop");
        break;
    }
}

QGroupBox *MainWindow::createGumbaConnectionGroupBox()
{
    QGroupBox *gumbaConnectionGroupBox = new QGroupBox(tr("Gumba connection"),this);
    QGridLayout *gumbaConnectionGrid = new QGridLayout;


    //Connection Gumba
    QPushButton *connectGumbaButton = new QPushButton("Connect Gumba");
    QPushButton *disconnectGumbaButton = new QPushButton("Disconnect Gumba");
    QPushButton *startGumbaApplicationButton = new QPushButton("Start Gumba");
    QPushButton *toggleRTSButton = new QPushButton("Toggle RTS");


    gumbaConnectionGrid->addWidget(connectGumbaButton,0,0);
    gumbaConnectionGrid->addWidget(disconnectGumbaButton,1,0);
    gumbaConnectionGrid->addWidget(startGumbaApplicationButton,0,1);
    gumbaConnectionGrid->addWidget(toggleRTSButton,1,1);

    gumbaConnectionGroupBox->setLayout(gumbaConnectionGrid);

    connect(connectGumbaButton,SIGNAL(clicked()),this,SLOT(connectGumbaClicked()));
    connect(disconnectGumbaButton,SIGNAL(clicked()),this,SLOT(disconnectGumbaClicked()));
    connect(startGumbaApplicationButton,SIGNAL(clicked()),this,SLOT(startGumbaApplicationClicked()));
    connect(toggleRTSButton,SIGNAL(clicked()),this,SLOT(toggleRTSClicked()));

    return gumbaConnectionGroupBox;
}

QGroupBox *MainWindow::createGumbaSensorDataGroupBox()
{
    QGroupBox* gumbaSensorDataGroupBox = new QGroupBox(tr("Gumba sensor Data"),this);
    QGridLayout *gumbaSensorDataGrid = new QGridLayout;

    // Sensor status
    bumperLeftLabel = new QLabel(this);
    bumperLeftLabel->setAlignment(Qt::AlignCenter);
    bumperLeftLabel->setText("Bumper L");
    bumperLeftLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    bumperRightLabel = new QLabel(this);
    bumperRightLabel->setAlignment(Qt::AlignCenter);
    bumperRightLabel->setText("Bumper R");
    bumperRightLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    obstacleLeftLabel = new QLabel(this);
    obstacleLeftLabel->setAlignment(Qt::AlignCenter);
    obstacleLeftLabel->setText("Obstacle L");
    obstacleLeftLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    obstacleRightLabel = new QLabel(this);
    obstacleRightLabel->setAlignment(Qt::AlignCenter);
    obstacleRightLabel->setText("Obstacle R");
    obstacleRightLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    motorLeftLabel = new QLabel(this);
    motorLeftLabel->setText("Motor L: 0");

    motorRightLabel = new QLabel(this);
    motorRightLabel->setText("Motor R: 0");

    lightLeftLabel = new QLabel(this);
    lightLeftLabel->setText("Light L: 0");

    lightRightLabel = new QLabel(this);
    lightRightLabel->setText("Light R: 0");

    batteryLabel = new QLabel(this);
    batteryLabel->setText("0 V");
    batteryLabel->setAlignment(Qt::AlignCenter);
    batteryLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    velocitySlider = new QSlider(Qt::Horizontal, this);
    velocitySlider->setTickInterval(1);
    velocitySlider->setFixedWidth(150);
    velocitySlider->setMinimum(1);
    velocitySlider->setMaximum(4);

    velocityLabel = new QLabel(this);
    velocityLabel->setAlignment(Qt::AlignCenter);
    velocityLabel->setText("v=0");

    gumbaSensorDataGrid->addWidget(bumperLeftLabel,0,0);
    gumbaSensorDataGrid->addWidget(bumperRightLabel,0,1);
    gumbaSensorDataGrid->addWidget(obstacleLeftLabel,1,0);
    gumbaSensorDataGrid->addWidget(obstacleRightLabel,1,1);
    gumbaSensorDataGrid->addWidget(motorLeftLabel,2,0);
    gumbaSensorDataGrid->addWidget(motorRightLabel,2,1);
    gumbaSensorDataGrid->addWidget(lightLeftLabel,3,0);
    gumbaSensorDataGrid->addWidget(lightRightLabel,3,1);
    gumbaSensorDataGrid->addWidget(batteryLabel,4,0);
    gumbaSensorDataGrid->addWidget(velocitySlider,5,0);
    gumbaSensorDataGrid->addWidget(velocityLabel,5,1);

    gumbaSensorDataGroupBox->setLayout(gumbaSensorDataGrid);

    connect(velocitySlider,SIGNAL(sliderMoved(int)),this,SLOT(velocitySliderChanged(int)));

    return gumbaSensorDataGroupBox;
}

QGroupBox *MainWindow::createGumbaServerConnectionGroupBox()
{
    QGroupBox *gumbaServerConnectionGroupBox = new QGroupBox(tr("Server connection"),this);
    QGridLayout *gumbaServerConnectionGrid = new QGridLayout;

    //Connection Host
    connectionStatusLabel = new QLabel(this);
    connectionStatusLabel->setFixedWidth(25);
    connectionStatusLabel->setAlignment(Qt::AlignCenter);
    connectionStatusLabel->setText("");
    connectionStatusLabel->setStyleSheet("background-color:red; border-width:1px; border-radius: 12px;");

    QPushButton *connectServerButton = new QPushButton("Connect Server");
    QPushButton *disconnectServerButton = new QPushButton("Disconnect Server");

    hostLineEdit = new QLineEdit();
    hostLineEdit->setFixedWidth(120);
    hostLineEdit->setText("10.10.10.40");

    portLineEdit = new QLineEdit();
    portLineEdit->setFixedWidth(60);
    portLineEdit->setText("2222");

    gumbaServerConnectionGrid->addWidget(connectServerButton,0,0);
    gumbaServerConnectionGrid->addWidget(disconnectServerButton,1,0);
    gumbaServerConnectionGrid->addWidget(connectionStatusLabel,0,2);
    gumbaServerConnectionGrid->addWidget(hostLineEdit,2,0);
    gumbaServerConnectionGrid->addWidget(portLineEdit,3,0);

    gumbaServerConnectionGroupBox->setLayout(gumbaServerConnectionGrid);

    connect(connectServerButton,SIGNAL(clicked()),this,SLOT(connectServerClicked()));
    connect(disconnectServerButton,SIGNAL(clicked()),this,SIGNAL(disconnectServer()));

    return gumbaServerConnectionGroupBox;
}

QGroupBox *MainWindow::createServoControllGroupBox()
{
    QGroupBox *servoControllGroupBox = new QGroupBox(tr("Servo controll"),this);
    QGridLayout *servoControllGrid = new QGridLayout;

    QPushButton *initServo = new QPushButton("Servo init");
    QPushButton *servoAllPwmOff = new QPushButton("All Servos OFF");
    QPushButton *servo0animationButton = new QPushButton("Start animation");
    QPushButton *servo0poweOffButton = new QPushButton("Power OFF");
    QPushButton *servo1animationButton = new QPushButton("Start animation");
    QPushButton *servo1poweOffButton = new QPushButton("Power OFF");
    QPushButton *servo2animationButton = new QPushButton("Start animation");
    QPushButton *servo2poweOffButton = new QPushButton("Power OFF");

    // Servo number    GPIO number   Pin in P1 header
    //      0               4             P1-7
    //      1              17             P1-11
    //      2              18             P1-12
    //      3              21             P1-13
    //      4              22             P1-15
    //      5              23             P1-16
    //      6              24             P1-18
    //      7              25             P1-22

    // ===================================================== Servo 0
    servo0Slider = new QSlider(Qt::Horizontal, this);
    servo0Slider->setFixedWidth(100);
    servo0Slider->setTickInterval(1);
    servo0Slider->setMinimum(downLimitSmall);
    servo0Slider->setMaximum(upLimitSmall);
    servo0Slider->setValue(downLimitSmall);

    servo0Label = new QLabel(this);
    servo0Label->setFixedWidth(60);
    servo0Label->setAlignment(Qt::AlignCenter);
    servo0Label->setText("S0 = ");

    // ===================================================== Servo 1
    servo1Slider = new QSlider(Qt::Horizontal, this);
    servo1Slider->setFixedWidth(100);
    servo1Slider->setTickInterval(1);
    servo1Slider->setMinimum(downLimitBig);
    servo1Slider->setMaximum(upLimitBig);
    servo1Slider->setValue(downLimitBig);

    servo1Label = new QLabel(this);
    servo1Label->setAlignment(Qt::AlignCenter);
    servo1Label->setText("S1 = ");

    // ===================================================== Servo 2
    servo2Slider = new QSlider(Qt::Horizontal, this);
    servo2Slider->setFixedWidth(100);
    servo2Slider->setTickInterval(1);
    servo2Slider->setMinimum(downLimitBig);
    servo2Slider->setMaximum(upLimitBig);
    servo2Slider->setValue(downLimitBig);

    servo2Label = new QLabel(this);
    servo2Label->setAlignment(Qt::AlignCenter);
    servo2Label->setText("S2 = ");

    animationServo0 = new QPropertyAnimation(servo0Slider,"sliderPosition");
    animationServo1 = new QPropertyAnimation(servo1Slider,"sliderPosition");
    animationServo2 = new QPropertyAnimation(servo2Slider,"sliderPosition");


    servoControllGrid->addWidget(initServo,0,2);
    servoControllGrid->addWidget(servoAllPwmOff,0,3);

    servoControllGrid->addWidget(servo0Slider,1,0);
    servoControllGrid->addWidget(servo0Label,1,1);
    servoControllGrid->addWidget(servo0animationButton,1,2);
    servoControllGrid->addWidget(servo0poweOffButton,1,3);

    servoControllGrid->addWidget(servo1Slider,2,0);
    servoControllGrid->addWidget(servo1Label,2,1);
    servoControllGrid->addWidget(servo1animationButton,2,2);
    servoControllGrid->addWidget(servo1poweOffButton,2,3);

    servoControllGrid->addWidget(servo2Slider,3,0);
    servoControllGrid->addWidget(servo2Label,3,1);
    servoControllGrid->addWidget(servo2animationButton,3,2);
    servoControllGrid->addWidget(servo2poweOffButton,3,3);

    servoControllGroupBox->setLayout(servoControllGrid);

    connect(initServo,SIGNAL(clicked()),this,SLOT(initServoClicked()));
    connect(servo0Slider,SIGNAL(valueChanged(int)),this,SLOT(servo0SliderChanged(int)));
    connect(servo1Slider,SIGNAL(valueChanged(int)),this,SLOT(servo1SliderChanged(int)));
    connect(servo2Slider,SIGNAL(valueChanged(int)),this,SLOT(servo2SliderChanged(int)));
    connect(servo0animationButton,SIGNAL(clicked()),this,SLOT(servo0animationClicked()));
    connect(servo0poweOffButton,SIGNAL(clicked()),this,SLOT(servo0PowerOffClicked()));
    connect(servo1animationButton,SIGNAL(clicked()),this,SLOT(servo1animationClicked()));
    connect(servo1poweOffButton,SIGNAL(clicked()),this,SLOT(servo1PowerOffClicked()));
    connect(servo2animationButton,SIGNAL(clicked()),this,SLOT(servo2animationClicked()));
    connect(servo2poweOffButton,SIGNAL(clicked()),this,SLOT(servo2PowerOffClicked()));
    connect(servoAllPwmOff,SIGNAL(clicked()),this,SLOT(allSevosPwmOff()));

    return servoControllGroupBox;
}

QGroupBox *MainWindow::createTerminalGroupBox()
{
    QGroupBox *terminalGroupBox = new QGroupBox(tr("Terminal"),this);
    QGridLayout *terminalGrid = new QGridLayout;

    // Terminal
    terminalView = new QTextEdit(this);
    terminalGrid->addWidget(terminalView,0,0);
    terminalView->setMinimumWidth(550);
    terminalView->setMinimumHeight(400);
    terminalView->setReadOnly(true);
    terminalView->setTextColor(QColor(0, 255, 0,255));
    terminalGroupBox->setLayout(terminalGrid);

    return terminalGroupBox;
}

QGroupBox *MainWindow::createWiiMoteGroupBox()
{
    QGroupBox *wiiMoteGroupBox = new QGroupBox(tr("Wii"),this);
    QGridLayout *wiiMoteGrid = new QGridLayout;

    QPushButton *startWiiMoteButton = new QPushButton("Start WiiMote");
    QPushButton *disconnectWiiMoteButton = new QPushButton("Disconnct WiiMote");
    wiiMoteRoll = new QLabel(this);
    wiiMoteRoll->setFixedWidth(140);
    wiiMoteRoll->setAlignment(Qt::AlignLeft);
    wiiMoteRoll->setText("R: 0");

    wiiMotePitch = new QLabel(this);
    wiiMotePitch->setFixedWidth(140);
    wiiMotePitch->setAlignment(Qt::AlignLeft);
    wiiMotePitch->setText("P: 0");

    wiiMoteYaw = new QLabel(this);
    wiiMoteYaw->setFixedWidth(140);
    wiiMoteYaw->setAlignment(Qt::AlignLeft);
    wiiMoteYaw->setText("Y: 0");

    nunchukAngle = new QLabel(this);
    nunchukAngle->setFixedWidth(140);
    nunchukAngle->setAlignment(Qt::AlignLeft);
    nunchukAngle->setText("Ang: 0");

    nunchukMagnitude = new QLabel(this);
    nunchukMagnitude->setFixedWidth(140);
    nunchukMagnitude->setAlignment(Qt::AlignLeft);
    nunchukMagnitude->setText("Mag: 0");

    wiiMoteGrid->addWidget(startWiiMoteButton,0,0);
    wiiMoteGrid->addWidget(disconnectWiiMoteButton,1,0);
    wiiMoteGrid->addWidget(wiiMoteRoll,0,1);
    wiiMoteGrid->addWidget(wiiMotePitch,1,1);
    wiiMoteGrid->addWidget(wiiMoteYaw,2,1);
    wiiMoteGrid->addWidget(nunchukAngle,0,2);
    wiiMoteGrid->addWidget(nunchukMagnitude,1,2);

    connect(startWiiMoteButton,SIGNAL(clicked()),this,SIGNAL(startWiiMote()))   ;
    connect(disconnectWiiMoteButton,SIGNAL(clicked()),this,SIGNAL(stopWiiMote()));

    wiiMoteGroupBox->setLayout(wiiMoteGrid);

    return wiiMoteGroupBox;
}

int MainWindow::convertPwmToDegreeBig(int pwm)
{
    // up-limit     = 249 ms = +90
    // down-limit   = 57  ms = -90
    return (180*(pwm-downLimitBig)/(upLimitBig-downLimitBig))-90;
}

int MainWindow::convertPwmToDegreeSmall(int pwm)
{
    // up-limit     = 249 ms = +90
    // down-limit   = 57  ms = -90
    return (180*(pwm-downLimitSmall)/(upLimitSmall-downLimitSmall))-90;
}

int MainWindow::convertDegreeToPwmBig(int degree)
{
    return ((degree-90)*(upLimitBig-downLimitBig)-(180*downLimitBig))/180;
}

int MainWindow::convertDegreeToPwmSmall(int degree)
{
    return ((degree-90)*(upLimitSmall-downLimitSmall)-(180*downLimitSmall))/180;
}


void MainWindow::connectServerClicked(){
    emit connectServer(hostLineEdit->text(),portLineEdit->text());
}

void MainWindow::connectGumbaClicked()
{
    emit sendCommand("RoboterApplication","connect");
    writeToTerminal("Connect gumba with server");
}

void MainWindow::disconnectGumbaClicked()
{
    emit sendCommand("RoboterApplication","disconnect");
    writeToTerminal("Disconnect gumba from server...");
}

void MainWindow::startGumbaApplicationClicked()
{
    emit sendCommand("RoboterApplication","start");
    writeToTerminal("start gumba application");
}

void MainWindow::toggleRTSClicked()
{
    emit sendCommand("RoboterApplication","toggle");
    writeToTerminal("toggle RTS");
}

void MainWindow::velocitySliderChanged(int pos)
{
    switch(pos){

    case 1:
        emit sendCommand("RoboterSpeed","1");
        writeToTerminal("speedlevel: 1");
        velocityLabel->setText("v = low");
        break;
    case 2:
        emit sendCommand("RoboterSpeed","2");
        writeToTerminal("speedlevel: 2");
        velocityLabel->setText("v = med");
        break;
    case 3:
        emit sendCommand("RoboterSpeed","3");
        writeToTerminal("speedlevel: 3");
        velocityLabel->setText("v = high");
        break;
    case 4:
        emit sendCommand("RoboterSpeed","4");
        writeToTerminal("speedlevel: MAX");
        velocityLabel->setText("v = max");
        break;
    }
}

void MainWindow::servo0SliderChanged(const int &pos)
{
    servo0Label->setText("S0 = " + QString::number(convertPwmToDegreeSmall(pos)));
    //if(pos%2==0){
    emit sendCommand("Servo0",QString::number(pos));
    //}
}

void MainWindow::servo0animationClicked()
{
    moveServo(0,upLimitSmall,downLimitSmall,periodMove);
    moveServo(1,upLimitBig,downLimitBig,periodMove);
    moveServo(2,upLimitBig,downLimitBig,periodMove);

}

void MainWindow::servo0PowerOffClicked()
{
    servo0Label->setText("S0 = OFF");
    emit sendCommand("Servo0",QString::number(0));
}

void MainWindow::servo1SliderChanged(const int &pos)
{
    servo1Label->setText("S1 = " + QString::number(convertPwmToDegreeBig(pos)));
    emit sendCommand("Servo1",QString::number(pos));
}

void MainWindow::servo1animationClicked()
{
    moveServo(1,downLimitBig,upLimitBig,periodMove);
}

void MainWindow::servo1PowerOffClicked()
{
    servo1Label->setText("S1 = OFF");
    emit sendCommand("Servo1",QString::number(0));
}

void MainWindow::servo2SliderChanged(const int &pos)
{
    servo2Label->setText("S2 = " + QString::number(convertPwmToDegreeBig(pos)));
    emit sendCommand("Servo2",QString::number(pos));
}

void MainWindow::servo2animationClicked()
{
    moveServo(2,downLimitBig,upLimitBig,periodMove);
}

void MainWindow::servo2PowerOffClicked()
{
    servo2Label->setText("S2 = OFF");
    emit sendCommand("Servo2",QString::number(0));
}

void MainWindow::allSevosPwmOff()
{
    animationServo0->stop();
    animationServo1->stop();
    animationServo2->stop();

    servo0PowerOffClicked();
    servo1PowerOffClicked();
    servo2PowerOffClicked();
    emit writeToTerminal("all movements stopped, als pwm's resetted!!");
}

void MainWindow::moveServo(const int &servoNumber, const int &start, const int &end, const int &period)
{
    //OutBounce
    //InOutExpo
    //InOutCubic
    //InOutQuint

    switch(servoNumber){
    case 0:
        animationServo0->setDuration(period);
        animationServo0->setStartValue(start);
        animationServo0->setEndValue(end);
        animationServo0->setEasingCurve(QEasingCurve::InOutQuint);
        animationServo0->start();
        break;
    case 1:
        animationServo1->setDuration(period);
        animationServo1->setStartValue(start);
        animationServo1->setEndValue(end);
        animationServo1->setEasingCurve(QEasingCurve::InOutQuint);
        animationServo1->start();
        break;
    case 2:
        animationServo2->setDuration(period);
        animationServo2->setStartValue(start);
        animationServo2->setEndValue(end);
        animationServo2->setEasingCurve(QEasingCurve::InOutQuint);
        animationServo2->start();
        break;
    }
}


void MainWindow::initServoClicked()
{
    emit sendCommand("Servo","init");
}

void MainWindow::updateWiiMoteOrientation(const float &roll, const float &pitch, const float &yaw)
{
    int roll_i, pitch_i, yaw_i;
    roll_i = roll;
    pitch_i = pitch;
    yaw_i = yaw;
    wiiMoteRoll->setText("R: " + QString::number(roll_i));
    wiiMotePitch->setText("P: " + QString::number(pitch_i));
    wiiMoteYaw->setText("Y: " + QString::number(yaw_i));

    if(m_wiiMoteABstate){
        servo2Slider->setValue(pitch_i*(-1)+50);
        //servo1Slider->setValue(roll_i*(-1)+100);
    }


}

void MainWindow::updateNunchuckJoyStickData(const float &angle, const float &magnitude)
{
    nunchukAngle->setText("Ang: " + QString::number(angle));
    nunchukMagnitude->setText("Mag: " + QString::number(magnitude));
}


void MainWindow::writeToTerminal(const QString &terminalString){
    qDebug() << terminalString;
    terminalView->append(" " + terminalString);
}

void MainWindow::connectionStatusChanged(const bool &connectionState)
{
    if(connectionState){
        connectionStatusLabel->setStyleSheet("background-color:green; border-width:1px; border-radius: 12px;");
    }
    if(!connectionState){
        connectionStatusLabel->setStyleSheet("background-color:red; border-width:1px; border-radius: 12px;");
    }
}

void MainWindow::bumperLeftPressed(const bool &bumperState)
{
    if(bumperState){
        bumperLeftLabel->setStyleSheet("background-color:red; border-radius: 5px;");
    }
    if(!bumperState){
        bumperLeftLabel->setStyleSheet("background-color:green; border-radius: 5px;");
    }
}

void MainWindow::bumperRightPressed(const bool &bumperState)
{
    if(bumperState){
        bumperRightLabel->setStyleSheet("background-color:red; border-radius: 5px;");
    }
    if(!bumperState){
        bumperRightLabel->setStyleSheet("background-color:green; border-radius: 5px;");
    }
}

void MainWindow::obstacleLeft(const bool &obstacleState)
{
    if(obstacleState){
        obstacleLeftLabel->setStyleSheet("background-color:red; border-radius: 5px;");
    }
    if(!obstacleState){
        obstacleLeftLabel->setStyleSheet("background-color:green; border-radius: 5px;");
    }
}

void MainWindow::obstacleRight(const bool &obstacleState)
{
    if(obstacleState){
        obstacleRightLabel->setStyleSheet("background-color:red; border-radius: 5px;");
    }
    if(!obstacleState){
        obstacleRightLabel->setStyleSheet("background-color:green; border-radius: 5px;");
    }
}

void MainWindow::motorLeft(int motorCurrent)
{
    motorLeftLabel->setText("Motor L: " + QString::number(motorCurrent));
}

void MainWindow::motorRight(int motorCurrent)
{
    motorRightLabel->setText("Motor R: " + QString::number(motorCurrent));
}

void MainWindow::lightLeft(int lightIntensity)
{
    lightLeftLabel->setText("Light L: " + QString::number(lightIntensity));
}

void MainWindow::lightRight(int lightIntensity)
{
    lightRightLabel->setText("Light R: " + QString::number(lightIntensity));
}

void MainWindow::batteryStatus(double battery)
{
    batteryLabel->setText(QString::number(battery)+" V");
}

void MainWindow::wiiMoteABChanged(const bool &pressedState)
{
    m_wiiMoteABstate = pressedState;
}


