#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>
#include <QColor>
#include <QDebug>
#include <QKeyEvent>
#include <QIcon>



#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_angle0 = (downLimitBig+upLimitBig)/2;
    m_angle1 = (downLimitBig+upLimitBig)/2;
    m_angle2 = (downLimitBig+upLimitBig)/2;
    m_angle3 = (downLimitBig+upLimitBig)/2;
    m_angle4 = (downLimitSmall+upLimitSmall)/2;
    m_angle4 = (downLimitSmall+upLimitSmall)/2;

    m_wiiMoteABstate = false;

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    m_visualisation = new RobotVisualisation();
    //setCentralWidget(m_visualisation);

    setWindowTitle("Gumba Client");
    setWindowIcon(QIcon("gumbaclient80.png"));

    QGridLayout *mainGridLayout = new QGridLayout(this);
    centralWidget->setLayout(mainGridLayout);

    mainGridLayout->addWidget(createGumbaServerConnectionGroupBox(),0,0);
    mainGridLayout->addWidget(createTerminalGroupBox(),1,0);
    mainGridLayout->addWidget(createRobotWidget(),0,1,2,1);


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

QWidget *MainWindow::createGumbaGroupBox()
{
    QWidget *gumbaWidget = new QWidget();
    QGridLayout *gumbaGrid = new QGridLayout;
    gumbaWidget->setLayout(gumbaGrid);

    gumbaGrid->addWidget(createGumbaConnectionGroupBox(),0,0);
    gumbaGrid->addWidget(createGumbaSensorDataGroupBox(),1,0);

    return gumbaWidget;
}

QWidget *MainWindow::createRobotarmGroupBox()
{
    QWidget *robotarmWidget = new QWidget();
    QGridLayout *robotarmGrid = new QGridLayout;
    robotarmWidget->setLayout(robotarmGrid);

    robotarmGrid->addWidget(createServoControllGroupBox(),0,0);
    //robotarmGrid->addWidget(createWiiMoteGroupBox(),1,0);

    return robotarmWidget;
}

