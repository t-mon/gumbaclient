#include "qwiimote.h"
#include <unistd.h>
#include <QDebug>



QWiiMote::QWiiMote(QObject *parent) :
    QObject(parent)
{
    //m_wiimote = new CWiimote();
}

void QWiiMote::searchWiiMotes()
{
//    CWii wii; // Defaults to 4 remotes
//    //std::vector<CWiimote>::iterator i;
//    int numFound;
//    int index;

//    qDebug() << "Searching for wiimotes... Turn them on!";
//    //Find the wiimote
//    numFound = wii.Find(5);
//    qDebug() << "Found" << numFound << "devices";
//    emit writeToTerminal("Found" + QString::number(numFound) + "devices");

//    // Connect to the wiimote
//    std::vector<CWiimote>& wiimotes = wii.Connect();
//    qDebug() << "Connected to " << (unsigned int)wiimotes.size() << " wiimotes";


//    // Setup the wiimotes
//    for(index = 0, i = wiimotes.begin(); i != wiimotes.end(); ++i, ++index){
//        // Use a reference to make working with the iterator handy.
//        CWiimote &wiimote = *i;

//        //Set Leds
//        wiimote.SetLEDs(LED_MAP[index]);

//        //Rumble for 0.2 seconds as a connection ack
//        wiimote.SetRumbleMode(CWiimote::ON);
//        usleep(200000);
//        wiimote.SetRumbleMode(CWiimote::OFF);
//    }

}
