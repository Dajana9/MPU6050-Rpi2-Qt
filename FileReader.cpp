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
    QString allData;
    allData = in.readAll();
    allData = allData.replace("\n"," ");
    QStringList recDataList = allData.split(" ");
    int size = recDataList.size()/6 - 6;

    while (count <size && mStop == false ) {
        count++;
     //   recDouble = line.toDouble();

        //line = in.readLine();
        QThread::msleep(100);// 20 samples in second is read--so 1/20 = 0.05 ann * 1000 = 50 ms
        // sleep(1);
        cleanData.xAccelSample = recDataList[0 + 6*count].toDouble();
        cleanData.yAccelSample = recDataList[1 + 6*count].toDouble();
        cleanData.zAccelSample = recDataList[2 + 6*count].toDouble();
        cleanData.xGyroSample = recDataList[3 + 6*count].toDouble();
        cleanData.yGyroSample = recDataList[4 + 6*count].toDouble();
        cleanData.zGyroSample = recDataList[5 + 6*count].toDouble();

        qDebug()<<cleanData.xAccelSample;
        emit read(cleanData);
        }
}

void FileReader::stop()
{
    mStop = true;
}