QWidget *MainWindow::createRobotWidget()
{
    QWidget *robotWidget = new QWidget();
    QTabWidget *robotTabs = new QTabWidget(this);
    QVBoxLayout *robotLayout = new QVBoxLayout();
    robotLayout->setSizeConstraint(QLayout::SetNoConstraint);

    robotTabs->addTab(createGumbaGroupBox(),tr("Gumba Controll"));
    robotTabs->addTab(createRobotarmGroupBox(),tr("Robotarm Controll"));
    robotTabs->addTab(createWiiMoteGroupBox(),tr("Wii Controll"));

    robotLayout->addWidget(robotTabs);
    robotWidget->setLayout(robotLayout);

    return robotWidget;
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
    motorRightLabel->setFixedWidth(150);

    lightLeftLabel = new QLabel(this);
    lightLeftLabel->setText("Light L: 0");

    lightRightLabel = new QLabel(this);
    lightRightLabel->setText("Light R: 0");


    gumbaBatteryBar = new QProgressBar();
    gumbaBatteryBar->setFixedWidth(150);
    gumbaBatteryBar->setMinimum(550);
    gumbaBatteryBar->setMaximum(880);
    gumbaBatteryBar->setOrientation(Qt::Horizontal);

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
    gumbaSensorDataGrid->addWidget(gumbaBatteryBar,4,0);
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
    gumbaServerConnectionGrid->addWidget(disconnectServerButton,0,1);
    gumbaServerConnectionGrid->addWidget(connectionStatusLabel,0,2);
    gumbaServerConnectionGrid->addWidget(hostLineEdit,0,3);
    gumbaServerConnectionGrid->addWidget(portLineEdit,0,4);

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
    QPushButton *servoHomePositionButton = new QPushButton("HomePosition");

    QPushButton *servoAllPwmOff = new QPushButton("All Servos OFF");
    QPushButton *servo0animationButton = new QPushButton("Start animation");
    QPushButton *servo0poweOffButton = new QPushButton("Power OFF");
    QPushButton *servo1animationButton = new QPushButton("Start animation");
    QPushButton *servo1poweOffButton = new QPushButton("Power OFF");
    QPushButton *servo2animationButton = new QPushButton("Start animation");
    QPushButton *servo2poweOffButton = new QPushButton("Power OFF");
    QPushButton *servo3animationButton = new QPushButton("Start animation");
    QPushButton *servo3poweOffButton = new QPushButton("Power OFF");
    QPushButton *servo4animationButton = new QPushButton("Start animation");
    QPushButton *servo4poweOffButton = new QPushButton("Power OFF");
    QPushButton *servo5animationButton = new QPushButton("Start animation");
    QPushButton *servo5poweOffButton = new QPushButton("Power OFF");


    QGroupBox *servo0GroupBox = new QGroupBox(tr("Servo 0"),this);
    QGroupBox *servo1GroupBox = new QGroupBox(tr("Servo 1"),this);
    QGroupBox *servo2GroupBox = new QGroupBox(tr("Servo 2"),this);
    QGroupBox *servo3GroupBox = new QGroupBox(tr("Servo 3"),this);
    QGroupBox *servo4GroupBox = new QGroupBox(tr("Servo 4"),this);
    QGroupBox *servo5GroupBox = new QGroupBox(tr("Servo 5"),this);

    QGridLayout *servo0Grid = new QGridLayout(this);
    QGridLayout *servo1Grid = new QGridLayout(this);
    QGridLayout *servo2Grid = new QGridLayout(this);
    QGridLayout *servo3Grid = new QGridLayout(this);
    QGridLayout *servo4Grid = new QGridLayout(this);
    QGridLayout *servo5Grid = new QGridLayout(this);

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
    servo0Slider->setMinimum(downLimitBig);
    servo0Slider->setMaximum(upLimitBig);
    servo0Slider->setValue((downLimitBig+upLimitBig)/2);

    servo0Label = new QLabel(this);
    servo0Label->setFixedWidth(60);
    servo0Label->setAlignment(Qt::AlignCenter);
    servo0Label->setText("S0 = OFF");

    // ===================================================== Servo 1
    servo1Slider = new QSlider(Qt::Horizontal, this);
    servo1Slider->setFixedWidth(100);
    servo1Slider->setTickInterval(1);
    servo1Slider->setMinimum(downLimitBig);
    servo1Slider->setMaximum(upLimitBig);
    servo1Slider->setValue((downLimitBig+upLimitBig)/2);

    servo1Label = new QLabel(this);
    servo1Label->setAlignment(Qt::AlignCenter);
    servo1Label->setText("S1 = OFF");

    // ===================================================== Servo 2
    servo2Slider = new QSlider(Qt::Horizontal, this);
    servo2Slider->setFixedWidth(100);
    servo2Slider->setTickInterval(1);
    servo2Slider->setMinimum(downLimitBig);
    servo2Slider->setMaximum(upLimitBig);
    servo2Slider->setValue((downLimitBig+upLimitBig)/2);

    servo2Label = new QLabel(this);
    servo2Label->setAlignment(Qt::AlignCenter);
    servo2Label->setText("S2 = OFF");


    // ===================================================== Servo 3
    servo3Slider = new QSlider(Qt::Horizontal, this);
    servo3Slider->setFixedWidth(100);
    servo3Slider->setTickInterval(1);
    servo3Slider->setMinimum(downLimitBig);
    servo3Slider->setMaximum(upLimitBig);
    servo3Slider->setValue((downLimitBig+upLimitBig)/2);

    servo3Label = new QLabel(this);
    servo3Label->setAlignment(Qt::AlignCenter);
    servo3Label->setText("S3 = OFF");

    // ===================================================== Servo 4
    servo4Slider = new QSlider(Qt::Horizontal, this);
    servo4Slider->setFixedWidth(100);
    servo4Slider->setTickInterval(1);
    servo4Slider->setMinimum(downLimitSmall);
    servo4Slider->setMaximum(upLimitSmall);
    servo4Slider->setValue((downLimitSmall+upLimitSmall)/2);

    servo4Label = new QLabel(this);
    servo4Label->setAlignment(Qt::AlignCenter);
    servo4Label->setText("S4 = OFF");

    // ===================================================== Servo 5
    servo5Slider = new QSlider(Qt::Horizontal, this);
    servo5Slider->setFixedWidth(100);
    servo5Slider->setTickInterval(1);
    servo5Slider->setMinimum(downLimitSmall);
    servo5Slider->setMaximum(upLimitSmall);
    servo5Slider->setValue((downLimitSmall+upLimitSmall)/2);

    servo5Label = new QLabel(this);
    servo5Label->setAlignment(Qt::AlignCenter);
    servo5Label->setText("S5 = OFF");

    animationServo0 = new QPropertyAnimation(servo0Slider,"sliderPosition");
    animationServo1 = new QPropertyAnimation(servo1Slider,"sliderPosition");
    animationServo2 = new QPropertyAnimation(servo2Slider,"sliderPosition");
    animationServo3 = new QPropertyAnimation(servo3Slider,"sliderPosition");
    animationServo4 = new QPropertyAnimation(servo4Slider,"sliderPosition");
    animationServo5 = new QPropertyAnimation(servo5Slider,"sliderPosition");

    servo0Grid->addWidget(servo0Slider,0,0);
    servo0Grid->addWidget(servo0Label,0,1);
    servo0Grid->addWidget(servo0animationButton,1,0);
    servo0Grid->addWidget(servo0poweOffButton,1,1);
    servo0GroupBox->setLayout(servo0Grid);

    servo1Grid->addWidget(servo1Slider,0,0);
    servo1Grid->addWidget(servo1Label,0,1);
    servo1Grid->addWidget(servo1animationButton,1,0);
    servo1Grid->addWidget(servo1poweOffButton,1,1);
    servo1GroupBox->setLayout(servo1Grid);

    servo2Grid->addWidget(servo2Slider,0,0);
    servo2Grid->addWidget(servo2Label,0,1);
    servo2Grid->addWidget(servo2animationButton,1,0);
    servo2Grid->addWidget(servo2poweOffButton,1,1);
    servo2GroupBox->setLayout(servo2Grid);

    servo3Grid->addWidget(servo3Slider,0,0);
    servo3Grid->addWidget(servo3Label,0,1);
    servo3Grid->addWidget(servo3animationButton,1,0);
    servo3Grid->addWidget(servo3poweOffButton,1,1);
    servo3GroupBox->setLayout(servo3Grid);

    servo4Grid->addWidget(servo4Slider,0,0);
    servo4Grid->addWidget(servo4Label,0,1);
    servo4Grid->addWidget(servo4animationButton,1,0);
    servo4Grid->addWidget(servo4poweOffButton,1,1);
    servo4GroupBox->setLayout(servo4Grid);

    servo5Grid->addWidget(servo5Slider,0,0);
    servo5Grid->addWidget(servo5Label,0,1);
    servo5Grid->addWidget(servo5animationButton,1,0);
    servo5Grid->addWidget(servo5poweOffButton,1,1);
    servo5GroupBox->setLayout(servo5Grid);

    servoControllGroupBox->setLayout(servoControllGrid);

    servoControllGrid->addWidget(initServo,0,0);
    servoControllGrid->addWidget(servoAllPwmOff,1,0);
    servoControllGrid->addWidget(servoHomePositionButton,2,0);

    servoControllGrid->addWidget(servo0GroupBox,3,0);
    servoControllGrid->addWidget(servo1GroupBox,4,0);
    servoControllGrid->addWidget(servo2GroupBox,5,0);
    servoControllGrid->addWidget(servo3GroupBox,6,0);
    servoControllGrid->addWidget(servo4GroupBox,7,0);
    servoControllGrid->addWidget(servo5GroupBox,8,0);

    connect(initServo,SIGNAL(clicked()),this,SLOT(initServoClicked()));
    connect(servoHomePositionButton,SIGNAL(clicked()),this,SLOT(servoHomePositionClicked()));
    connect(servoAllPwmOff,SIGNAL(clicked()),this,SLOT(allSevosPwmOff()));

    connect(servo0Slider,SIGNAL(valueChanged(int)),this,SLOT(servo0SliderChanged(int)));
    connect(servo1Slider,SIGNAL(valueChanged(int)),this,SLOT(servo1SliderChanged(int)));
    connect(servo2Slider,SIGNAL(valueChanged(int)),this,SLOT(servo2SliderChanged(int)));
    connect(servo3Slider,SIGNAL(valueChanged(int)),this,SLOT(servo3SliderChanged(int)));
    connect(servo4Slider,SIGNAL(valueChanged(int)),this,SLOT(servo4SliderChanged(int)));
    connect(servo5Slider,SIGNAL(valueChanged(int)),this,SLOT(servo5SliderChanged(int)));

    connect(servo0animationButton,SIGNAL(clicked()),this,SLOT(servo0animationClicked()));
    connect(servo1animationButton,SIGNAL(clicked()),this,SLOT(servo1animationClicked()));
    connect(servo2animationButton,SIGNAL(clicked()),this,SLOT(servo2animationClicked()));
    connect(servo3animationButton,SIGNAL(clicked()),this,SLOT(servo3animationClicked()));
    connect(servo4animationButton,SIGNAL(clicked()),this,SLOT(servo4animationClicked()));
    connect(servo5animationButton,SIGNAL(clicked()),this,SLOT(servo5animationClicked()));

    connect(servo0poweOffButton,SIGNAL(clicked()),this,SLOT(servo0PowerOffClicked()));
    connect(servo1poweOffButton,SIGNAL(clicked()),this,SLOT(servo1PowerOffClicked()));
    connect(servo2poweOffButton,SIGNAL(clicked()),this,SLOT(servo2PowerOffClicked()));
    connect(servo3poweOffButton,SIGNAL(clicked()),this,SLOT(servo3PowerOffClicked()));
    connect(servo4poweOffButton,SIGNAL(clicked()),this,SLOT(servo4PowerOffClicked()));
    connect(servo5poweOffButton,SIGNAL(clicked()),this,SLOT(servo5PowerOffClicked()));

    return servoControllGroupBox;
}

