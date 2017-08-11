#include "RotatingCube.h"
#include <QtWidgets>
#include <QtOpenGL>
#include <MainWindow.h>
#include "ExerciseWindow.h"

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
    QImage imageFront(":/Image/mpu-6050.jpg");
    QImage imageBack(":/Image/mpuBack.jpg");
    QImage imageSides(":/Image/blue.jpg");

    imageFront = imageFront.convertToFormat(QImage::Format_RGB888);
    imageFront = imageFront.mirrored();

    imageBack = imageBack.convertToFormat(QImage::Format_RGB888);
    imageBack = imageBack.mirrored();

    imageSides = imageSides.convertToFormat(QImage::Format_RGB888);
    imageSides = imageSides.mirrored();

    //textures[j] = new QOpenGLTexture(QImage(QString(":/images/side%1.png").arg(j + 1)).mirrored());
    glGenTextures(3, &mTexture[0]);

    glBindTexture(GL_TEXTURE_2D, mTexture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imageFront.width(),
                 imageFront.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, imageFront.bits());


    glBindTexture(GL_TEXTURE_2D, mTexture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imageBack.width(),
                 imageBack.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, imageBack.bits());

    glBindTexture(GL_TEXTURE_2D, mTexture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imageSides.width(),
                 imageSides.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, imageSides.bits());

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
    //pyperspectiveGL(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void RotatingCube::perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
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
    glRotatef(mYRotate,0.0f,0.0f,1.0f);
    glRotatef(mZRotate,0.0f,1.0f,0.0f);

    glBindTexture(GL_TEXTURE_2D, mTexture[0]);//Top
    glBegin(GL_QUADS);
        glNormal3f( 0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  0.2f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  0.2f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  0.2f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  0.2f, -1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, mTexture[1]); //bottom
    glBegin(GL_QUADS);
        glNormal3f( 0.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -0.2f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -0.2f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -0.2f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -0.2f,  1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, mTexture[2]);//side
    glBegin(GL_QUADS);
        glNormal3f( 0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -0.2f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -0.2f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  0.2f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  0.2f,  1.0f);

        glNormal3f( 0.0f, 0.0f,-1.0f);//side
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -0.2f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  0.2f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  0.2f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -0.2f, -1.0f);

        glNormal3f( 1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -0.2f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  0.2f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  0.2f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -0.2f,  1.0f);

        glNormal3f(-1.0f, 0.0f, 0.0f);//side
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -0.2f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -0.2f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  0.2f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  0.2f, -1.0f);
        glEnd();

}

void RotatingCube::setRotation(double xRot, double yRot, MainWindow::gyroData gd)
{
    ExerciseWindow ew;

    if(ew.abs(gd.xGyroSum) > ew.abs(xRot + xRot*0.1) || ew.abs(gd.xGyroSum) < ew.abs(xRot - xRot*0.1)){ //if the value is 10% less od higher
       gd.xGyroSum = xRot;
    }
    if(ew.abs(gd.yGyroSum) > ew.abs(xRot + xRot*0.1) || ew.abs(gd.yGyroSum) < ew.abs(xRot - xRot*0.1)){ //if the value is 10% less od higher
       gd.yGyroSum = yRot;
    }
    mXRotate = 0.7*gd.xGyroSum + 0.3 * xRot;
    mYRotate = 0.7*gd.yGyroSum + 0.3 * yRot;
    mZRotate = gd.zGyroSum;
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

