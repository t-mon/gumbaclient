#ifndef ROBOTVISUALISATION_H
#define ROBOTVISUALISATION_H

#include <QObject>
#include <QGLWidget>
#include <qgl.h>

class RobotVisualisation : public QGLWidget
{

public:
    RobotVisualisation();

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
