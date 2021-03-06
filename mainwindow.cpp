#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>
#include <QColor>
#include <QDebug>
#include <QKeyEvent>
#include <QComboBox>
#include <QIcon>
#include <math.h>


#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_angle0 = M_PI_2;
    m_angle1 = M_PI;
    m_angle2 = M_PI_2+M_PI_2/2;
    m_angle3 = M_PI_2;
    m_angle4 = M_PI_2+M_PI_2/2;
    m_angle5 = M_PI_2;

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

    connect(this,SIGNAL(zoomIn()),m_visualisation,SLOT(zoomIn()));
    connect(this,SIGNAL(zoomOut()),m_visualisation,SLOT(zoomOut()));

    emit calculatePosition(m_angle0,m_angle1,m_angle2,m_angle3,m_angle4,m_angle5);
    updateAllAngles();
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
    case Qt::Key_Plus:
        emit zoomIn();
        break;
    case Qt::Key_Minus:
        emit zoomOut();
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
    QWidget *robotWidget = new QWidget(this);
    QTabWidget *robotTabs = new QTabWidget(this);
    QVBoxLayout *robotLayout = new QVBoxLayout();
    robotLayout->setSizeConstraint(QLayout::SetNoConstraint);

    robotTabs->addTab(createRobotarmGroupBox(),tr("Robotarm Controll"));
    robotTabs->addTab(createGumbaGroupBox(),tr("Gumba Controll"));
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

    speedLeftLabel = new QLabel(this);
    speedLeftLabel->setText("Speed L: 0");

    speedRightLabel = new QLabel(this);
    speedRightLabel->setText("Speed R: 0");

    lightLeftLabel = new QLabel(this);
    lightLeftLabel->setText("Light L: 0");

    lightRightLabel = new QLabel(this);
    lightRightLabel->setText("Light R: 0");


    gumbaBatteryBar = new QProgressBar();
    gumbaBatteryBar->setFixedWidth(150);
    gumbaBatteryBar->setMinimum(550);
    gumbaBatteryBar->setMaximum(880);
    gumbaBatteryBar->setOrientation(Qt::Horizontal);

    gumbaSensorDataGrid->addWidget(bumperLeftLabel,0,0);
    gumbaSensorDataGrid->addWidget(bumperRightLabel,0,1);
    gumbaSensorDataGrid->addWidget(obstacleLeftLabel,1,0);
    gumbaSensorDataGrid->addWidget(obstacleRightLabel,1,1);
    gumbaSensorDataGrid->addWidget(motorLeftLabel,2,0);
    gumbaSensorDataGrid->addWidget(motorRightLabel,2,1);
    gumbaSensorDataGrid->addWidget(speedLeftLabel,3,0);
    gumbaSensorDataGrid->addWidget(speedRightLabel,3,1);
    gumbaSensorDataGrid->addWidget(lightLeftLabel,4,0);
    gumbaSensorDataGrid->addWidget(lightRightLabel,4,1);
    gumbaSensorDataGrid->addWidget(gumbaBatteryBar,5,0);


    gumbaSensorDataGroupBox->setLayout(gumbaSensorDataGrid);

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


    QGroupBox *servo0GroupBox = new QGroupBox(tr("Joint 1"),this);
    QGroupBox *servo1GroupBox = new QGroupBox(tr("Joint 2"),this);
    QGroupBox *servo2GroupBox = new QGroupBox(tr("Joint 3"),this);
    QGroupBox *servo3GroupBox = new QGroupBox(tr("Joint 4"),this);
    QGroupBox *servo4GroupBox = new QGroupBox(tr("Joint 5"),this);
    QGroupBox *servo5GroupBox = new QGroupBox(tr("Joint 6"),this);

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
    servo0Slider->setTickInterval(100);
    servo0Slider->setMinimum(0);
    servo0Slider->setMaximum(M_PI*100000);
    servo0Slider->setValue(M_PI_2*100000);

    servo0Label = new QLabel(this);
    servo0Label->setFixedWidth(60);
    servo0Label->setAlignment(Qt::AlignCenter);
    servo0Label->setText("S0 = OFF");

    // ===================================================== Servo 1
    servo1Slider = new QSlider(Qt::Horizontal, this);
    servo1Slider->setFixedWidth(100);
    servo1Slider->setTickInterval(100);
    servo1Slider->setMinimum(0);
    servo1Slider->setMaximum(M_PI*100000);
    servo1Slider->setValue(M_PI_2*100000);

    servo1Label = new QLabel(this);
    servo1Label->setAlignment(Qt::AlignCenter);
    servo1Label->setText("S1 = OFF");

    // ===================================================== Servo 2
    servo2Slider = new QSlider(Qt::Horizontal, this);
    servo2Slider->setFixedWidth(100);
    servo2Slider->setTickInterval(100);
    servo2Slider->setMinimum(0);
    servo2Slider->setMaximum(M_PI*100000);
    servo2Slider->setValue(M_PI_2*100000);

    servo2Label = new QLabel(this);
    servo2Label->setAlignment(Qt::AlignCenter);
    servo2Label->setText("S2 = OFF");

    // ===================================================== Servo 3
    servo3Slider = new QSlider(Qt::Horizontal, this);
    servo3Slider->setFixedWidth(100);
    servo3Slider->setTickInterval(100);
    servo3Slider->setMinimum(0);
    servo3Slider->setMaximum(M_PI*100000);
    servo3Slider->setValue(M_PI_2*100000);

    servo3Label = new QLabel(this);
    servo3Label->setAlignment(Qt::AlignCenter);
    servo3Label->setText("S3 = OFF");

    // ===================================================== Servo 4
    servo4Slider = new QSlider(Qt::Horizontal, this);
    servo4Slider->setFixedWidth(100);
    servo4Slider->setTickInterval(100);
    servo4Slider->setMinimum(0);
    servo4Slider->setMaximum(M_PI*100000);
    servo4Slider->setValue(M_PI_2*100000);

    servo4Label = new QLabel(this);
    servo4Label->setAlignment(Qt::AlignCenter);
    servo4Label->setText("S4 = OFF");

    // ===================================================== Servo 5
    servo5Slider = new QSlider(Qt::Horizontal, this);
    servo5Slider->setFixedWidth(100);
    servo5Slider->setTickInterval(100);
    servo5Slider->setMinimum(0);
    servo5Slider->setMaximum(M_PI*100000);
    servo5Slider->setValue(M_PI_2*100000);

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

    QWidget *buttons = new QWidget(this);
    QVBoxLayout *buttonLayout = new QVBoxLayout(this);
    buttons->setLayout(buttonLayout);

    buttonLayout->addWidget(initServo);
    buttonLayout->addWidget(servoAllPwmOff);
    buttonLayout->addWidget(servoHomePositionButton);
    servoControllGrid->addWidget(buttons,0,1);
    servoControllGrid->addWidget(createServoPositionGroupBox(),1,1);
    servoControllGrid->addWidget(createServoOrientationGroupBox(),2,1);

    QWidget *moveKoordinateSystem = new QWidget(this);
    QVBoxLayout *moveKoordinateSystemLayout = new QVBoxLayout(this);
    moveKoordinateSystem->setLayout(moveKoordinateSystemLayout);
    QLabel *systemLabel = new QLabel("Koordinatesystem",this);
    QComboBox *box = new QComboBox(this);
    box->addItem("Base",0);
    box->addItem("TCP",0);
    moveKoordinateSystemLayout->addWidget(systemLabel);
    moveKoordinateSystemLayout->addWidget(box);

    servoControllGrid->addWidget(moveKoordinateSystem,3,1);
    servoControllGrid->addWidget(createTcpControllGroupBox(),4,1);
    servoControllGrid->addWidget(createTcpControllRotGroupBox(),5,1);

    servoControllGrid->addWidget(servo0GroupBox,0,0);
    servoControllGrid->addWidget(servo1GroupBox,1,0);
    servoControllGrid->addWidget(servo2GroupBox,2,0);
    servoControllGrid->addWidget(servo3GroupBox,3,0);
    servoControllGrid->addWidget(servo4GroupBox,4,0);
    servoControllGrid->addWidget(servo5GroupBox,5,0);

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

    connect(box,SIGNAL(activated(int)),this,SIGNAL(koordinateSystemChanged(int)));
    return servoControllGroupBox;
}

