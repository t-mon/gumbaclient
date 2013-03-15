
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>
#include <QColor>
#include <QDebug>
#include <QKeyEvent>


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
    connect(connectGumbaButton,SIGNAL(clicked()),this,SLOT(connectGumbaClicked()));

    QPushButton *disconnectGumbaButton = new QPushButton("Disconnect Gumba");
    disconnectGumbaButton->setFixedWidth(150);
    gridLayout->addWidget(disconnectGumbaButton,1,0);
    connect(disconnectGumbaButton,SIGNAL(clicked()),this,SLOT(disconnectGumbaClicked()));

    QPushButton *startGumbaApplicationButton = new QPushButton("Start Gumba");
    startGumbaApplicationButton->setFixedWidth(150);
    gridLayout->addWidget(startGumbaApplicationButton,0,3);
    connect(startGumbaApplicationButton,SIGNAL(clicked()),this,SLOT(startGumbaApplicationClicked()));

    QPushButton *toggleRTSButton = new QPushButton("Toggle RTS");
    toggleRTSButton->setFixedWidth(150);
    gridLayout->addWidget(toggleRTSButton,1,3);
    connect(toggleRTSButton,SIGNAL(clicked()),this,SLOT(toggleRTSClicked()));

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
    //motorLeftLabel->setAlignment(Qt::AlignCenter);
    motorLeftLabel->setText("Motor L: 0");
    //motorLeftLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    motorRightLabel = new QLabel(this);
    motorRightLabel->setFixedWidth(90);
    gridLayout->addWidget(motorRightLabel,0,7);
    //motorRightLabel->setAlignment(Qt::AlignCenter);
    motorRightLabel->setText("Motor R: 0");
    //motorRightLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    lightLeftLabel = new QLabel(this);
    lightLeftLabel->setFixedWidth(90);
    gridLayout->addWidget(lightLeftLabel,1,6);
    //lightLeftLabel->setAlignment(Qt::AlignCenter);
    lightLeftLabel->setText("Light L: 0");
    //lightLeftLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    lightRightLabel = new QLabel(this);
    lightRightLabel->setFixedWidth(90);
    gridLayout->addWidget(lightRightLabel,1,7);
    //lightRightLabel->setAlignment(Qt::AlignCenter);
    lightRightLabel->setText("Light R: 0");
    //lightRightLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    batteryLabel = new QLabel(this);
    batteryLabel->setFixedWidth(90);
    gridLayout->addWidget(batteryLabel,2,0);
    //batteryLabel->setAlignment(Qt::AlignCenter);
    batteryLabel->setText("0 V");
    //batteryLabel->setStyleSheet("background-color:gray; border-radius: 5px;");

    velocitySlider = new QSlider(Qt::Horizontal, this);
    velocitySlider->setFixedWidth(100);
    gridLayout->addWidget(velocitySlider,3,0);
    velocitySlider->setTickInterval(1);
    velocitySlider->setMinimum(1);
    velocitySlider->setMaximum(4);
    connect(velocitySlider,SIGNAL(sliderMoved(int)),this,SLOT(velocitySliderChanged(int)));







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
    connect(connectServerButton,SIGNAL(clicked()),this,SLOT(connectServerClicked()));

    QPushButton *disconnectServerButton = new QPushButton("Disconnect Server");
    disconnectServerButton->setFixedWidth(150);
    gridUnderTerminalLayout->addWidget(disconnectServerButton,0,2);
    connect(disconnectServerButton,SIGNAL(clicked()),this,SIGNAL(disconnectServer()));

    hostLineEdit = new QLineEdit();
    hostLineEdit->setFixedWidth(120);
    gridUnderTerminalLayout->addWidget(hostLineEdit,0,3);
    hostLineEdit->setText("10.10.10.40");

    portLineEdit = new QLineEdit();
    portLineEdit->setFixedWidth(60);
    gridUnderTerminalLayout->addWidget(portLineEdit,0,4);
    portLineEdit->setText("2222");

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
        //ui->speedLable->setText("v = low");
        break;
    case 2:
        emit sendCommand("RoboterSpeed","2");
        writeToTerminal("> speedlevel: 2");
        //ui->speedLable->setText("v = med");
        break;
    case 3:
        emit sendCommand("RoboterSpeed","3");
        writeToTerminal("> speedlevel: 3");
        //ui->speedLable->setText("v = high");
        break;
    case 4:
        emit sendCommand("RoboterSpeed","4");
        writeToTerminal("> speedlevel: MAX");
        //ui->speedLable->setText("v = max");
        break;
    }
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


