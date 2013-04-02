#ifndef ROBOTVISUALISATION_H
#define ROBOTVISUALISATION_H

#include <QGLWidget>
#include <qgl.h>

static int servo0 = 90;
static int servo1 = -90;
static int servo2 = 0;


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

};

#endif // ROBOTVISUALISATION_H
