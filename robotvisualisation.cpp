#include <GL/glu.h>
#include <GL/glut.h>
#include <QDebug>
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

RobotVisualisation::RobotVisualisation()
{
}

void RobotVisualisation::initializeGL()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
}

void RobotVisualisation::resizeGL(GLint w, GLint h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -5.0);

}

void RobotVisualisation::paintGL()
{
    GLfloat length = 0.4;
    glClear (GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef (-length, 0.0, 0.0);
    glRotatef ((GLfloat) servo1, 0.0, 0.0, 1.0);
    glTranslatef (length, 0.0, 0.0);
    glPushMatrix();
    glScalef (2.0, 0.4, 1.0);
    wireCube(length);

    glPopMatrix();

    glTranslatef (length, 0.0, 0.0);
    glRotatef ((GLfloat) servo2, 0.0, 0.0, 1.0);
    glTranslatef (length, 0.0, 0.0);
    glPushMatrix();
    glScalef (2.0, 0.4, 1.0);
    wireCube(length);

    glPopMatrix();

    glTranslatef (length, 0.0, 0.0);
    glRotatef ((GLfloat) servo3+90, 0.0, 0.0, 1.0);
    glTranslatef (length/2, 0.0, 0.0);
    glPushMatrix();
    glScalef (1.0, 0.4, 1.0);
    wireCube(length);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}


void RobotVisualisation::wireCube(GLdouble length)
{
    glBegin(GL_LINE_LOOP);
    glVertex3f(-length/2, -length/2, length/2);
    glVertex3f(length/2, -length/2, length/2);
    glVertex3f(length/2, length/2, length/2);
    glVertex3f(-length/2, length/2, length/2);
    glEnd();

}

void RobotVisualisation::armRectangle()
{



}

void RobotVisualisation::wristRectangle(float length)
{

    glBegin(GL_LINE_LOOP);
    glVertex3f(-length/2, -length/2, length/2);
    glVertex3f(length/2, -length/2, length/2);
    glVertex3f(length/2, length/2, length/2);
    glVertex3f(-length/2, length/2, length/2);
    glEnd();

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