QGroupBox *MainWindow::createServoPositionGroupBox()
{
    QGroupBox *createServoPositionGroupBox = new QGroupBox(tr("TCP Position"),this);
    QVBoxLayout *servoPositionLayout = new QVBoxLayout;
    createServoPositionGroupBox->setLayout(servoPositionLayout);

    tcp_x_Lable = new QLabel(this);
    tcp_x_Lable->setFixedWidth(100);
    tcp_x_Lable->setAlignment(Qt::AlignLeft);
    tcp_x_Lable->setText("x  = ");

    tcp_y_Lable = new QLabel(this);
    tcp_y_Lable->setFixedWidth(100);
    tcp_y_Lable->setAlignment(Qt::AlignLeft);
    tcp_y_Lable->setText("y  = ");

    tcp_z_Lable = new QLabel(this);
    tcp_z_Lable->setFixedWidth(100);
    tcp_z_Lable->setAlignment(Qt::AlignLeft);
    tcp_z_Lable->setText("z  = ");

    servoPositionLayout->addWidget(tcp_x_Lable);
    servoPositionLayout->addWidget(tcp_y_Lable);
    servoPositionLayout->addWidget(tcp_z_Lable);

    return createServoPositionGroupBox;
}

QGroupBox *MainWindow::createServoOrientationGroupBox()
{
    QGroupBox *createServoOrientationGroupBox = new QGroupBox(tr("TCP Orientation"),this);
    QVBoxLayout *servoOrientationLayout = new QVBoxLayout;
    createServoOrientationGroupBox->setLayout(servoOrientationLayout);

    tcp_wx_Lable = new QLabel(this);
    tcp_wx_Lable->setFixedWidth(100);
    tcp_wx_Lable->setAlignment(Qt::AlignLeft);
    tcp_wx_Lable->setText("wx = ");

    tcp_wy_Lable = new QLabel(this);
    tcp_wy_Lable->setFixedWidth(100);
    tcp_wy_Lable->setAlignment(Qt::AlignLeft);
    tcp_wy_Lable->setText("wy = ");

    tcp_wz_Lable = new QLabel(this);
    tcp_wz_Lable->setFixedWidth(100);
    tcp_wz_Lable->setAlignment(Qt::AlignLeft);
    tcp_wz_Lable->setText("wz = ");

    servoOrientationLayout->addWidget(tcp_wx_Lable);
    servoOrientationLayout->addWidget(tcp_wy_Lable);
    servoOrientationLayout->addWidget(tcp_wz_Lable);

    return createServoOrientationGroupBox;
}

