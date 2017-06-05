#ifndef POSITIONVELOCITY_H
#define POSITIONVELOCITY_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "MainWindow.h"

namespace Ui {
class PositionVelocity;
}

class PositionVelocity : public QMainWindow
{
    Q_OBJECT

public:
    explicit PositionVelocity(QWidget *parent = 0);
    ~PositionVelocity();

    QTime timer;
    double abs(double value);
public slots:
    void on_sensorValues_clicked();
    void newNumber(MainWindow::data cleanData);
    void on_pause_clicked();
protected:
    double velocity = 0;
    double zAccelTmp = 0;
    double zPos = 0;
    int counter = 0;
    int zMax;
    void plotAxisData(int AxesDecision, double accel, double velocity, double position);
    void calculateVelocityPosition(int axisChoice, MainWindow::data cleanData);
    double ax0 = 0;
    double ax1 = 0;
    double sx0 = 0;
    double sx1 = 0;
    double vx0 = 0;
    double vx1 = 0;
    double sumSx = 0;
    double max = 0;

    double ay0 = 0;
    double ay1 = 0;
    double sy0 = 0;
    double sy1 = 0;
    double vy0 = 0;
    double vy1 = 0;

signals:
    void onStop();
private slots:
    void on_restart_clicked();

    void on_plot2D_clicked();

private:
    Ui::PositionVelocity *ui;
    void setupParametars(QCustomPlot *plot);
    MainWindow mainwin;
    bool mStop;
};

#endif // POSITIONVELOCITY_H