QGroupBox *MainWindow::createTerminalGroupBox()
{
    QGroupBox *terminalGroupBox = new QGroupBox(tr("Terminal"),this);
    QVBoxLayout *terminalLayout = new QVBoxLayout;

    terminalLayout->setSizeConstraint(QLayout::SetNoConstraint);

    QTabWidget *tabs = new QTabWidget();

    // Terminal
    terminalView = new QTextEdit(this);
    terminalView->setMinimumWidth(550);
    terminalView->setMinimumHeight(400);
    terminalView->setReadOnly(true);
    terminalView->setTextColor(QColor(0, 255, 0,255));

    tabs->addTab(m_visualisation,tr("Robotarm"));
    tabs->addTab(terminalView,tr("Terminal"));

    terminalLayout->addWidget(tabs);
    terminalGroupBox->setLayout(terminalLayout);

    return terminalGroupBox;
}

QWidget *MainWindow::createWiiMoteGroupBox()
{
    QWidget *wiiWidget = new QWidget();
    QVBoxLayout *wiiWidgetLayout = new QVBoxLayout();

    wiiWidget->setLayout(wiiWidgetLayout);

    QPushButton *startWiiMoteButton = new QPushButton("Start WiiMote");
    QPushButton *disconnectWiiMoteButton = new QPushButton("Disconnct WiiMote");

    QGroupBox *wiiGroupBox = new QGroupBox(tr("Wii"),this);
    QGroupBox *nunchuckGroupBox = new QGroupBox(tr("Nunchuck"),this);
    QGroupBox *classicGroupBox = new QGroupBox(tr("Classic Controller"),this);
    QGroupBox *guitarHeroGroupBox = new QGroupBox(tr("Guitar Hero"),this);

    QGridLayout *wiiGridLayout = new QGridLayout;
    QGridLayout *nunchuckGridLayout = new QGridLayout;
    QGridLayout *classicGridLayout = new QGridLayout;
    QGridLayout *guitarHeroGridLayout = new QGridLayout;

    wiiGroupBox->setLayout(wiiGridLayout);
    nunchuckGroupBox->setLayout(nunchuckGridLayout);
    classicGroupBox->setLayout(classicGridLayout);
    guitarHeroGroupBox->setLayout(guitarHeroGridLayout);

    // ============================================================ Wii

    wiiBatteryBar = new QProgressBar();
    wiiBatteryBar->setFixedWidth(140);
    wiiBatteryBar->setMinimum(0);
    wiiBatteryBar->setMaximum(100);
    wiiBatteryBar->setOrientation(Qt::Horizontal);

    wiiMoteRoll = new QLabel(this);
    wiiMoteRoll->setFixedWidth(140);
    wiiMoteRoll->setAlignment(Qt::AlignLeft);
    wiiMoteRoll->setText("Roll = 0");

    wiiMotePitch = new QLabel(this);
    wiiMotePitch->setFixedWidth(140);
    wiiMotePitch->setAlignment(Qt::AlignLeft);
    wiiMotePitch->setText("Pitch = 0");

    wiiMoteYaw = new QLabel(this);
    wiiMoteYaw->setFixedWidth(140);
    wiiMoteYaw->setAlignment(Qt::AlignLeft);
    wiiMoteYaw->setText("Yaw = 0");

    wiiGridLayout->addWidget(startWiiMoteButton,0,0);
    wiiGridLayout->addWidget(disconnectWiiMoteButton,1,0);
    wiiGridLayout->addWidget(wiiBatteryBar,2,0);

    wiiGridLayout->addWidget(wiiMoteRoll,0,1);
    wiiGridLayout->addWidget(wiiMotePitch,1,1);
    wiiGridLayout->addWidget(wiiMoteYaw,2,1);

    // ============================================================ Nunchuck
    nunchukRoll = new QLabel(this);
    nunchukRoll->setFixedWidth(140);
    nunchukRoll->setAlignment(Qt::AlignLeft);
    nunchukRoll->setText("Roll = 0");

    nunchukPitch = new QLabel(this);
    nunchukPitch->setFixedWidth(140);
    nunchukPitch->setAlignment(Qt::AlignLeft);
    nunchukPitch->setText("Pitch = 0");

    nunchukYaw = new QLabel(this);
    nunchukYaw->setFixedWidth(140);
    nunchukYaw->setAlignment(Qt::AlignLeft);
    nunchukYaw->setText("Yaw = 0");

    nunchukAngle = new QLabel(this);
    nunchukAngle->setFixedWidth(140);
    nunchukAngle->setAlignment(Qt::AlignLeft);
    nunchukAngle->setText("Angle = 0");

    nunchukMagnitude = new QLabel(this);
    nunchukMagnitude->setFixedWidth(140);
    nunchukMagnitude->setAlignment(Qt::AlignLeft);
    nunchukMagnitude->setText("Magnitude = 0");

    nunchuckGridLayout->addWidget(nunchukRoll,0,0);
    nunchuckGridLayout->addWidget(nunchukPitch,1,0);
    nunchuckGridLayout->addWidget(nunchukYaw,2,0);
    nunchuckGridLayout->addWidget(nunchukAngle,0,1);
    nunchuckGridLayout->addWidget(nunchukMagnitude,1,1);


    // ============================================================ Classic controller
    classicLeftAngle = new QLabel(this);
    classicLeftAngle->setFixedWidth(140);
    classicLeftAngle->setAlignment(Qt::AlignLeft);
    classicLeftAngle->setText("Angle = 0");

    classicLeftMagnitude = new QLabel(this);
    classicLeftMagnitude->setFixedWidth(140);
    classicLeftMagnitude->setAlignment(Qt::AlignLeft);
    classicLeftMagnitude->setText("Magnitude = 0");

    classicRightAngle = new QLabel(this);
    classicRightAngle->setFixedWidth(140);
    classicRightAngle->setAlignment(Qt::AlignLeft);
    classicRightAngle->setText("Angle = 0");

    classicRightMagnitude = new QLabel(this);
    classicRightMagnitude->setFixedWidth(140);
    classicRightMagnitude->setAlignment(Qt::AlignLeft);
    classicRightMagnitude->setText("Magnitude = 0");

    classicGridLayout->addWidget(classicLeftAngle,0,0);
    classicGridLayout->addWidget(classicLeftMagnitude,1,0);
    classicGridLayout->addWidget(classicRightAngle,0,1);
    classicGridLayout->addWidget(classicRightMagnitude,1,1);


    // ============================================================ Guitar Hero
    guitarWhaa = new QLabel(this);
    guitarWhaa->setFixedWidth(140);
    guitarWhaa->setAlignment(Qt::AlignLeft);
    guitarWhaa->setText("Whaa: 0");

    guitarAngle = new QLabel(this);
    guitarAngle->setFixedWidth(140);
    guitarAngle->setAlignment(Qt::AlignLeft);
    guitarAngle->setText("Angle = 0");

    guitarMagnitude = new QLabel(this);
    guitarMagnitude->setFixedWidth(140);
    guitarMagnitude->setAlignment(Qt::AlignLeft);
    guitarMagnitude->setText("Magnitude = 0");

    guitarHeroGridLayout->addWidget(guitarWhaa,0,0);
    guitarHeroGridLayout->addWidget(guitarAngle,0,1);
    guitarHeroGridLayout->addWidget(guitarMagnitude,1,1);

    // ============================================================
    wiiWidgetLayout->addWidget(wiiGroupBox);
    wiiWidgetLayout->addWidget(nunchuckGroupBox);
    wiiWidgetLayout->addWidget(classicGroupBox);
    wiiWidgetLayout->addWidget(guitarHeroGroupBox);


    connect(startWiiMoteButton,SIGNAL(clicked()),this,SIGNAL(startWiiMote()))   ;
    connect(disconnectWiiMoteButton,SIGNAL(clicked()),this,SIGNAL(stopWiiMote()));

    return wiiWidget;
}