QGroupBox *MainWindow::createTcpControllGroupBox()
{
    QGroupBox *tcpControllGroupBox = new QGroupBox(tr("Translation"),this);
    QGridLayout *tcpControllLayout = new QGridLayout(this);
    tcpControllGroupBox->setLayout(tcpControllLayout);

    QPushButton *xPlus = new QPushButton("x +",this);
    QPushButton *xMinus = new QPushButton("x -",this);
    QPushButton *yPlus = new QPushButton("y +",this);
    QPushButton *yMinus = new QPushButton("y -",this);
    QPushButton *zPlus = new QPushButton("z +",this);
    QPushButton *zMinus = new QPushButton("z -",this);

    xPlus->setAutoRepeat(true);
    xMinus->setAutoRepeat(true);
    yPlus->setAutoRepeat(true);
    yMinus->setAutoRepeat(true);
    zPlus->setAutoRepeat(true);
    zMinus->setAutoRepeat(true);

    int delay= 20;
    xPlus->setAutoRepeatInterval(delay);
    xMinus->setAutoRepeatInterval(delay);
    yPlus->setAutoRepeatInterval(delay);
    yMinus->setAutoRepeatInterval(delay);
    zPlus->setAutoRepeatInterval(delay);
    zMinus->setAutoRepeatInterval(delay);

    tcpControllLayout->addWidget(xMinus,0,0);
    tcpControllLayout->addWidget(xPlus,0,1);
    tcpControllLayout->addWidget(yMinus,1,0);
    tcpControllLayout->addWidget(yPlus,1,1);
    tcpControllLayout->addWidget(zMinus,2,0);
    tcpControllLayout->addWidget(zPlus,2,1);

    xPlus->setFixedWidth(50);
    xMinus->setFixedWidth(50);
    yPlus->setFixedWidth(50);
    yMinus->setFixedWidth(50);
    zPlus->setFixedWidth(50);
    zMinus->setFixedWidth(50);

    connect(xPlus,SIGNAL(clicked()),this,SLOT(tcpXPlusClicked()));
    connect(xMinus,SIGNAL(clicked()),this,SLOT(tcpXMinusClicked()));
    connect(yMinus,SIGNAL(clicked()),this,SLOT(tcpYMinusClicked()));
    connect(yPlus,SIGNAL(clicked()),this,SLOT(tcpYPlusClicked()));
    connect(zMinus,SIGNAL(clicked()),this,SLOT(tcpZMinusClicked()));
    connect(zPlus,SIGNAL(clicked()),this,SLOT(tcpZPlusClicked()));

    return tcpControllGroupBox;
}

