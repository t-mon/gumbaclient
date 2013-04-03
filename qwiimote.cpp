#include "qwiimote.h"
#include <unistd.h>
#include <QDebug>


QWiiMote::QWiiMote(QObject *parent) :
    QObject(parent)
{
    m_exit = false;
}

void QWiiMote::HandleEvent()
{
    // Handels A pressed & A+B Pressed
    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_A)){
        if(m_wiiMote->Buttons.isPressed(CButtons::BUTTON_B)){
            writeToTerminal("---> wii button A+B pressed ");
            emit button_AB_pressed(true);
        }else{
            qDebug() << "---> wii button A pressed ";
            emit button_A_pressed(true);
        }
    }

    // Handels A released & A+B released
    if(m_wiiMote->Buttons.isReleased(CButtons::BUTTON_A)){
        if(m_wiiMote->Buttons.isPressed(CButtons::BUTTON_B)){
            writeToTerminal("---> wii button A+B released ");
            emit button_AB_pressed(false);
        }else{
            qDebug() << "---> wii button A released ";
            emit button_A_pressed(false);
        }
    }

    // Handels B pressed & B+A Pressed
    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_B)){
        if(m_wiiMote->Buttons.isPressed(CButtons::BUTTON_A)){
            writeToTerminal("---> wii button A+B pressed ");
            emit button_AB_pressed(true);
        }else{
            qDebug() << "---> wii button B pressed ";
            emit button_B_pressed(true);
        }
    }

    // Handels B released & B+A released
    if(m_wiiMote->Buttons.isReleased(CButtons::BUTTON_B)){
        if(m_wiiMote->Buttons.isPressed(CButtons::BUTTON_A)){
            writeToTerminal("---> wii button A+B released ");
            emit button_AB_pressed(false);
        }else{
            qDebug() << "---> wii button B released ";
            emit button_B_pressed(false);
        }
    }

    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_PLUS)){
        qDebug() << "---> wii button PLUS pressed ";
        emit button_PLUS_pressed();
    }
    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_MINUS)){
        qDebug() << "---> wii button MINUS pressed ";
        emit button_MINUS_pressed();
    }
    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_DOWN)){
        qDebug() << "---> wii button DOWN pressed ";
        emit button_DOWN_pressed();
    }
    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_UP)){
        qDebug() << "---> wii button UP pressed ";
        emit button_UP_pressed();
    }
    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_LEFT)){
        qDebug() << "---> wii button LEFT pressed ";
        emit button_LEFT_pressed();
    }
    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_RIGHT)){
        qDebug() << "---> wii button RIGHT pressed ";
        emit button_RIGHT_pressed();
    }
    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_ONE)){
        qDebug() << "---> wii button ONE pressed ";
        emit button_ONE_pressed();
    }
    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_TWO)){
        qDebug() << "---> wii button TWO pressed ";
        emit button_TWO_pressed();
    }
    if(m_wiiMote->Buttons.isJustPressed(CButtons::BUTTON_HOME)){
        qDebug() << "---> wii button HOME pressed ";
        emit button_HOME_pressed();
    }


    // if the accelerometer is turned on then print angles
    if(m_wiiMote->isUsingACC())
    {
        float pitch, roll, yaw;
        m_wiiMote->Accelerometer.GetOrientation(pitch, roll, yaw);
        emit orientationWiiMoteChanged(roll,pitch,yaw);

    }

    // if the Motion Plus is turned on
    if(m_wiiMote->isUsingMotionPlus()) {
        float roll, pitch, yaw;
        m_wiiMote->ExpansionDevice.MotionPlus.Gyroscope.GetRates(roll,pitch,yaw);
        emit orientationMotionPlusChanged(roll, pitch, yaw);
    }


    int exType = m_wiiMote->ExpansionDevice.GetType();
    if(exType == m_wiiMote->ExpansionDevice.TYPE_NUNCHUK)
    {
        float pitch, roll, yaw;
        float angle, magnitude;

        CNunchuk &nc = m_wiiMote->ExpansionDevice.Nunchuk;

        if(nc.Buttons.isPressed(CNunchukButtons::BUTTON_C)){
            qDebug() << "---> wii Nunchuk button C pressed ";
            emit button_NC_C_pressed();
        }
        if(nc.Buttons.isPressed(CNunchukButtons::BUTTON_Z)){
            qDebug() << "---> wii Nunchuk button Z pressed ";
            emit button_NC_Z_pressed();
        }

        nc.Accelerometer.GetOrientation(pitch, roll, yaw);
        nc.Joystick.GetPosition(angle, magnitude);

        emit orientationNunChuckChanged(roll, pitch, yaw);
        emit nunchukJoystickChanged(angle,magnitude);
    }
    else if(exType == m_wiiMote->ExpansionDevice.TYPE_CLASSIC)
    {
        CClassic &cc = m_wiiMote->ExpansionDevice.Classic;

        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_A)){
            qDebug() << "---> wii Classic button A pressed ";
            emit button_Classic_A_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_B)){
            qDebug() << "---> wii Classic button B pressed ";
            emit button_Classic_B_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_X)){
            qDebug() << "---> wii Classic button X pressed ";
            emit button_Classic_X_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_Y)){
            qDebug() << "---> wii Classic button Y pressed ";
            emit button_Classic_Y_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_LEFT)){
            qDebug() << "---> wii Classic button LEFT pressed ";
            emit button_Classic_LEFT_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_RIGHT)){
            qDebug() << "---> wii Classic button RIGHT pressed ";
            emit button_Classic_RIGHT_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_UP)){
            qDebug() << "---> wii Classic button UP pressed ";
            emit button_Classic_UP_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_DOWN)){
            qDebug() << "---> wii Classic button DOWN pressed ";
            emit button_Classic_DOWN_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_PLUS)){
            qDebug() << "---> wii Classic button PLUS pressed ";
            emit button_Classic_PLUS_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_MINUS)){
            qDebug() << "---> wii Classic button MINUS pressed ";
            emit button_Classic_MINUS_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_HOME)){
            qDebug() << "---> wii Classic button HOME pressed ";
            emit button_Classic_HOME_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_ZL)){
            qDebug() << "---> wii Classic button ZL pressed ";
            emit button_Classic_ZL_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_ZR)){
            qDebug() << "---> wii Classic button ZR pressed ";
            emit button_Classic_ZR_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_FULL_L)){
            qDebug() << "---> wii Classic button FULL_L pressed ";
            emit button_Classic_FULL_L_pressed();
        }
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_FULL_R)){
            qDebug() << "---> wii Classic button FULL_R pressed ";
            emit button_Classic_FULL_R_pressed();
        }

        float angle_l, magnitude_l, angle_r, magnitude_r;
        cc.LeftJoystick.GetPosition(angle_l, magnitude_l);
        cc.RightJoystick.GetPosition(angle_r, magnitude_r);
        emit classicJoystickRightChanged(angle_r,magnitude_r);
        emit classicJoystickLeftChanged(angle_l,magnitude_l);

    }
    else if(exType == m_wiiMote->ExpansionDevice.TYPE_GUITAR_HERO_3)
    {

        CGuitarHero3 &gh = m_wiiMote->ExpansionDevice.GuitarHero3;

        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_STRUM_UP)){
            qDebug() << "---> wii Guitar Hero STRUM UP pressed";
            emit button_GH_STRUM_UP_pressed();
        }
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_STRUM_DOWN)){
            qDebug() << "---> wii Guitar Hero STRUM DOWN pressed";
            emit button_GH_STRUM_DOWN_pressed();
        }
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_YELLOW)){
            qDebug() << "---> wii Guitar Hero YELLOW pressed";
            emit button_GH_YELLOW_pressed();
        }
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_GREEN)){
            qDebug() << "---> wii Guitar Hero GREEN pressed";
            emit button_GH_GREEN_pressed();
        }
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_BLUE)){
            qDebug() << "---> wii Guitar Hero BLUE pressed";
            emit button_GH_BLUE_pressed();
        }
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_RED)){
            qDebug() << "---> wii Guitar Hero RED pressed";
            emit button_GH_RED_pressed();
        }
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_ORANGE)){
            qDebug() << "---> wii Guitar Hero ORANGE pressed";
            emit button_GH_ORANGE_pressed();
        }
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_PLUS)){
            qDebug() << "---> wii Guitar Hero PLUS pressed";
            emit button_GH_PLUS_pressed();
        }
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_MINUS)){
            qDebug() << "---> wii Guitar Hero MINUS pressed";
            emit button_GH_MINUS_pressed();
        }

        emit guitarHeroWhammyBarChanged(gh.GetWhammyBar());

        float angle, magnitude;
        gh.Joystick.GetPosition(angle, magnitude);

        emit guitarHeroJoystickChanged(angle, magnitude);

    }
}

