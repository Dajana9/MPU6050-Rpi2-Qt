#include "ShowRecData.h"
#include "ui_ShowRecData.h"
#include "MainWindow.h"
#include <cmath>
#include "ExerciseWindow.h"


ShowRecData::ShowRecData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShowRecData)
{
    ui->setupUi(this);

    setupParametars();
}

void ShowRecData::setupParametars()
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


   // ui->plot->axisRect()->setupFullAxesBox();
    //ui->plot->yAxis->setRange(-1, 1);
    ui->plot->yAxis->setLabel("Sensor values");

    ui->plot->setInteraction(QCP::iRangeDrag,true);
    ui->plot->setInteraction(QCP::iRangeZoom,true);
}

ShowRecData::~ShowRecData()
{
    delete ui;
}

void ShowRecData::plot(int axis)
{

    QFile recData(QCoreApplication::applicationDirPath() + "/recData.csv");
    QTextStream in(&recData);
    if(!recData.open(QFile::ReadOnly|QIODevice::Text))
        return;
    int count = 0;
    QString line;
    QStringList recDataList;
    line = in.readAll();
    line = line.replace("\n"," ");
    recDataList = line.split(" ");
    double recDouble;
    //ui->plot->xAxis->setRange(0,100);
    while (count < (recDataList.size()/6)){
        line = recDataList[axis + 6*count];
        recDouble = line.toDouble();
        ui->plot->graph(axis)->addData(count,recDouble);

        //rescale value (vertical) axis to fit the current data:
        //ui->plot->graph(0)->rescaleValueAxis(true);
        //ui->plot->graph(0)->rescaleKeyAxis(true);
        qDebug()<<recDouble;
        count++;
}
    ui->plot->replot();

}
void ShowRecData::on_xAccel_clicked()
{
    plot(0);
}

void ShowRecData::on_yAccel_clicked()
{
    plot(1);
}

void ShowRecData::on_zAccel_clicked()
{
    plot(2);
}

void ShowRecData::on_xGyro_clicked()
{
    plot(3);

}

void ShowRecData::on_yGyro_clicked()
{
    plot(4);
}

void ShowRecData::on_zGyro_clicked()
{
    plot(5);
}

void ShowRecData::on_clearAll_clicked()
{
    ui->plot->clearPlottables();
    ui->plot->replot();
    setupParametars();
    ui->xAccel->setChecked(false);
    ui->yAccel->setChecked(false);
    ui->zAccel->setChecked(false);
    ui->xGyro->setChecked(false);
    ui->yGyro->setChecked(false);
    ui->zGyro->setChecked(false);

}