QGroupBox *MainWindow::createTcpControllRotGroupBox()
{
    QGroupBox *tcpControllGroupBox = new QGroupBox(tr("Rotation"),this);
    QGridLayout *tcpControllLayout = new QGridLayout(this);
    tcpControllGroupBox->setLayout(tcpControllLayout);

    QPushButton *wxPlus = new QPushButton("wx +",this);
    QPushButton *wxMinus = new QPushButton("wx -",this);
    QPushButton *wyPlus = new QPushButton("wy +",this);
    QPushButton *wyMinus = new QPushButton("wy -",this);
    QPushButton *wzPlus = new QPushButton("wz +",this);
    QPushButton *wzMinus = new QPushButton("wz -",this);

    wxPlus->setAutoRepeat(true);
    wxMinus->setAutoRepeat(true);
    wyPlus->setAutoRepeat(true);
    wyMinus->setAutoRepeat(true);
    wzPlus->setAutoRepeat(true);
    wzMinus->setAutoRepeat(true);

    int delay= 20;
    wxPlus->setAutoRepeatInterval(delay);
    wxMinus->setAutoRepeatInterval(delay);
    wyPlus->setAutoRepeatInterval(delay);
    wyMinus->setAutoRepeatInterval(delay);
    wzPlus->setAutoRepeatInterval(delay);
    wzMinus->setAutoRepeatInterval(delay);

    tcpControllLayout->addWidget(wxMinus,0,0);
    tcpControllLayout->addWidget(wxPlus,0,1);
    tcpControllLayout->addWidget(wyMinus,1,0);
    tcpControllLayout->addWidget(wyPlus,1,1);
    tcpControllLayout->addWidget(wzMinus,2,0);
    tcpControllLayout->addWidget(wzPlus,2,1);

    wxPlus->setFixedWidth(50);
    wxMinus->setFixedWidth(50);
    wyPlus->setFixedWidth(50);
    wyMinus->setFixedWidth(50);
    wzPlus->setFixedWidth(50);
    wzMinus->setFixedWidth(50);

    connect(wxPlus,SIGNAL(clicked()),this,SLOT(tcpWXPlusClicked()));
    connect(wxMinus,SIGNAL(clicked()),this,SLOT(tcpWXMinusClicked()));
    connect(wyMinus,SIGNAL(clicked()),this,SLOT(tcpWYMinusClicked()));
    connect(wyPlus,SIGNAL(clicked()),this,SLOT(tcpWYPlusClicked()));
    connect(wzMinus,SIGNAL(clicked()),this,SLOT(tcpWZMinusClicked()));
    connect(wzPlus,SIGNAL(clicked()),this,SLOT(tcpWZPlusClicked()));

    return tcpControllGroupBox;
}

