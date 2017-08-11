#include "PositionVelocity.h"
#include "ui_PositionVelocity.h"
#include "MainWindow.h"
#include <cmath>
#include "ExerciseWindow.h"

PositionVelocity::PositionVelocity(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PositionVelocity)
{
    ui->setupUi(this);
    QCustomPlot *x;
    x = ui->plotAccel;
    setupParametars(x);
    x = ui->plotVelocity;
    setupParametars(x);
    x = ui->plotPosition;
    setupParametars(x);

}

void PositionVelocity::setupParametars(QCustomPlot *plot)
{
    plot->addGraph();
    plot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
   // ui->xAccel->setStyleSheet("background-color:rgba(40, 110, 255, 60)");

    plot->addGraph();
    plot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    //ui->yAccel->setStyleSheet("background-color:rgba(255, 110, 40)");

    plot->addGraph();
    plot->graph(2)->setPen(QPen(QColor(110, 255, 40)));
    //ui->zAccel->setStyleSheet("background-color:rgba(110, 255, 40, 60)");

    plot->addGraph();
    plot->graph(3)->setPen(QPen(QColor(255, 40, 255)));

    plot->addGraph();
    plot->graph(4)->setPen(QPen(QColor(255, 40, 255)));

    plot->setInteraction(QCP::iRangeDrag,true);
    plot->setInteraction(QCP::iRangeZoom,true);

    timer.start();
    timer2.start();

}

PositionVelocity::~PositionVelocity()
{
    emit onStop();
    delete ui;
}


void PositionVelocity::plotAxisData(int AxesDecision,double accel, double velocity, double position)
{

    static QTime time(QTime::currentTime());
    //calculate two new data points:
    double key = time.elapsed()/100.0; // time elapsed since start of demo, in seconds ( without /100 is ms)
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.0005) // at most add point every 2 ms
    {
    switch (AxesDecision){
        case 0:

        ui->plotAccel->yAxis->setLabel("Accelerometar values");
        // add data to lines:
        ui->plotAccel->graph(0)->addData(key,accel);
        //rescale value (vertical) axis to fit the current data:
        ui->plotAccel->graph(0)->rescaleValueAxis(true);
        ui->plotAccel->graph(0)->rescaleKeyAxis(true);

        ui->plotVelocity->yAxis->setLabel("Velocity values");
        ui->plotVelocity->graph(0)->addData(key,velocity);
        //rescale value (vertical) axis to fit the current data:
        ui->plotVelocity->graph(0)->rescaleValueAxis(true);
        ui->plotVelocity->graph(0)->rescaleKeyAxis(true);

        ui->plotPosition->yAxis->setLabel("Position values");
        ui->plotPosition->graph(0)->addData(key,position);
        ui->plotPosition->graph(0)->rescaleValueAxis(true);
        ui->plotPosition->graph(0)->rescaleKeyAxis(true);
        break;

        case 1:
        ui->plotAccel->yAxis->setLabel("Accelerometar values");
        ui->plotAccel->graph(1)->addData(key,accel);
        //rescale value (vertical) axis to fit the current data:
        ui->plotAccel->graph(1)->rescaleValueAxis(true);
        ui->plotAccel->graph(1)->rescaleKeyAxis(true);

        ui->plotVelocity->yAxis->setLabel("Velocity values");
        ui->plotVelocity->graph(1)->addData(key,velocity);
        //rescale value (vertical) axis to fit the current data:
        ui->plotVelocity->graph(1)->rescaleValueAxis(true);
        ui->plotVelocity->graph(1)->rescaleKeyAxis(true);


        break;

        case 2:
        ui->plotAccel->yAxis->setLabel("Accelerometar values");
        ui->plotAccel->graph(2)->addData(key,accel);
        //rescale value (vertical) axis to fit the current data:
        ui->plotAccel->graph(2)->rescaleValueAxis(true);
        ui->plotAccel->graph(2)->rescaleKeyAxis(true);

        ui->plotVelocity->yAxis->setLabel("Velocity values");
        ui->plotVelocity->graph(2)->addData(key,velocity);
        //rescale value (vertical) axis to fit the current data:
        ui->plotVelocity->graph(2)->rescaleValueAxis(true);
        ui->plotVelocity->graph(2)->rescaleKeyAxis(true);

        ui->plotPosition->yAxis->setLabel("Position values");
        ui->plotPosition->graph(2)->addData(key,position);
        //rescale value (vertical) axis to fit the current data:
        ui->plotPosition->graph(2)->rescaleValueAxis(true);
        ui->plotPosition->graph(2)->rescaleKeyAxis(true);
        break;

    case 3:
        ui->plotPosition->yAxis->setRange(-0.5,0.5);
        ui->plotVelocity->yAxis->setRange(-0.5,0.5);
        ui->plotAccel->yAxis->setRange(-0.5,0.5);
        break;
        }
    ui->plotAccel->graph(3)->addData(key,5);
    ui->plotAccel->graph(4)->addData(key,-5);

    lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotAccel->xAxis->setRange(key, 20, Qt::AlignRight);
    ui->plotVelocity->xAxis->setRange(key, 20, Qt::AlignRight);
    ui->plotPosition->xAxis->setRange(key, 20, Qt::AlignRight);

    ui->plotAccel->replot();
    ui->plotVelocity->replot();
    ui->plotPosition->replot();

}

