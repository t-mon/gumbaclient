#ifndef ROBOTVISUALISATION_H
#define ROBOTVISUALISATION_H

#include <QGLWidget>
#include <qgl.h>

static int servo0 = 90;
static int servo1 = -90;
static int servo2 = 0;
static int servo3 = 0;
static int servo4 = 0;
static int servo5 = 0;


class RobotVisualisation : public QGLWidget
{
public:
    RobotVisualisation();

private:

protected:
    void initializeGL(void);
    void resizeGL( int w, int h );
    void paintGL();
    void wireCube(float length);

public slots:
    void updateservo0(int degree);
    void updateservo1(int degree);
    void updateservo2(int degree);
    void updateservo3(int degree);
    void updateservo4(int degree);
    void updateservo5(int degree);

};

#endif // ROBOTVISUALISATION_H
