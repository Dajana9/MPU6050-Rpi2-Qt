#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "PlotData.h"
#include "ExerciseWindow.h"
#include "RotatingCube.h"
#include "PositionVelocity.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::data MainWindow::readUrlData()
{
    QNetworkAccessManager manager;
    QNetworkReply *response;
    QEventLoop event;
    QString html;
    static QString urlTxt = ui->URL->text();
    response = manager.get(QNetworkRequest(QUrl(urlTxt))); //mob http://192.168.43.50:8080/
    QObject::connect(response,SIGNAL(finished()),&event,SLOT(quit()));
    event.exec();
    html = response->readAll();
    QStringList accelerometerAndGyroscopeData = html.split(" ");

    data cleanData = filterDataCreateStruct(accelerometerAndGyroscopeData);
    return cleanData;
}

MainWindow::data MainWindow::filterDataCreateStruct(QStringList accelerometerAndGyroscopeData){
    data rawData,cleanData;

    rawData.xGyroSample = dataToDouble(accelerometerAndGyroscopeData, 0) * 0.01;
    rawData.yGyroSample = dataToDouble(accelerometerAndGyroscopeData, 1) * 0.01;
    rawData.zGyroSample = dataToDouble(accelerometerAndGyroscopeData, 2) * 0.01;
    rawData.xAccelSample = dataToDouble(accelerometerAndGyroscopeData, 3);
    rawData.yAccelSample = dataToDouble(accelerometerAndGyroscopeData, 4);
    rawData.zAccelSample = dataToDouble(accelerometerAndGyroscopeData, 5);

    if (counter < 10){
         xGyroOffsetSum +=rawData.xGyroSample;
         yGyroOffsetSum +=rawData.yGyroSample;
         zGyroOffsetSum +=rawData.zGyroSample;
         xAccelOffsetSum +=rawData.xAccelSample;
         yAccelOffsetSum +=rawData.yAccelSample;
         zAccelOffsetSum +=rawData.zAccelSample;

    }
    else if(counter == 10)
    {
        xGyroOffset = xGyroOffsetSum/10;
        yGyroOffset = yGyroOffsetSum/10;
        zGyroOffset = zGyroOffsetSum/10;
        xAccelOffset = xAccelOffsetSum/10; // zAccel is at 0.9 becuse of gravity
        yAccelOffset = yAccelOffsetSum/10;
        zAccelOffset = zAccelOffsetSum/10;
    }
    else if(counter > 10)
    {
        xAccelSum += (rawData.xAccelSample - xAccelOffset);
        yAccelSum += (rawData.yAccelSample - yAccelOffset);
        zAccelSum += (rawData.zAccelSample - zAccelOffset);

        if(counter % 2 == 0)
        {
            cleanData.xAccelSample = xAccelSum/2;
            cleanData.yAccelSample = yAccelSum/2;
            cleanData.zAccelSample = zAccelSum/2;

            xAccelSum = 0;
            yAccelSum = 0;
            zAccelSum = 0;
        }
    }
    counter++;

    cleanData.xGyroSample = rawData.xGyroSample - xGyroOffset;
    cleanData.yGyroSample = rawData.yGyroSample - yGyroOffset;
    cleanData.zGyroSample = rawData.zGyroSample - zGyroOffset;

    return cleanData;

}

void MainWindow::on_URL_OK_clicked()
{
    ui->URL_OK->setDisabled(true);
    connect(this, &MainWindow::onStop ,this, &MainWindow::stop);

    start();


    //connect(&network,&NetworkClass::on_number,this,&MainWindow::newNumber);
    //connect(this,&MainWindow::on_stop, &network,&NetworkClass::stop);

    //QFuture<void> test = QtConcurrent::run(&this->network,&NetworkClass::start);

}


void MainWindow::on_pause_clicked()
{
    ui->URL_OK->setEnabled(true);
    emit onStop();
}


void MainWindow::start()
{
    timer.start();
    sampleRate = 0;
    mStop = false;
    while(!mStop)
    {
        data cleanData = readUrlData();
        //send angle rotation to Cube
        if(counter > 15){
            ui->rotatingCube->setRotation(getXrotation(cleanData),getYrotation(cleanData));
            createTable(cleanData);
            emit onNumber(cleanData);
    }   }
}

void MainWindow::stop()
{
    mStop = true;
}

void MainWindow::createTable(MainWindow::data cleanData)
{
    ui->tableWidget->setColumnCount(6);
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row,0, new QTableWidgetItem(QString::number(cleanData.xAccelSample,'f',2)));
    ui->tableWidget->setItem(row,1, new QTableWidgetItem(QString::number(cleanData.yAccelSample,'f',2)));
    ui->tableWidget->setItem(row,2, new QTableWidgetItem(QString::number(cleanData.zAccelSample,'f',2)));
    ui->tableWidget->setItem(row,3, new QTableWidgetItem(QString::number(cleanData.xGyroSample,'f',2)));
    ui->tableWidget->setItem(row,4, new QTableWidgetItem(QString::number(cleanData.yGyroSample,'f',2)));
    ui->tableWidget->setItem(row,5, new QTableWidgetItem(QString::number(cleanData.zGyroSample,'f',2)));

    ui->tableWidget->resizeColumnsToContents();

    if(row < 15){
        ui->tableWidget->adjustSize();
    }else{
        ui->tableWidget->removeRow(0);
    }
}
double MainWindow::dataToDouble(QStringList StringValues, int position)
{
  return StringValues[position].toDouble();
}

void MainWindow::on_exercise_button_clicked()
{
    emit onStop();
    ExerciseWindow* exercisewindow = new ExerciseWindow(this);
    exercisewindow->show();
}

void MainWindow::on_rawData_clicked()
{
    emit onStop();
    PlotData* plotdata = new PlotData(this);
    plotdata->show();
}

double MainWindow::dist(double a,double b) {

    return sqrt((a*a)+(b*b));
}

double MainWindow::dist(double a,double b, double c) {

    return sqrt((a*a)+(b*b)+(c*c));
}

double MainWindow::getYrotation(MainWindow::data cleanData)
{
    double radians = atan2(cleanData.xAccelSample,dist(cleanData.yAccelSample,cleanData.zAccelSample + zAccelOffset));
    double degrees = radians * (180.0/3.14);

    return degrees;
}

double MainWindow::getXrotation(MainWindow::data cleanData)
{
    double radians = atan2(cleanData.yAccelSample,dist(cleanData.xAccelSample,cleanData.zAccelSample + zAccelOffset));
    double degrees = radians * (180.0/3.14);

    return degrees;
}

void MainWindow::on_positionVelocity_clicked()
{
    emit onStop();
    PositionVelocity* positionvelocity = new PositionVelocity(this);
    positionvelocity->show();
}
