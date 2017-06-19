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


//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%ms");
//    plot->xAxis->setTicker(timeTicker);
//    plot->axisRect()->setupFullAxesBox();
//    plot->yAxis->setRange(-0.2, 0.2);
//    plot->xAxis->setLabel("Time");

    plot->setInteraction(QCP::iRangeDrag,true);
    plot->setInteraction(QCP::iRangeZoom,true);

    timer.start();

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
        //rescale value (vertical) axis to fit the current data:
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

        ui->plotPosition->yAxis->setLabel("Position values");
        ui->plotPosition->graph(1)->addData(key,position);
        //rescale value (vertical) axis to fit the current data:
        ui->plotPosition->graph(1)->rescaleValueAxis(true);
        ui->plotPosition->graph(1)->rescaleKeyAxis(true);
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

    double deltaT = 0.05;

    ax1 = ax1 * 100;
    ay1 = ay1 * 100;


    if(ew.abs(ax1) > 15){
         vx1 = vx0 + (ew.abs(ax1) + ew.abs(ax0))/2 * deltaT;
         sx1 = sx0 + (ew.abs(vx1) + ew.abs(vx0))/2 * deltaT;
    }
    if(ew.abs(ay1) > 15){
         vy1 = vy0 + (ay1 + ay0)/2 * deltaT;
         sy1 = sy0 + (vy1 + vy0)/2 * deltaT;
    }



    if(timer2.elapsed()> 2){ // a needed a restart becouse of large drift in velocity
        timer2.restart();
        ax0 = 0;
        vx0 = 0;
        vx1 = 0;
        sx0 = 0;
        ay0 = 0;
        vy0 = 0;
    }
    else
    {
        ax0 = ax1;
        vx0 = vx1;
        sx0 = sx1;
        ay0 = ay1;
        vy0 = vy1;
        sy0 = sy1;
    }
    if(ui->xAxis->isChecked()){
        plotAxisData(1,ax1,vx1,sx1);    }
    if(ui->yAxis->isChecked()){
        plotAxisData(1,ay1,vy1,sy1);    }
   // if(ui->zAxis->isChecked()){
     //   plotAxisData(1,ax1,vx1,sx1);    }
}

void PositionVelocity::calculateVelocityPosition(int axisChoice, MainWindow::data cleanData)
{
    ExerciseWindow ew;

    if(ew.diffAbs(vx1,vx0) > 0.05 && (ew.abs(ax1) > 0.05)){
        vx1Total +=vx1;
    }
    if(ew.diffAbs(vy1,vy0) > 0.01 && (ew.abs(ay1) > 0.01)){
        vy1Total +=vy1;
    }
    if((ax0 > 0 && ax1 < 0 && vx1Total > 0.5)||(ax0 < 0 && ax1 > 0 && vx1Total < -0.5))
    {
    //sx1 = sx0 + (vx1 + vx0)/2 * deltaT;
    sx1 = vx1Total;
    }

    if((ay0 > 0 && ay1 < 0 && vy1Total > 0.5)||(ay0 < 0 && ay1 > 0 && vy1Total < -0.5))
    {
    //sx1 = sx0 + (vx1 + vx0)/2 * deltaT;
    sy1 = vy1Total;
    }



    //losije od zGyro ali vidi se rezultat
    ax1 = cos(cleanData.zGyroSample)*cleanData.xAccelSample + sin(cleanData.zGyroSample)*cleanData.yAccelSample; //akceleracija u X smjeru
    //ay = -sin(cleanData.zGyroSample)*cleanData.xAccelSample + cos(cleanData.zGyroSample)*cleanData.yAccelSample;
    // YAcc[i]=-sin(kut[i])*XAcc1[i]+cos(kut[i])*YAcc1[i]; //akceleracija u Y smjer

    int dt = 1/43;


    vx1 = vx0 * (ax1 + ax0)/2 * dt;
    sx1 = sx0 * (vx1 + vx0)/2 * dt;

    ax0 = ax1;
    vx0 = vx1;
    sx0 = sx1;
    plotAxisData(axisChoice,ax1,vx1,sx1);
   /* double value = cleanData.zAccelSample*100;


    double pi = 3.14;
    double D;
    D = ax1/(2* pi*pi * 43 * sin(2* pi * 43));
    //vx1 = 3.14 * 43 * D * cos(2*pi*43);
    //sx1 = D * sin(2*pi*43)/2;


    D = ay1/(2* pi*pi * 43 * sin(2* pi * 43));
    vy1 = 3.14 * 43 * D * cos(2*pi*43);
    sy1 = D * sin(2*pi*43)/2;



    int zMax;
    if(zMax < value){
        zMax = value;
    }else{
        //value = zMax;
    }
    counter++;

    if(timer.elapsed()>1)
    {
        if((zAccelTmp > 0 && cleanData.zAccelSample < 0 )||(zAccelTmp < 0 && cleanData.zAccelSample > 0 ))
        {
            velocity += timer.elapsed() * cleanData.zAccelSample;

            zPos = velocity / timer.elapsed();
            timer.restart();

        }
        zAccelTmp = cleanData.zAccelSample;
    }
    if(cleanData.zAccelSample < 1)
        zPos = abs(cleanData.xGyroSample*100) + abs(cleanData.yGyroSample*100);
    else if (cleanData.zAccelSample > 1)
        zPos = -abs(cleanData.xGyroSample*100) - abs(cleanData.yGyroSample*100);


    //last_x = K * (last_x + gyro_x_delta) + (K1 * rotation_x)
    //last_y = K * (last_y + gyro_y_delta) + (K1 * rotation_y)
    if(counter == 5){
        counter=0;
    }
        double cc =2*3.14*counter*200;
        zPos = -sin(cc);

    plotAxisData(2,value,velocity,zPos);
*/
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
