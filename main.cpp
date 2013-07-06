#include <QtGui/QApplication>
#include <GL/glut.h>
#include "core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


    a.setStyleSheet("QTextEdit { background-color: QColor(0, 0, 0, 220)}");
    Core core;
    
    return a.exec();
}
