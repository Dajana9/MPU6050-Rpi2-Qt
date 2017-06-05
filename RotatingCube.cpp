#include "RotatingCube.h"
#include <QtWidgets>
#include <QtOpenGL>

RotatingCube::RotatingCube(QWidget *parent)
     : QGLWidget(parent), mFullScreen(false),
       mXRotate(0.0f), mYRotate(0.0f),
       mZ(-5.0f)
{
    showNormal();
    startTimer(50);
}

RotatingCube::~RotatingCube()
{
    glDeleteTextures(3, &mTexture[0]);
}
void RotatingCube::loadGLTexture()
{
    QImage image(":/Image/Crate.bmp");
    image = image.convertToFormat(QImage::Format_RGB888);
    image = image.mirrored();


    //textures[j] = new QOpenGLTexture(QImage(QString(":/images/side%1.png").arg(j + 1)).mirrored());
    glGenTextures(3, &mTexture[0]);

    glBindTexture(GL_TEXTURE_2D, mTexture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(),
                 image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
}

void RotatingCube::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void RotatingCube::initializeGL()
{
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void RotatingCube::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,mZ);

   // glBindTexture(GL_TEXTURE_2D, mTexture[mFilter]);

    glRotatef(mXRotate,1.0f,0.0f,0.0f);
    glRotatef(-mYRotate,0.0f,0.0f,1.0f);
    glRotatef(mZRotate,1.0f,1.0f,1.0f);

    //glRotatef(mZRotate,0.0f,0.0f,1.0f);

    glBegin(GL_QUADS);

        glNormal3f( 0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

        glNormal3f( 0.0f, 0.0f,-1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

        glNormal3f( 0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

        glNormal3f( 0.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

        glNormal3f( 1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();

}

void RotatingCube::setRotation(double xRot, double yRot)
{
    mXRotate = xRot;
    mYRotate = yRot;
    loadGLTexture();
    updateGL();
}

void RotatingCube::setRotationGyro(double x,double y,double z)
{

    mXRotate += x;
    mYRotate += y;
    mZRotate += z;
    loadGLTexture();
    updateGL();

}
