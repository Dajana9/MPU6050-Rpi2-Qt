#-------------------------------------------------
#
# Project created by QtCreator 2017-05-23T14:30:20
#
#-------------------------------------------------

QT       += core gui concurrent opengl
QT       += network multimedia
QT       += widgets
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RpiMpu6050
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    RotatingCube.cpp \
    PlotData.cpp \
    qcustomplot.cpp \
    ExerciseWindow.cpp \
    FileReader.cpp \
    PositionVelocity.cpp

HEADERS  += MainWindow.h \
    RotatingCube.h \
    PlotData.h \
    qcustomplot.h \
    ExerciseWindow.h \
    FileReader.h \
    PositionVelocity.h

FORMS    += MainWindow.ui \
    PlotData.ui \
    ExerciseWindow.ui \
    PositionVelocity.ui

RESOURCES += \
    rpimpu6050.qrc

LIBS += -lglut -lGLU