float MainWindow::convertPwmToDegreeBig(int pwm)
{
    // up-limit     = 249 ms = +90
    // down-limit   = 57  ms = -90
    return (180*(pwm-downLimitBig)/(upLimitBig-downLimitBig))-90;
}

float MainWindow::convertPwmToDegreeSmall(int pwm)
{
    // up-limit     = 249 ms = +90
    // down-limit   = 57  ms = -90
    return (180*(pwm-downLimitSmall)/(upLimitSmall-downLimitSmall))-90;
}

int MainWindow::convertDegreeToPwmBig(float degree)
{
    return ((degree-90)*(upLimitBig-downLimitBig)-(180*downLimitBig))/180;
}

int MainWindow::convertDegreeToPwmSmall(float degree)
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
    servo0Label->setText("S0 = " + QString::number(convertPwmToDegreeBig(pos)));
    emit sendCommand("Servo0",QString::number(pos));
    m_angle0 = (pos);
    m_visualisation->updateservo0(convertPwmToDegreeBig(pos));
}

void MainWindow::servo0animationClicked()
{
    moveServo(0,m_angle0,(downLimitBig+upLimitBig)/2,periodMove);

}

void MainWindow::servo0PowerOffClicked()
{
    servo0Label->setText("S0 = OFF");
    animationServo0->stop();
    emit sendCommand("Servo0",QString::number(0));
}