void QWiiMote::HandleStatus()
{
    emit wiiMoteStatusChanged(m_wiiMote->GetID(),m_wiiMote->isUsingACC(),m_wiiMote->isUsingIR(),m_wiiMote->GetBatteryLevel());
    emit wiiLedChanged(m_wiiMote->isLEDSet(1), m_wiiMote->isLEDSet(2), m_wiiMote->isLEDSet(3),m_wiiMote->isLEDSet(4));
    emit wiiBatteryChanged(m_wiiMote->GetBatteryLevel());
}


void QWiiMote::startWiiMotesLoop()
{
    m_exit = false;
    qDebug() << "Start wii loop";
    CWii wii;
    std::vector<CWiimote>::iterator i;
    int reloadWiimotes = 0;

    emit writeToTerminal("searching for WiiMotes...");

    // Find and connect to the wiimotes
    std::vector<CWiimote>& wiimotes = wii.FindAndConnect();

    if (!wiimotes.size()) {
        emit writeToTerminal("no wii remote found!");
        emit stopProcess();
        m_exit = true;
        return;
    }

    if(wiimotes.size() != 1){
        emit writeToTerminal("please add just one Wii Remote");
        emit stopProcess();
        m_exit = true;
        return;
    }
    emit writeToTerminal("...WiiMote connected!");
    m_wiiMote = new CWiimote(wiimotes.front());
    m_wiiMote->SetLEDs(CWiimote::LED_1);
    emit writeToTerminal("...WiiMote sensingmode ON");
    m_wiiMote->SetMotionSensingMode(CWiimote::ON);
    m_wiiMote->SetSmoothing(true);

    emit writeToTerminal("====================================");
    emit writeToTerminal("Controller Status [wiimote " +QString::number(m_wiiMote->GetID()) + "]");
    emit writeToTerminal("   attachment: " + QString::number(m_wiiMote->ExpansionDevice.GetType()));
    emit writeToTerminal("   speaker:    " + QString::number(m_wiiMote->isUsingSpeaker()));
    emit writeToTerminal("   ir:         " + QString::number(m_wiiMote->isUsingIR()));
    emit writeToTerminal("   leds:       " + QString::number(m_wiiMote->isLEDSet(1)) + " " + QString::number(m_wiiMote->isLEDSet(2)) + " " +  QString::number(m_wiiMote->isLEDSet(3)) + " " + QString::number(m_wiiMote->isLEDSet(4)));
    emit writeToTerminal("   battery:    " + QString::number(m_wiiMote->GetBatteryLevel()) + "\n");


    do{
        if(reloadWiimotes){
            // Regenerate the list of wiimotes
            wiimotes = wii.GetWiimotes();
            qDebug() << "Reload wiimotes";
            reloadWiimotes = 0;
        }

        //Poll the wiimotes to get the status like pitch or roll
        if(wii.Poll())
        {
            switch(m_wiiMote->GetEvent()){

            case CWiimote::EVENT_EVENT:
                HandleEvent();
                break;

            case CWiimote::EVENT_STATUS:
                emit HandleStatus();
                break;

            case CWiimote::EVENT_DISCONNECT:
                emit writeToTerminal("Wii: Wii mote disconnected!");
                m_exit=true;
                emit stopProcess();
                break;

            case CWiimote::EVENT_UNEXPECTED_DISCONNECT:
                emit writeToTerminal("Wii: Wii mote disconnected!");
                m_exit=true;
                emit stopProcess();
                break;

            case CWiimote::EVENT_NUNCHUK_INSERTED:
                emit writeToTerminal("Wii: Nunchuk inserted!");
                reloadWiimotes = 1;
                break;

            case CWiimote::EVENT_NUNCHUK_REMOVED:
                emit writeToTerminal("Wii: Nunchuk removed!");
                break;

            case CWiimote::EVENT_MOTION_PLUS_INSERTED:
                emit writeToTerminal("Wii: WiiMotionPlus inserted!");
                break;

            case CWiimote::EVENT_MOTION_PLUS_REMOVED:
                emit writeToTerminal("Wii: WiiMotionPlus removed!");
                reloadWiimotes = 1;
                break;

            case CWiimote::EVENT_CLASSIC_CTRL_INSERTED:
                emit writeToTerminal("Wii: Classic Controller inserted!");
                reloadWiimotes = 1;
                break;

            case CWiimote::EVENT_CLASSIC_CTRL_REMOVED:
                emit writeToTerminal("Wii: Classic Controller removed!");
                break;

            case CWiimote::EVENT_GUITAR_HERO_3_CTRL_INSERTED:
                emit writeToTerminal("Wii: Guitar Hero guitar inserted!");
                reloadWiimotes = 1;
                break;

            case CWiimote::EVENT_GUITAR_HERO_3_CTRL_REMOVED:
                emit writeToTerminal("Wii: Guitar Hero guitar removed!");
                break;

            default:
                break;
            }
        }
    }while(wiimotes.size() || !m_exit); // Go so long as there are wiimotes left to poll
    qDebug() << "wii loop finished";
    emit stopProcess();


}


void QWiiMote::disconnectWiiMote()
{
    m_exit = true;
    m_wiiMote->Disconnect();
    emit stopProcess();
    writeToTerminal("WiiMote disconnect");
}


