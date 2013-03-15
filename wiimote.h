#ifndef WIIMOTE_H
#define WIIMOTE_H
//#include <wiic/wiicpp.h>
#include <QObject>


class WiiMote : public QObject
{
    Q_OBJECT
public:
    explicit WiiMote(QObject *parent = 0);
    
private:
    //CWiimote *m_wiimote;

signals:
    
public slots:
    
};

#endif // WIIMOTE_H