void MainWindow::servo1SliderChanged(const int &pos)
{
    servo1Label->setText("S1 = " + QString::number(convertPwmToDegreeBig(pos)));
    emit sendCommand("Servo1",QString::number(pos));
    m_angle1 = (pos);
    m_visualisation->updateservo1(convertPwmToDegreeBig(pos));

}

void MainWindow::servo1animationClicked()
{
    moveServo(1,m_angle1,(downLimitBig+upLimitBig)/2,periodMove);
}

void MainWindow::servo1PowerOffClicked()
{
    servo1Label->setText("S1 = OFF");
    animationServo1->stop();
    emit sendCommand("Servo1",QString::number(0));

}

void MainWindow::servo2SliderChanged(const int &pos)
{
    servo2Label->setText("S2 = " + QString::number(convertPwmToDegreeBig(pos)));
    emit sendCommand("Servo2",QString::number(pos));
    m_angle2 = pos;
    m_visualisation->updateservo2(convertPwmToDegreeBig(pos));

}

void MainWindow::servo2animationClicked()
{
    moveServo(2,m_angle2,(downLimitBig+upLimitBig)/2,periodMove);
}

void MainWindow::servo2PowerOffClicked()
{
    servo2Label->setText("S2 = OFF");
    animationServo2->stop();
    emit sendCommand("Servo2",QString::number(0));
}

