#include <QtGui>
#include <QtOpenGL>
#include <QMatrix4x4>
#include <QVector3D>
#include <qmath.h>
#include <GL/glut.h>
#include "robotvisualisation.h"


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

static float servo0 = 0;
static float servo1 = 90;
static float servo2 = -90;
static float servo3 = -90;
static float servo4 = 0;
static float servo5 = 0;

RobotVisualisation::RobotVisualisation(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers),parent)
{
    // Scale
    m_l=1;
    m_L1 = m_l/10+m_l/30;
    m_L2 = m_l/5+m_l/15;
    m_L3 = m_l/5+m_l/15;
    m_L4 = m_l/15+m_l/30+m_l/30;

    qDebug() << "L1=" << m_L1 << "L2=" << m_L2 << "L3=" << m_L3 << "L4=" << m_L4;

    setXRotation(4991);
    setYRotation(50);

}

void RobotVisualisation::initializeGL()
{
}

void RobotVisualisation::resizeGL(GLint width, GLint height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0,(GLdouble)width/(GLdouble)height,0.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,width,height);

}

void RobotVisualisation::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0,0.0,-2);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    //Base
    drawGrid();
    drawKoordinateSystem();

    //First joint (servo0)
    glPushMatrix();
    glRotatef((GLfloat) servo0,0,0,1);
    drawKoordinateSystem();
    //L1
    drawCylinder(m_l/30,m_l/10);
    glPushMatrix();
    glTranslatef (0,0,m_l/10);

    //Second Joint (servo1)
    glTranslatef (0,0,m_l/30);
    glColor3f(0.8,0.8,0.8);
    glutWireSphere(m_l/30,32,32);

    glPushMatrix();
    glRotatef(90,1,0,0);
    glRotatef((GLfloat) servo1,0,0,1);
    drawKoordinateSystem();

    //L2
    glPushMatrix();
    glRotatef(90,0,1,0);
    glTranslatef (0,0,m_l/30);
    drawCylinder(m_l/30,m_l/5);

    //Third joint (servo2)
    glTranslatef (0,0,m_l/5+m_l/30);
    glRotatef(-90,0,1,0);
    glPushMatrix();
    glColor3f(0.8,0.8,0.8);
    glutWireSphere(m_l/30,32,32);
    glRotatef((GLfloat) servo2,0,0,1);
    drawKoordinateSystem();

    //L3
    glPushMatrix();
    glRotatef(90,0,1,0);
    glTranslatef (0,0,m_l/30);
    drawCylinder(m_l/30,m_l/5);

    //Fourth joint (servo3)
    glTranslatef (0,0,m_l/5+m_l/30);
    glRotatef(-90,0,1,0);
    glPushMatrix();
    glColor3f(0.8,0.8,0.8);
    glutWireSphere(m_l/30,32,32);
    glRotatef((GLfloat) servo3+90,0,0,1);
    drawKoordinateSystem();

    //L4
    glRotatef(90,0,1,0);
//    glTranslatef (0,0,m_l/30);
//    glPushMatrix();
//    drawCylinder(m_l/30,m_l/15);

    //Fith joint (servo4)
    glTranslatef (0,0,m_l/30+m_l/30);
    glRotatef(90,0,0,1);
    glPushMatrix();
    glRotatef((GLfloat)servo4,0,0,1);
    glColor3f(0.5,0.5,0.5);
    glutWireCube(m_l/15);
    drawKoordinateSystem();

    //TCP
    glTranslatef (0,0,m_l/15);
    glPushMatrix();
    drawKoordinateSystem();

    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

}

void RobotVisualisation::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void RobotVisualisation::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 10 * dy);
        setYRotation(yRot + 10 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 10 * dy);
        setYRotation(yRot + 10 * dx);
    }
    lastPos = event->pos();
}

