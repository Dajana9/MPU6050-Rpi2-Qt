#ifndef ROTATINGCUBE_H
#define ROTATINGCUBE_H

#include <QWidget>
#include <QGLWidget>
#include <QOpenGLWidget>
#include <GL/glu.h>
#include <QGLWidget>
#include <QKeyEvent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QEventLoop>
#include <QObject>
#include <QtCore>
#include <QtGui>
#include <MainWindow.h>

class RotatingCube : public QGLWidget
{
    Q_OBJECT
public:
   explicit RotatingCube(QWidget *parent = 0);
    ~RotatingCube();


    void setRotationGyro(double x, double y, double z);



    void setRotation(double xRot, double yRot, MainWindow::gyroData gd);

    double diffAbs(double value1, double value2);
protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int width, int height);
    int texture = 0;

private:
    void loadGLTexture();

    bool mFullScreen;

    GLdouble mXRotate;
    GLdouble mYRotate;
    GLfloat mZRotate;
    GLfloat mXSpeed;
    GLfloat mYSpeed;
    GLfloat mZ;

    GLuint mFilter;
    GLuint mTexture[3];
    QNetworkAccessManager manager;
    QNetworkReply *response;
    QEventLoop event;
    QString html;
};

#endif // ROTATINGCUBE_H
