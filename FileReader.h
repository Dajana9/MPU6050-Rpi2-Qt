#ifndef FILEREADER_H
#define FILEREADER_H
#include "MainWindow.h"

#include <QObject>


class FileReader : public QObject
{
    Q_OBJECT

public:
    explicit FileReader(QObject *parent = 0);

    void readData();

    struct recDataVariables
    {
        double xGyroSample;
        double yGyroSample;
        double zGyroSample;
        double xAccelSample;
        double yAccelSample;
        double zAccelSample;
    };
signals:
    void read(FileReader::recDataVariables cleanData);

public slots:
    void stop();

private:
    bool mStop;
protected:
    FileReader::recDataVariables cleanData;
    QStringList data;
    QString line;
    QTextStream in;
    QFile recData;
};

#endif // FILEREADER_H
