#include <QtGui/QApplication>
#include "core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QTextEdit { background-color: QColor(0, 0, 0, 220)}");
    Core core;
    
    return a.exec();
}