QGroupBox *MainWindow::createTerminalGroupBox()
{
    QGroupBox *terminalGroupBox = new QGroupBox(tr("Terminal"),this);
    QVBoxLayout *terminalLayout = new QVBoxLayout(this);

    terminalLayout->setSizeConstraint(QLayout::SetNoConstraint);

    QTabWidget *tabs = new QTabWidget(this);

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
    QWidget *wiiWidget = new QWidget(this);
    QVBoxLayout *wiiWidgetLayout = new QVBoxLayout(this);

    wiiWidget->setLayout(wiiWidgetLayout);

    QPushButton *startWiiMoteButton = new QPushButton("Start WiiMote",this);
    QPushButton *disconnectWiiMoteButton = new QPushButton("Disconnct WiiMote",this);

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


void MainWindow::servo0SliderChanged(const int &pos)
{
    emit sendCommand("Servo0",QString::number(pos));
    m_angle0 = (float)pos/100000;
    servo0Label->setText("S0 = " + QString::number(m_angle0*180 /M_PI - 90));
    m_visualisation->updateservo0(m_angle0 *180 /M_PI - 90);
    emit calculatePosition(m_angle0,m_angle1,m_angle2,m_angle3,m_angle4,m_angle5);
}

void MainWindow::servo0animationClicked()
{
    moveServo(0,m_angle0*100000,M_PI_2*100000,periodMove);

}

void MainWindow::servo0PowerOffClicked()
{
    servo0Label->setText("S0 = OFF");
    animationServo0->stop();
    emit sendCommand("Servo0",QString::number(0));
}

void MainWindow::servo1SliderChanged(const int &pos)
{
    servo1Label->setText("S1 = " + QString::number((float)pos/100000 *180 /M_PI - 90));
    emit sendCommand("Servo1",QString::number(pos));
    m_angle1 = (float)pos/100000;
    m_visualisation->updateservo1(m_angle1 *180 /M_PI);
    emit calculatePosition(m_angle0,m_angle1,m_angle2,m_angle3,m_angle4,m_angle5);
}

void MainWindow::servo1animationClicked()
{
    moveServo(1,m_angle1*100000,M_PI_2*100000,periodMove);
}

void MainWindow::servo1PowerOffClicked()
{
    servo1Label->setText("S1 = OFF");
    animationServo1->stop();
    emit sendCommand("Servo1",QString::number(0));

}

void MainWindow::servo2SliderChanged(const int &pos)
{
    servo2Label->setText("S2 = " + QString::number((float)pos/100000 *180 /M_PI - 90));
    emit sendCommand("Servo2",QString::number(pos));
    m_angle2 = (float)pos/100000;
    m_visualisation->updateservo2(m_angle2 *180 /M_PI - 90);
    emit calculatePosition(m_angle0,m_angle1,m_angle2,m_angle3,m_angle4,m_angle5);
}

void MainWindow::servo2animationClicked()
{
    moveServo(2,m_angle2*100000,M_PI_2*100000,periodMove);
}

void MainWindow::servo2PowerOffClicked()
{
    servo2Label->setText("S2 = OFF");
    animationServo2->stop();
    emit sendCommand("Servo2",QString::number(0));
}

void MainWindow::servo3SliderChanged(const int &pos)
{
    servo3Label->setText("S3 = " + QString::number(((float)pos/100000 *180 /M_PI) - 90));
    emit sendCommand("Servo3",QString::number(pos));
    m_angle3 = (float)pos/100000;
    m_visualisation->updateservo3(m_angle3 *180 /M_PI - 90);
    emit calculatePosition(m_angle0,m_angle1,m_angle2,m_angle3,m_angle4,m_angle5);
}
void MainWindow::servo3animationClicked()
{
    moveServo(3,m_angle3*100000,M_PI_2*100000,periodMove);
}

void MainWindow::servo3PowerOffClicked()
{
    servo3Label->setText("S3 = OFF");
    animationServo3->stop();
    emit sendCommand("Servo3",QString::number(0));
}

void MainWindow::servo4SliderChanged(const int &pos)
{
    servo4Label->setText("S4 = " + QString::number((float)pos/100000 *180 /M_PI - 90));
    emit sendCommand("Servo4",QString::number(pos));
    m_angle4 = (float)pos/100000;
    m_visualisation->updateservo4(m_angle4 *180 /M_PI - 90);
    emit calculatePosition(m_angle0,m_angle1,m_angle2,m_angle3,m_angle4,m_angle5);
}

void MainWindow::servo4animationClicked()
{
    moveServo(4,m_angle4*100000,M_PI_2*100000,periodMove);
}

void MainWindow::servo4PowerOffClicked()
{
    servo4Label->setText("S4 = OFF");
    animationServo4->stop();
    emit sendCommand("Servo4",QString::number(0));
}

void MainWindow::servo5SliderChanged(const int &pos)
{
    servo5Label->setText("S5 = " + QString::number((float)pos/100000 *180 /M_PI - 90));
    emit sendCommand("Servo5",QString::number(pos));
    m_angle5 = (float)pos/100000;
    m_visualisation->updateservo5(m_angle5 *180 /M_PI - 90);
    emit calculatePosition(m_angle0,m_angle1,m_angle2,m_angle3,m_angle4,m_angle5);
}

void MainWindow::servo5animationClicked()
{
    moveServo(5,m_angle5*100000,M_PI_2*100000,periodMove);
}

void MainWindow::servo5PowerOffClicked()
{
    servo5Label->setText("S5 = OFF");
    animationServo5->stop();
    emit sendCommand("Servo5",QString::number(0));
}

void MainWindow::updateAllAngles()
{
    m_visualisation->updateservo0(m_angle0 *180 /M_PI - 90);
    m_visualisation->updateservo1(m_angle1 *180 /M_PI - 90);
    m_visualisation->updateservo2(m_angle2 *180 /M_PI - 90);
    m_visualisation->updateservo3(m_angle3 *180 /M_PI - 90);
    m_visualisation->updateservo4(m_angle4 *180 /M_PI - 90);
    m_visualisation->updateservo5(m_angle5 *180 /M_PI - 90);
}

void MainWindow::tcpXPlusClicked()
{
    emit moveToPosition(1,0,0,0,0,0);
}

void MainWindow::tcpXMinusClicked()
{
    emit moveToPosition(-1,0,0,0,0,0);
}

void MainWindow::tcpYPlusClicked()
{
    emit moveToPosition(0,1,0,0,0,0);
}

void MainWindow::tcpYMinusClicked()
{
    emit moveToPosition(0,-1,0,0,0,0);
}

void MainWindow::tcpZPlusClicked()
{
    emit moveToPosition(0,0,1,0,0,0);
}

void MainWindow::tcpZMinusClicked()
{
    emit moveToPosition(0,0,-1,0,0,0);
}

void MainWindow::tcpWXPlusClicked()
{
    emit moveToPosition(0,0,0,M_PI/180,0,0);
}

void MainWindow::tcpWXMinusClicked()
{
    emit moveToPosition(0,0,0,-M_PI/180,0,0);
}

void MainWindow::tcpWYPlusClicked()
{
    emit moveToPosition(0,0,0,0,M_PI/180,0);
}

void MainWindow::tcpWYMinusClicked()
{
    emit moveToPosition(0,0,0,0,-M_PI/180,0);
}

void MainWindow::tcpWZPlusClicked()
{
    emit moveToPosition(0,0,0,0,0,M_PI/180);
}

void MainWindow::tcpWZMinusClicked()
{
    emit moveToPosition(0,0,0,0,0,-M_PI/180);
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
    //Linear
    //OutBounce
    //InOutExpo
    //InOutCubic
    //InOutQuint

    QEasingCurve movementCurve = QEasingCurve::Linear;

    switch(servoNumber){
    case 0:
        animationServo0->setDuration(period);
        animationServo0->setStartValue(start);
        animationServo0->setEndValue(end);
        animationServo0->setEasingCurve(movementCurve);
        animationServo0->start();
        break;
    case 1:
        animationServo1->setDuration(period);
        animationServo1->setStartValue(start);
        animationServo1->setEndValue(end);
        animationServo1->setEasingCurve(movementCurve);
        animationServo1->start();
        break;
    case 2:
        animationServo2->setDuration(period);
        animationServo2->setStartValue(start);
        animationServo2->setEndValue(end);
        animationServo2->setEasingCurve(movementCurve);
        animationServo2->start();
        break;
    case 3:
        animationServo3->setDuration(period);
        animationServo3->setStartValue(start);
        animationServo3->setEndValue(end);
        animationServo3->setEasingCurve(movementCurve);
        animationServo3->start();
        break;
    case 4:
        animationServo4->setDuration(period);
        animationServo4->setStartValue(start);
        animationServo4->setEndValue(end);
        animationServo4->setEasingCurve(movementCurve);
        animationServo4->start();
        break;
    case 5:
        animationServo5->setDuration(period);
        animationServo5->setStartValue(start);
        animationServo5->setEndValue(end);
        animationServo5->setEasingCurve(movementCurve);
        animationServo5->start();
        break;
    default:
        break;
    }
}

void MainWindow::moveJoints(float theta1, float theta2, float theta3, float theta4, float theta5, float theta6)
{
    servo0Slider->setValue((m_angle0+theta1)*100000);
    servo1Slider->setValue((m_angle1+theta2)*100000);
    servo2Slider->setValue((m_angle2+theta3)*100000);
    servo3Slider->setValue((m_angle3+theta4)*100000);
    servo4Slider->setValue((m_angle4+theta5)*100000);
    servo5Slider->setValue((m_angle5+theta6)*100000);

}


void MainWindow::initServoClicked()
{
    emit sendCommand("Servo","init");
}

void MainWindow::servoHomePositionClicked()
{
    moveServo(0,(m_angle0*100000),M_PI_2*100000,periodMove);
    moveServo(1,(m_angle1*100000),M_PI*100000,periodMove);
    moveServo(2,(m_angle2*100000),M_PI_2+M_PI_2/2*100000,periodMove);
    moveServo(3,(m_angle3*100000),M_PI_2*100000,periodMove);
    moveServo(4,(m_angle4*100000),M_PI_2+M_PI_2/2*100000,periodMove);
    moveServo(5,(m_angle5*100000),M_PI_2*100000,periodMove);

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
        servo2Slider->setValue((int)(pitch_i*(-1)+50)*M_PI*100000/180);
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

void MainWindow::speedLeft(double speedL)
{
    speedLeftLabel->setText("Speed L: " + QString::number(speedL));
}

void MainWindow::speedRight(double speedR)
{
    speedRightLabel->setText("Speed R: " + QString::number(speedR));
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

void MainWindow::tcpPositionChanged(const float &x, const float &y, const float &z, const float &wx, const float &wy, const float &wz)
{
    tcp_x_Lable->setText("x  = " + QString::number(x));
    tcp_y_Lable->setText("y  = " + QString::number(y));
    tcp_z_Lable->setText("z  = " + QString::number(z));
    tcp_wx_Lable->setText("wx = " + QString::number(wx*180/M_PI));
    tcp_wy_Lable->setText("wy = " + QString::number(wy*180/M_PI));
    tcp_wz_Lable->setText("wz = " + QString::number(wz*180/M_PI));
    qDebug() << m_angle0 << m_angle1 << m_angle2 << m_angle3 << m_angle4 << m_angle5;


}


