#include "PlotData.h"
#include "ui_PlotData.h"
#include "MainWindow.h"
#include <cmath>

PlotData::PlotData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotData)
{
    ui->setupUi(this);
    setupParametars();
}

void PlotData::setupParametars()
{

    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->xAccel->setStyleSheet("background-color:rgba(40, 110, 255, 60)");

    ui->plot->addGraph();
    ui->plot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->yAccel->setStyleSheet("background-color:rgba(255, 110, 40)");

    ui->plot->addGraph();
    ui->plot->graph(2)->setPen(QPen(QColor(110, 255, 40)));
    ui->zAccel->setStyleSheet("background-color:rgba(110, 255, 40, 60)");

    ui->plot->addGraph();
    ui->plot->graph(3)->setPen(QPen(QColor(255, 40, 255)));
    ui->xGyro->setStyleSheet("background-color:rgba(255, 40, 255, 60)");

    ui->plot->addGraph();
    ui->plot->graph(4)->setPen(QPen(QColor(110, 110, 40)));
    ui->yGyro->setStyleSheet("background-color:rgba(110, 110, 40, 60)");

    ui->plot->addGraph();
    ui->plot->graph(5)->setPen(QPen(QColor(255, 51, 0)));
    ui->zGyro->setStyleSheet("background-color:rgba(255, 51, 0, 60)");


    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plot->xAxis->setTicker(timeTicker);
    ui->plot->axisRect()->setupFullAxesBox();
    ui->plot->yAxis->setRange(-1.2, 1.2);


  //  qRegisterMetaType<QCPRange>("QCPRange");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->yAxis2, SLOT(setRange(QCPRange)));
//    connect(accelValues,SIGNAL(clicked),this,plotAccelData());


    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
   // connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    //dataTimer.start(0); // Interval 0 means to refresh as fast as possible

}

PlotData::~PlotData()
{
    emit onStop();
    delete ui;
}


void PlotData::plotAxisData(int AxesDecision,MainWindow::data cleanData)
{

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/100.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.0002) // at most add point every 2 ms
    {
    switch (AxesDecision){
        case 0:
        // add data to lines:
        ui->plot->graph(0)->addData(key,cleanData.xAccelSample);
        //rescale value (vertical) axis to fit the current data:
        ui->plot->graph(0)->rescaleValueAxis(true);
        ui->plot->graph(0)->rescaleKeyAxis(true);
        break;

        case 1:
        ui->plot->graph(1)->addData(key,cleanData.yAccelSample);
        ui->plot->graph(1)->rescaleValueAxis(true);
        ui->plot->graph(1)->rescaleKeyAxis(true);
        break;

        case 2:
        ui->plot->graph(2)->addData(key,cleanData.zAccelSample);
        ui->plot->graph(2)->rescaleValueAxis(true);
        ui->plot->graph(2)->rescaleKeyAxis(true);
        break;

        case 3:
        ui->plot->graph(3)->addData(key, cleanData.xGyroSample);
        ui->plot->graph(3)->rescaleValueAxis(true);
        ui->plot->graph(3)->rescaleKeyAxis(true);
        break;

        case 4:
        ui->plot->graph(4)->addData(key, cleanData.yGyroSample);
        ui->plot->graph(4)->rescaleValueAxis(true);
        ui->plot->graph(4)->rescaleKeyAxis(true);
        break;

        case 5:
        ui->plot->graph(5)->addData(key,cleanData.zGyroSample);
        ui->plot->graph(5)->rescaleValueAxis(true);
        ui->plot->graph(5)->rescaleKeyAxis(true);
        break;

        }
    lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plot->xAxis->setRange(key, 15, Qt::AlignRight);
    ui->plot->replot();

}


void PlotData::on_sensorValues_clicked()
{
    qRegisterMetaType<MainWindow::data>("MainWindow::data");

    connect(&mainwin,&MainWindow::onNumber,this,&PlotData::newNumber);
    connect(this,&PlotData::onStop, &mainwin,&MainWindow::stop);


    QFuture<void> test = QtConcurrent::run(&this->mainwin,&MainWindow::start);

}

