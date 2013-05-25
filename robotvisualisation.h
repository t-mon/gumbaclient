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
    QPoint m_lastPos;
    int m_xRot;
    int m_yRot;
    int m_zRot;

protected:
    void initializeGL(void);
    void resizeGL( GLint w, GLint h );
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
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
