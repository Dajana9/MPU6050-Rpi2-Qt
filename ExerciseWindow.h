#ifndef EXERCISEWINDOW_H
#define EXERCISEWINDOW_H

#include <QMainWindow>
#include "MainWindow.h"
#include <QtMultimedia>

namespace Ui {
class ExerciseWindow;
}

class ExerciseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExerciseWindow(QWidget *parent = 0);
    ~ExerciseWindow();

private slots:
    void on_start_clicked();

    void on_stop_clicked();

    void on_recData_clicked();

    void on_stopRec_clicked();

public slots:
    void newNumber(MainWindow::data cleanData);
    void rec();
    void stopRec();

protected:
    double xGyroSum = 0;
    double yGyroSum = 0;
    double xGyroTmp = 0;
    double yGyroTmp = 0;

    double diffAbs(double value1, double value2);
    double abs(double value);
    QFile recData;
signals:
     void onStop();

private:
    Ui::ExerciseWindow *ui;
    MainWindow mainwin;
    QMediaPlayer* player;
    bool mRec;
};

#endif // EXERCISEWINDOW_H
