#ifndef ROBOTVISUALISATION_H
#define ROBOTVISUALISATION_H

#include <QObject>
#include <QGLWidget>
#include <qgl.h>

class RobotVisualisation : public QGLWidget
{
    Q_OBJECT

public:
    RobotVisualisation( QWidget *parent = 0 );

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void updateservo0(float degree);
    void updateservo1(float degree);
    void updateservo2(float degree);
    void updateservo3(float degree);
    void updateservo4(float degree);
    void updateservo5(float degree);

    void zoomIn();
    void zoomOut();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);


    void drawGrid();
    void drawKoordinateSystem();
    void drawCylinder(float radius, float height);

private:
    // Gridsize
    float m_l;
    float m_L1;
    float m_L2;
    float m_L3;
    float m_L4;

    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
};

#endif // ROBOTVISUALISATION_H