void PositionVelocity::on_sensorValues_clicked()
{
    qRegisterMetaType<MainWindow::data>("MainWindow::data");

    connect(&mainwin,&MainWindow::onNumber,this,&PositionVelocity::newNumber);
    connect(this,&PositionVelocity::onStop, &mainwin,&MainWindow::stop);


    QFuture<void> test = QtConcurrent::run(&this->mainwin,&MainWindow::start);
    timer2.start();
}

void PositionVelocity::newNumber(MainWindow::data cleanData)
{

    ExerciseWindow ew;
    ax1 = cos(cleanData.zGyroSample)*cleanData.xAccelSample + sin(cleanData.zGyroSample)*cleanData.yAccelSample; //akceleration in x direction
    ay1 = -sin(cleanData.zGyroSample)*cleanData.xAccelSample + cos(cleanData.zGyroSample)*cleanData.yAccelSample; //akceleration in y direction

    double deltaT = 0.005; // sampleRate
    double xGyro = cleanData.xGyroSample * 36;
    ax1 = ax1 * 100;
    ay1 = ay1 * 100;

    if(ui->xAxis->isChecked()){
        if(ew.abs(ax1) > 15){ //start integration
            if(timer2.elapsed() > 500){
                timer2.restart();
                timer3.start();
            }


            if(cleanData.yGyroSample > 15){ //remove slope
                ax1 = ax1 - cleanData.yGyroSample;
                }
            double vx0 = vx1;
            vx1 += (ew.abs(ax1) + ew.abs(ax0))/2 * deltaT;
            sx1 += (ew.abs(vx1) + ew.abs(vx0))/2 * deltaT;
        }
    }
    if(ui->yAxis->isChecked()){
        if(ew.abs(ay1) > 15){
            timer2.restart();
            timer3.start();

            if(xGyro > 15){
                ay1 = ay1 - xGyro;
                qDebug()<<xGyro;
                }
            vy1 = vy0 + (ay1 + ay0)/2 * deltaT;
            sy1 = sy0 + (vy1 + vy0)/2 * deltaT;

        }
    }


    int index = ui->moveType->currentIndex();
    if(index == 1){
        //ax = 0
        //vx = konst;
        //sx = raste
        vx = vx1;
        if(timer3.elapsed() > 500){
            timer3.restart();
            sx +=sx1;
        }
    }
    if(index == 2){
        //ax = konst
        //vx = raste/pada
        //sx = raste/pada
        if(timer3.elapsed() > 500){
            timer3.restart();
        vx += vx1;
        sx += (vx - vx1)/2 * deltaT;
        }
    }
    if(index == 3)
        if(timer3.elapsed()>500){
            //ax = movemant in 0.5 seconds after that wait for another
            vx1 = 0;
            vx = 0;
            sx1 = 0;
            sx = 0;
            ax0 = 0;
    }

    if(ui->xAxis->isChecked()){
        plotAxisData(0,ax1,vx,sx);    }
    if(ui->yAxis->isChecked()){
        plotAxisData(1,ay1,vy1,sy1);    }
   // if(ui->zAxis->isChecked()){
     //   plotAxisData(1,ax1,vx1,sx1);    }
}

void PositionVelocity::on_pause_clicked()
{
     emit onStop();
     mStop = true;

}
double PositionVelocity::abs(double value){
return sqrt(value*value);
}


void PositionVelocity::on_restart_clicked()
{
    emit onStop();
    plotAxisData(3,0,0,0);


}

void PositionVelocity::on_plot2D_clicked()
{

}
