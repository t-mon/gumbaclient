#include <GL/glu.h>
#include <QDebug>
#include "robotvisualisation.h"



RobotVisualisation::RobotVisualisation()
{
}

void RobotVisualisation::initializeGL()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
}

void RobotVisualisation::resizeGL(int w, int h)
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
    glClear (GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef (-0.9, 0.0, 0.0);
    glRotatef ((GLfloat) servo0, 0.0, 0.0, 1.0);
    glTranslatef (1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef (2.0, 0.4, 1.0);
    wireCube(0.8);

    glPopMatrix();

    glTranslatef (0.9, 0.0, 0.0);
    glRotatef ((GLfloat) servo1, 0.0, 0.0, 1.0);
    glTranslatef (1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef (2.0, 0.4, 1.0);
    wireCube(0.8);

    glPopMatrix();

    glTranslatef (0.9, 0.0, 0.0);
    glRotatef ((GLfloat) servo2, 0.0, 0.0, 1.0);
    glTranslatef (1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef (2.0, 0.4, 1.0);
    wireCube(0.8);

    glPopMatrix();

    glPopMatrix();

}


void RobotVisualisation::wireCube(float length)
{

    glBegin(GL_LINE_LOOP);
    glVertex3f(-length/2, -length/2, length/2);
    glVertex3f(length/2, -length/2, length/2);
    glVertex3f(length/2, length/2, length/2);
    glVertex3f(-length/2, length/2, length/2);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(-length/2, -length/2, -length/2);
    glVertex3f(length/2, -length/2, -length/2);
    glVertex3f(length/2, length/2, -length/2);
    glVertex3f(-length/2, length/2, -length/2);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(-length/2, -length/2, length/2);
    glVertex3f(-length/2, -length/2, -length/2);

    glVertex3f(length/2, -length/2, length/2);
    glVertex3f(length/2, -length/2, -length/2);

    glVertex3f(length/2, length/2, length/2);
    glVertex3f(length/2, length/2, -length/2);

    glVertex3f(-length/2, length/2, length/2);
    glVertex3f(-length/2, length/2, -length/2);
    glEnd();
}

void RobotVisualisation::updateservo0(int degree)
{
    servo0 = (degree +90) % 360;
    updateGL();
}

void RobotVisualisation::updateservo1(int degree)
{
    servo1 = degree % 360;
    updateGL();
}

void RobotVisualisation::updateservo2(int degree)
{
    servo2 = degree % 360;
    updateGL();
}
