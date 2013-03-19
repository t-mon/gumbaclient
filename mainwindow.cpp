
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>
#include <QColor>
#include <QDebug>
#include <QKeyEvent>


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
    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    setWindowTitle("Gumba Client");

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    centralWidget->setLayout(verticalLayout);


    QGridLayout *gridLayout = new QGridLayout();

    verticalLayout->addLayout(gridLayout);

    QLabel *imageLable = new QLabel();
    verticalLayout->addWidget(imageLable);


    //Connection Gumba
    QPushButton *connectGumbaButton = new QPushButton("Connect Gumba");
    connectGumbaButton->setFixedWidth(150);
    gridLayout->addWidget(connectGumbaButton,0,0);

    QPushButton *disconnectGumbaButton = new QPushButton("Disconnect Gumba");
    disconnectGumbaButton->setFixedWidth(150);
    gridLayout->addWidget(disconnectGumbaButton,1,0);

    QPushButton *startGumbaApplicationButton = new QPushButton("Start Gumba");
    startGumbaApplicationButton->setFixedWidth(150);
    gridLayout->addWidget(startGumbaApplicationButton,0,3);

    QPushButton *toggleRTSButton = new QPushButton("Toggle RTS");
    toggleRTSButton->setFixedWidth(150);
    gridLayout->addWidget(toggleRTSButton,1,3);

    // Sensor status
    bumperLeftLabel = new QLabel(this);
    bumperLeftLabel->setFixedWidth(110);
    gridLayout->addWidget(bumperLeftLabel,0,4);
    bumperLeftLabel->setAlignment(Qt::AlignCenter);
    bumperLeftLabel->setText("Bumper L");
    bumperLeftLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    bumperRightLabel = new QLabel(this);
    bumperRightLabel->setFixedWidth(110);
    gridLayout->addWidget(bumperRightLabel,0,5);
    bumperRightLabel->setAlignment(Qt::AlignCenter);
    bumperRightLabel->setText("Bumper R");
    bumperRightLabel->setStyleSheet("background-color:gray; border-radius: 5px;");


    obstacleLeftLabel = new QLabel(this);
    obstacleLeftLabel->setFixedWidth(110);
    gridLayout->addWidget(obstacleLeftLabel,1,4);
    obstacleLeftLabel->setAlignment(Qt::AlignCenter);
    obstacleLeftLabel->setText("Obstacle L");
    obstacleLeftLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    obstacleRightLabel = new QLabel(this);
    obstacleRightLabel->setFixedWidth(110);
    gridLayout->addWidget(obstacleRightLabel,1,5);
    obstacleRightLabel->setAlignment(Qt::AlignCenter);
    obstacleRightLabel->setText("Obstacle R");
    obstacleRightLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    motorLeftLabel = new QLabel(this);
    motorLeftLabel->setFixedWidth(90);
    gridLayout->addWidget(motorLeftLabel,0,6);
    motorLeftLabel->setText("Motor L: 0");

    motorRightLabel = new QLabel(this);
    motorRightLabel->setFixedWidth(90);
    gridLayout->addWidget(motorRightLabel,0,7);
    motorRightLabel->setText("Motor R: 0");

    lightLeftLabel = new QLabel(this);
    lightLeftLabel->setFixedWidth(90);
    gridLayout->addWidget(lightLeftLabel,1,6);
    lightLeftLabel->setText("Light L: 0");

    lightRightLabel = new QLabel(this);
    lightRightLabel->setFixedWidth(90);
    gridLayout->addWidget(lightRightLabel,1,7);
    lightRightLabel->setText("Light R: 0");

    batteryLabel = new QLabel(this);
    batteryLabel->setFixedWidth(90);
    gridLayout->addWidget(batteryLabel,2,0);
    batteryLabel->setText("0 V");

    velocitySlider = new QSlider(Qt::Horizontal, this);
    velocitySlider->setFixedWidth(100);
    gridLayout->addWidget(velocitySlider,3,0);
    velocitySlider->setTickInterval(1);
    velocitySlider->setMinimum(1);
    velocitySlider->setMaximum(4);

    velocityLabel = new QLabel(this);
    velocityLabel->setFixedWidth(90);
    gridLayout->addWidget(velocityLabel,4,0);
    velocityLabel->setAlignment(Qt::AlignCenter);
    velocityLabel->setText("v=0");


    QPushButton *initServo = new QPushButton("Servo init");
    initServo->setFixedWidth(150);
    gridLayout->addWidget(initServo,4,1);


    QPushButton *servoAllPwmOff = new QPushButton("All Servos OFF");
    servoAllPwmOff->setFixedWidth(150);
    gridLayout->addWidget(servoAllPwmOff,5,1);

    // Servo number    GPIO number   Pin in P1 header
    //      0               4             P1-7
    //      1              17             P1-11
    //      2              18             P1-12
    //      3              21             P1-13
    //      4              22             P1-15
    //      5              23             P1-16
    //      6              24             P1-18
    //      7              25             P1-22

    servo0Slider = new QSlider(Qt::Horizontal, this);
    servo0Slider->setFixedWidth(150);
    gridLayout->addWidget(servo0Slider,2,2);
    servo0Slider->setTickInterval(1);
    servo0Slider->setMinimum(downLimitSmall);
    servo0Slider->setMaximum(upLimitSmall);
    servo0Slider->setValue(downLimitSmall);

    servo0Label = new QLabel(this);
    servo0Label->setFixedWidth(150);
    gridLayout->addWidget(servo0Label,3,2);
    servo0Label->setAlignment(Qt::AlignCenter);
    servo0Label->setText("S0 = ");

    servo1Slider = new QSlider(Qt::Horizontal, this);
    servo1Slider->setFixedWidth(150);
    gridLayout->addWidget(servo1Slider,2,3);
    servo1Slider->setTickInterval(1);
    servo1Slider->setMinimum(downLimitBig);
    servo1Slider->setMaximum(upLimitBig);
    servo1Slider->setValue(downLimitBig);

    servo1Label = new QLabel(this);
    servo1Label->setFixedWidth(150);
    gridLayout->addWidget(servo1Label,3,3);
    servo1Label->setAlignment(Qt::AlignCenter);
    servo1Label->setText("S1 = ");


    servo2Slider = new QSlider(Qt::Horizontal, this);
    servo2Slider->setFixedWidth(150);
    gridLayout->addWidget(servo2Slider,2,4);
    servo2Slider->setTickInterval(1);
    servo2Slider->setMinimum(downLimitBig);
    servo2Slider->setMaximum(upLimitBig);
    servo2Slider->setValue(downLimitBig);

    servo2Label = new QLabel(this);
    servo2Label->setFixedWidth(150);
    gridLayout->addWidget(servo2Label,3,4);
    servo2Label->setAlignment(Qt::AlignCenter);
    servo2Label->setText("S2 = ");

    QPushButton *servo0animationButton = new QPushButton("Start animation");
    servo0animationButton->setFixedWidth(150);
    gridLayout->addWidget(servo0animationButton,4,2);

    QPushButton *servo0poweOffButton = new QPushButton("Power OFF");
    servo0poweOffButton->setFixedWidth(150);
    gridLayout->addWidget(servo0poweOffButton,5,2);

    QPushButton *servo1animationButton = new QPushButton("Start animation");
    servo1animationButton->setFixedWidth(150);
    gridLayout->addWidget(servo1animationButton,4,3);

    QPushButton *servo1poweOffButton = new QPushButton("Power OFF");
    servo1poweOffButton->setFixedWidth(150);
    gridLayout->addWidget(servo1poweOffButton,5,3);


    QPushButton *servo2animationButton = new QPushButton("Start animation");
    servo2animationButton->setFixedWidth(150);
    gridLayout->addWidget(servo2animationButton,4,4);

    QPushButton *servo2poweOffButton = new QPushButton("Power OFF");
    servo2poweOffButton->setFixedWidth(150);
    gridLayout->addWidget(servo2poweOffButton,5,4);

    animationServo0 = new QPropertyAnimation(servo0Slider,"sliderPosition");
    animationServo1 = new QPropertyAnimation(servo1Slider,"sliderPosition");
    animationServo2 = new QPropertyAnimation(servo2Slider,"sliderPosition");


    // Terminal
    terminalView = new QTextEdit(this);
    verticalLayout->addWidget(terminalView);
    terminalView->setMinimumWidth(500);
    terminalView->setMinimumHeight(400);
    terminalView->setReadOnly(true);
    terminalView->setTextColor(QColor(0, 255, 0,255));


    QGridLayout *gridUnderTerminalLayout = new QGridLayout();

    verticalLayout->addLayout(gridUnderTerminalLayout);

    //Connection Host
    connectionStatusLabel = new QLabel(this);
    connectionStatusLabel->setFixedWidth(25);
    gridUnderTerminalLayout->addWidget(connectionStatusLabel,0,0);
    connectionStatusLabel->setAlignment(Qt::AlignCenter);
    connectionStatusLabel->setText("");
    connectionStatusLabel->setStyleSheet("background-color:red; border-width:1px; border-radius: 12px;");

    QPushButton *connectServerButton = new QPushButton("Connect Server");
    connectServerButton->setFixedWidth(150);
    gridUnderTerminalLayout->addWidget(connectServerButton,0,1);

    QPushButton *disconnectServerButton = new QPushButton("Disconnect Server");
    disconnectServerButton->setFixedWidth(150);
    gridUnderTerminalLayout->addWidget(disconnectServerButton,0,2);

    hostLineEdit = new QLineEdit();
    hostLineEdit->setFixedWidth(120);
    gridUnderTerminalLayout->addWidget(hostLineEdit,0,3);
    hostLineEdit->setText("10.10.10.40");

    portLineEdit = new QLineEdit();
    portLineEdit->setFixedWidth(60);
    gridUnderTerminalLayout->addWidget(portLineEdit,0,4);
    portLineEdit->setText("2222");



    connect(connectGumbaButton,SIGNAL(clicked()),this,SLOT(connectGumbaClicked()));
    connect(disconnectGumbaButton,SIGNAL(clicked()),this,SLOT(disconnectGumbaClicked()));
    connect(startGumbaApplicationButton,SIGNAL(clicked()),this,SLOT(startGumbaApplicationClicked()));
    connect(toggleRTSButton,SIGNAL(clicked()),this,SLOT(toggleRTSClicked()));
    connect(velocitySlider,SIGNAL(sliderMoved(int)),this,SLOT(velocitySliderChanged(int)));
    connect(initServo,SIGNAL(clicked()),this,SLOT(initServoClicked()));


    connect(servo0Slider,SIGNAL(valueChanged(int)),this,SLOT(servo0SliderChanged(int)));
    //connect(servo0Slider,SIGNAL(valueChanged(int)),this,SLOT(servo0SliderChanged(int)));
    connect(servo1Slider,SIGNAL(valueChanged(int)),this,SLOT(servo1SliderChanged(int)));
    //connect(servo1Slider,SIGNAL(valueChanged(int)),this,SLOT(servo1SliderChanged(int)));
    connect(servo2Slider,SIGNAL(valueChanged(int)),this,SLOT(servo2SliderChanged(int)));
    //connect(servo2Slider,SIGNAL(valueChanged(int)),this,SLOT(servo2SliderChanged(int)));

    connect(servo0animationButton,SIGNAL(clicked()),this,SLOT(servo0animationClicked()));
    connect(servo0poweOffButton,SIGNAL(clicked()),this,SLOT(servo0PowerOffClicked()));
    connect(servo1animationButton,SIGNAL(clicked()),this,SLOT(servo1animationClicked()));
    connect(servo1poweOffButton,SIGNAL(clicked()),this,SLOT(servo1PowerOffClicked()));
    connect(servo2animationButton,SIGNAL(clicked()),this,SLOT(servo2animationClicked()));
    connect(servo2poweOffButton,SIGNAL(clicked()),this,SLOT(servo2PowerOffClicked()));
    connect(servoAllPwmOff,SIGNAL(clicked()),this,SLOT(allSevosPwmOff()));

    connect(connectServerButton,SIGNAL(clicked()),this,SLOT(connectServerClicked()));
    connect(disconnectServerButton,SIGNAL(clicked()),this,SIGNAL(disconnectServer()));


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
    writeToTerminal("Disconnect gumba from server");
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
        writeToTerminal("> speedlevel: 1");
        velocityLabel->setText("v = low");
        break;
    case 2:
        emit sendCommand("RoboterSpeed","2");
        writeToTerminal("> speedlevel: 2");
        velocityLabel->setText("v = med");
        break;
    case 3:
        emit sendCommand("RoboterSpeed","3");
        writeToTerminal("> speedlevel: 3");
        velocityLabel->setText("v = high");
        break;
    case 4:
        emit sendCommand("RoboterSpeed","4");
        writeToTerminal("> speedlevel: MAX");
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
    //if(pos%2==0){
    emit sendCommand("Servo1",QString::number(pos));
    //}
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
    //if(pos%2==0){
        emit sendCommand("Servo2",QString::number(pos));
    //}
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
    qDebug() << "all movements stopped, als pwm's resetted!!";
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

void MainWindow::writeToTerminal(const QString &terminalString){
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


