#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QMetaType>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void start();
    int counter = 0;
    int sampleRate = 0;
    struct data
    {
        double xGyroSample;
        double yGyroSample;
        double zGyroSample;
        double xAccelSample;
        double yAccelSample;
        double zAccelSample;
    };
//    Q_DECLARE_METATYPE(MainWindow::data)

    MainWindow::data filterDataCreateStruct(QStringList StringValues);
    MainWindow::data readUrlData();

protected:
    double xAccelSum = 0;
    double yAccelSum = 0;
    double zAccelSum = 0;

    double xGyroOffsetSum = 0;
    double yGyroOffsetSum = 0;
    double zGyroOffsetSum = 0;
    double xAccelOffsetSum = 0;
    double yAccelOffsetSum = 0;
    double zAccelOffsetSum = 0;

    double xGyroOffset = 0;
    double yGyroOffset = 0;
    double zGyroOffset = 0;
    double xAccelOffset = 0;
    double yAccelOffset = 0;
    double zAccelOffset = 0;

    double dataToDouble(QStringList StringValues, int position);

    double dist(double a, double b);
    double dist(double a, double b, double c);
    double getYrotation(MainWindow::data cleanData);
    double getXrotation(MainWindow::data cleanData);
    QTime timer;
    void createTable(MainWindow::data cleanData);
private slots:

    void on_URL_OK_clicked();
    void on_pause_clicked();
    void on_exercise_button_clicked();
    void on_rawData_clicked();

    void on_positionVelocity_clicked();

signals:

    void onNumber(MainWindow::data cleanData);
    void onStop();

public slots:

    void stop();

private:
    Ui::MainWindow *ui;
    void removeFlickering();
    void calculateOffset();
    bool mStop;
};

#endif // MAINWINDOW_H