void RobotVisualisation::wheelEvent(QWheelEvent *event)
{
    if(event->orientation()==Qt::Horizontal){
        if(event->delta() > 0){
            qDebug() << "scroll horizontal right";
            zRot+=20;
            setZRotation(zRot);
            updateGL();
            return;
        }
        if(event->delta() < 0){
            qDebug() << "scroll horizontal left";
            zRot-=20;
            setZRotation(zRot);
            updateGL();
            return;
        }
    }
    if(event->orientation()==Qt::Vertical){
        //qDebug() << "scroll vertical" << event->delta();
        if(event->delta() > 0){
            m_l+=0.02;
            updateGL();
        }
        if(event->delta() < 0){
            m_l-=0.02;
            updateGL();
        }
    }
}


void RobotVisualisation::drawGrid()
{
    float n=10;
    float dxy=m_l/n;

    glColor3f(0.1,0.2,0.1);
    glBegin(GL_QUADS);
    glVertex3f( m_l/2,m_l/2, 0);
    glVertex3f( m_l/2,-m_l/2,0);
    glVertex3f( -m_l/2,-m_l/2,0);
    glVertex3f( -m_l/2,m_l/2,0);
    glEnd();

    glBegin(GL_LINES);
    for(int i=0;i<=n;i++){
        glColor3f(0,1,0);
        glVertex3f(-m_l/2,m_l/2-(i*dxy),0);
        glVertex3f(m_l/2,m_l/2-(i*dxy),0);
        glVertex3f(-m_l/2+(i*dxy),m_l/2,0);
        glVertex3f(-m_l/2+(i*dxy),-m_l/2,0);
    }
    glEnd();

    glBegin(GL_LINES);
    // x
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(m_l*0.7,0,0);
    // y
    glColor3f(1,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,m_l*0.7,0);
    // z
    //    glColor3f(0,0,0);
    //    glVertex3f(0,0,0);
    //    glVertex3f(0,0,m_l*0.7);
    glEnd();
}

void RobotVisualisation::drawKoordinateSystem()
{
    glBegin(GL_LINES);
    // x
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(m_l/12,0,0);
    // y
    glColor3f(1,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,m_l/12,0);
    // z
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,m_l/12);
    glEnd();
}

void RobotVisualisation::drawCylinder(float radius, float height)
{
    glColor3f(0.5,0.5,0.5);

    int slices = 32;

    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluQuadricDrawStyle(quadratic, GLU_POINT);
    gluCylinder(quadratic,radius,radius,height,slices,slices);
    gluQuadricDrawStyle(quadratic, GL_FILL);
    gluDisk(quadratic,0,radius,slices,slices);
    glPushMatrix();
    glTranslatef(0,0,height);
    gluQuadricDrawStyle(quadratic, GL_FILL);
    gluDisk(quadratic,0,radius,slices,slices);
    glPopMatrix();
}



void RobotVisualisation::updateservo0(int degree)
{
    servo0 = degree % 360;
    updateGL();
}

void RobotVisualisation::updateservo1(int degree)
{
    servo1 = (degree +90) % 360;
    updateGL();
}

void RobotVisualisation::updateservo2(int degree)
{
    servo2 = (degree -90) % 360;
    updateGL();
}

void RobotVisualisation::updateservo3(int degree)
{
    servo3 = (degree -90) % 360;
    updateGL();
}

void RobotVisualisation::updateservo4(int degree)
{
    servo4 = degree % 360;
    updateGL();
}

void RobotVisualisation::updateservo5(int degree)
{
    servo5 = degree % 360;
    updateGL();
}

void RobotVisualisation::zoomIn()
{
    m_l+=0.01;
    updateGL();
}

void RobotVisualisation::zoomOut()
{
    m_l-=0.01;
    updateGL();
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void RobotVisualisation::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        //qDebug() << "x=" << angle;
        updateGL();
    }
}

void RobotVisualisation::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit xRotationChanged(angle);
        //qDebug() << "y=" << angle;
        updateGL();
    }
}

void RobotVisualisation::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}
