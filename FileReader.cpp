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

    while ( mStop == false ) {
        count++;

        if(line.isNull())
          line.begin();
        line = in.readLine();
        QThread::msleep(100);
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
