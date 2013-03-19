#ifndef QWIIMOTE_H
#define QWIIMOTE_H

//#include <wiic/wiicpp.h>
#include <QObject>

class QWiiMote : public QObject
{
    Q_OBJECT
public:
    explicit QWiiMote(QObject *parent = 0);

private:
    //CWiimote *m_wiimote;
    //int LED_MAP[4] = {CWiimote::LED_1, CWiimote::LED_2, CWiimote::LED_3, CWiimote::LED_4};

signals:
    void writeToTerminal(const QString &terminalString);

public slots:
    void searchWiiMotes();

};

#endif // QWIIMOTE_H