void PlotData::newNumber(MainWindow::data cleanData)
{

    if(ui->xAccel->isChecked()){
     plotAxisData(0, cleanData);}
    if(ui->yAccel->isChecked()){
     plotAxisData(1, cleanData);}
    if(ui->zAccel->isChecked()){
     plotAxisData(2, cleanData);}
    if(ui->xGyro->isChecked()){
     plotAxisData(3, cleanData);}
    if(ui->yGyro->isChecked()){
     plotAxisData(4, cleanData);}
    if(ui->zGyro->isChecked()){
     plotAxisData(5, cleanData);}

}

void PlotData::on_pause_clicked()
{
     emit onStop();
     mStop = true;

}

void PlotData::stopRec()
{
    mStop = true;
}


void PlotData::on_recData_clicked()
{
    qRegisterMetaType<FileReader::recDataVariables>("FileReader::recDataVariables");

    connect(this, &PlotData::onStop, &fileReader, &FileReader::stop);
    connect(&fileReader, &FileReader::read ,this, &PlotData::newRecNumber);

    QFuture<void> test1 = QtConcurrent::run(&this->fileReader,&FileReader::readData);

   // start();

}

void PlotData::newRecNumber(FileReader::recDataVariables recData)
{

    MainWindow::data cleanData;

    cleanData.xAccelSample = recData.xAccelSample;
    cleanData.yAccelSample = recData.yAccelSample;
    cleanData.zAccelSample = recData.zAccelSample;
    cleanData.xGyroSample = recData.xGyroSample;
    cleanData.yGyroSample = recData.yGyroSample;
    cleanData.zGyroSample = recData.zGyroSample;

    if(ui->xAccel->isChecked()){
     plotAxisData(0, cleanData);}
    if(ui->yAccel->isChecked()){
     plotAxisData(1, cleanData);}
    if(ui->zAccel->isChecked()){
     plotAxisData(2, cleanData);}
    if(ui->xGyro->isChecked()){
     plotAxisData(3, cleanData);}
    if(ui->yGyro->isChecked()){
     plotAxisData(4, cleanData);}
    if(ui->zGyro->isChecked()){
     plotAxisData(5, cleanData);}

}


void PlotData::on_restart_clicked()
{
    ui->plot->yAxis->setRange(-1.2, 1.2);
    ui->plot->replot();

}
void PlotData::start(){


   /* mStop = false;

    MainWindow::data cleanData;
    QFile recData(QCoreApplication::applicationDirPath() + "/recData.csv");
    QTextStream in(&recData);
    QStringList data;
    if(!recData.open(QFile::ReadOnly|QIODevice::Text))
        return;
    int count = 0;
    while (mStop==false) {
    count++;
        QString line = in.readLine();
        //QThread::msleep(100);
        // sleep(1);
        data = line.split(",");

        cleanData.xAccelSample = data[0].toDouble();
        cleanData.yAccelSample = data[1].toDouble();
        cleanData.zAccelSample = data[2].toDouble();
        cleanData.xGyroSample = data[3].toDouble();
        cleanData.yGyroSample = data[4].toDouble();
        cleanData.zGyroSample = data[5].toDouble();


        if(ui->xAccel->isChecked()){
         plotAxisData(0, cleanData);}
        if(ui->yAccel->isChecked()){
         plotAxisData(1, cleanData);}
        if(ui->zAccel->isChecked()){
         plotAxisData(2, cleanData);}
        if(ui->xGyro->isChecked()){
         plotAxisData(3, cleanData);}
        if(ui->yGyro->isChecked()){
         plotAxisData(4, cleanData);}
        if(ui->zGyro->isChecked()){
         plotAxisData(5, cleanData);}

        qDebug()<<count;
        qDebug()<<data;
        qDebug()<<recData.size();
        }
    qDebug()<<"alo";
       // recData.close();
*/}