void MainWindow::servo3SliderChanged(const int &pos)
{
    servo3Label->setText("S3 = " + QString::number(convertPwmToDegreeBig(pos)));
    emit sendCommand("Servo3",QString::number(pos));
    m_angle3 = (pos);
    m_visualisation->updateservo3(convertPwmToDegreeBig(pos));
}

void MainWindow::servo3animationClicked()
{
    moveServo(3,m_angle3,(downLimitBig+upLimitBig)/2,periodMove);
}

void MainWindow::servo3PowerOffClicked()
{
    servo3Label->setText("S3 = OFF");
    animationServo3->stop();
    emit sendCommand("Servo3",QString::number(0));
}

void MainWindow::servo4SliderChanged(const int &pos)
{
    servo4Label->setText("S4 = " + QString::number(convertPwmToDegreeSmall(pos)));
    emit sendCommand("Servo4",QString::number(pos));
    m_angle4 = (pos);
    m_visualisation->updateservo4(convertPwmToDegreeSmall(pos));
}

void MainWindow::servo4animationClicked()
{
    moveServo(4,m_angle4,(downLimitSmall+upLimitSmall)/2,periodMove);
}

void MainWindow::servo4PowerOffClicked()
{
    servo4Label->setText("S4 = OFF");
    animationServo4->stop();
    emit sendCommand("Servo4",QString::number(0));
}

