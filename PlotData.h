#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <QMainWindow>
#include "MainWindow.h"

namespace Ui {
class PlotData;
}

class PlotData : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotData(QWidget *parent = 0);
    ~PlotData();
public slots:

    void newNumber(MainWindow::data cleanData);
    void on_sensorValues_clicked();

signals:
     void onStop();

protected:
     void plotAxisData(int AxesDecision, MainWindow::data cleanData);
     void setupParametars();
private slots:

     void on_pause_clicked();
     void on_recData_clicked();

private:
    Ui::PlotData *ui;
    MainWindow mainwin;

};

#endif // PLOTDATA_H
