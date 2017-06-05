#include "FileReader.h"
#include "MainWindow.h"

FileReader::FileReader(QObject *parent) : QObject(parent)
{

}

void FileReader::readData(){

    QFile recData(QCoreApplication::applicationDirPath() + "/recData.csv");
    QTextStream in(&recData);
    if(!recData.open(QFile::ReadOnly|QIODevice::Text))
        return;
    mStop = false;
    int count = 0;

    while ( count < 3000 && mStop == false ) {
        count++;

        if(line.isNull())
          line.begin();
        line = in.readLine();
        QThread::msleep(50);// 20 samples in second is read--so 1/20 = 0.05 ann * 1000 = 50 ms
        // sleep(1);
        data = line.split(",");

        cleanData.xAccelSample = data[0].toDouble();
        cleanData.yAccelSample = data[1].toDouble();
        cleanData.zAccelSample = data[2].toDouble();
        cleanData.xGyroSample = data[3].toDouble();
        cleanData.yGyroSample = data[4].toDouble();
        cleanData.zGyroSample = data[5].toDouble();

        qDebug()<<count;
        qDebug()<<data;
        qDebug()<<in.ReadPastEnd;


        emit read(cleanData);
        }
    qDebug()<<"alo";
       // recData.close();
}

void FileReader::stop()
{
    mStop = true;
}