void MainWindow::servo5SliderChanged(const int &pos)
{
    servo5Label->setText("S5 = " + QString::number(convertPwmToDegreeSmall(pos)));
    emit sendCommand("Servo5",QString::number(pos));
    m_angle5 = (pos);
    m_visualisation->updateservo5(convertPwmToDegreeSmall(pos));
}

void MainWindow::servo5animationClicked()
{
    moveServo(5,m_angle5,(downLimitSmall+upLimitSmall)/2,periodMove);
}

void MainWindow::servo5PowerOffClicked()
{
    servo5Label->setText("S5 = OFF");
    animationServo5->stop();
    emit sendCommand("Servo5",QString::number(0));
}

void MainWindow::allSevosPwmOff()
{
    animationServo0->stop();
    animationServo1->stop();
    animationServo2->stop();
    animationServo3->stop();
    animationServo4->stop();
    animationServo5->stop();

    servo0PowerOffClicked();
    servo1PowerOffClicked();
    servo2PowerOffClicked();
    servo3PowerOffClicked();
    servo4PowerOffClicked();
    servo5PowerOffClicked();

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
    case 3:
        animationServo3->setDuration(period);
        animationServo3->setStartValue(start);
        animationServo3->setEndValue(end);
        animationServo3->setEasingCurve(QEasingCurve::InOutQuint);
        animationServo3->start();
        break;
    case 4:
        animationServo4->setDuration(period);
        animationServo4->setStartValue(start);
        animationServo4->setEndValue(end);
        animationServo4->setEasingCurve(QEasingCurve::InOutQuint);
        animationServo4->start();
        break;
    case 5:
        animationServo5->setDuration(period);
        animationServo5->setStartValue(start);
        animationServo5->setEndValue(end);
        animationServo5->setEasingCurve(QEasingCurve::InOutQuint);
        animationServo5->start();
        break;
    default:
        break;
    }
}


void MainWindow::initServoClicked()
{
    emit sendCommand("Servo","init");
}

