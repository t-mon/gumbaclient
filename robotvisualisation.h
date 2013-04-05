#ifndef ROBOTVISUALISATION_H
#define ROBOTVISUALISATION_H

#include <QGLWidget>
#include <qgl.h>

static float servo0 = 0;
static float servo1 = 90;
static float servo2 = -90;
static float servo3 = -90;
static float servo4 = 0;
static float servo5 = 0;

//  HOME POSITION on -> 0,0,0,0,0
//    _______
//    |     |
//    |
//    |
//--------------
//
//       1         0         0       100
// T05 = 0        -1         0         0
//       0         0        -1       100
//       0         0         0         1


class RobotVisualisation : public QGLWidget
{
public:
    RobotVisualisation(QObject *parent = 0);

private:

protected:
    void initializeGL(void);
    void resizeGL( GLint w, GLint h );
    void paintGL();
    void wireCube(GLdouble length);
    void armRectangle();
    void wristRectangle(float length);

public slots:
    void updateservo0(int degree);
    void updateservo1(int degree);
    void updateservo2(int degree);
    void updateservo3(int degree);
    void updateservo4(int degree);
    void updateservo5(int degree);

};

#endif // ROBOTVISUALISATION_H