void MainWindow::servoHomePositionClicked()
{
    moveServo(0,(m_angle0),(downLimitBig+upLimitBig)/2,periodMove);
    moveServo(1,(m_angle1),(downLimitBig+upLimitBig)/2,periodMove);
    moveServo(2,(m_angle2),(downLimitBig+upLimitBig)/2,periodMove);
    moveServo(3,(m_angle3),(downLimitBig+upLimitBig)/2,periodMove);
    moveServo(4,(m_angle4),(downLimitSmall+upLimitSmall)/2,periodMove);
    moveServo(5,(m_angle5),(downLimitSmall+upLimitSmall)/2,periodMove);

}

void MainWindow::updateWiiMoteOrientation(const float &roll, const float &pitch, const float &yaw)
{
    int roll_i, pitch_i, yaw_i;
    roll_i = roll;
    pitch_i = pitch;
    yaw_i = yaw;
    wiiMoteRoll->setText("Roll = " + QString::number(roll_i));
    wiiMotePitch->setText("Pitch = " + QString::number(pitch_i));
    wiiMoteYaw->setText("Yaw = " + QString::number(yaw_i));

    if(m_wiiMoteABstate){
        servo2Slider->setValue(pitch_i*(-1)+50);
    }


}

void MainWindow::updateNunchuckOrientation(const float &roll, const float &pitch, const float &yaw)
{
    int roll_i, pitch_i, yaw_i;
    roll_i = roll;
    pitch_i = pitch;
    yaw_i = yaw;
    nunchukRoll->setText("Roll = " + QString::number(roll_i));
    nunchukPitch->setText("Pitch = " + QString::number(pitch_i));
    nunchukYaw->setText("Yaw = " + QString::number(yaw_i));

    if(m_wiiMoteABstate){
        servo0Slider->setValue(pitch_i*(-1)+100);
    }
}

void MainWindow::updateNunchuckJoyStickData(const float &angle, const float &magnitude)
{
    nunchukAngle->setText("Angle = " + QString::number(angle));
    nunchukMagnitude->setText("Magnitude = " + QString::number(magnitude));
}

void MainWindow::updateClassicLeftJoyStickData(const float &angle, const float &magnitude)
{
    classicLeftAngle->setText("Angle = " + QString::number(angle));
    classicLeftMagnitude->setText("Magnitude = " + QString::number(magnitude));
}

void MainWindow::updateClassicRightJoyStickData(const float &angle, const float &magnitude)
{
    classicRightAngle->setText("Angle = " + QString::number(angle));
    classicRightMagnitude->setText("Magnitude = " + QString::number(magnitude));
}

void MainWindow::updateGuitarHeroJoyStickData(const float &angle, const float &magnitude)
{
    guitarAngle->setText("Angle = " + QString::number(angle));
    guitarMagnitude->setText("Magnitude = " + QString::number(magnitude));}

void MainWindow::updateGuitarHeroWhaaData(const float &whaa)
{
    guitarWhaa->setText("Whaa = " + QString::number(whaa));

}

void MainWindow::updateWiiBattery(const float &battery)
{
    int batteryPercent = battery*100;
//    if(batteryPercent >= 30){
//        wiiBatteryBar->setStyleSheet(wiiBatteryBar->property("defaultStyleSheet").toString() +" QProgressBar::chunk { background: green;}");
//    }else{
//        if(batteryPercent < 30){
//            wiiBatteryBar->setStyleSheet(wiiBatteryBar->property("defaultStyleSheet").toString() +" QProgressBar::chunk { background: yellow;}");
//        }
//        if(batteryPercent < 15){
//            wiiBatteryBar->setStyleSheet(wiiBatteryBar->property("defaultStyleSheet").toString() +" QProgressBar::chunk { background: red;}");
//        }
//    }
    wiiBatteryBar->setValue(batteryPercent);
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
    gumbaBatteryBar->setValue(battery);
}

void MainWindow::wiiMoteABChanged(const bool &pressedState)
{
    m_wiiMoteABstate = pressedState;
}


