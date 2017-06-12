#ifndef SHOWRECDATA_H
#define SHOWRECDATA_H

#include <QMainWindow>

namespace Ui {
class ShowRecData;
}

class ShowRecData : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShowRecData(QWidget *parent = 0);
    ~ShowRecData();

protected:
    void plot(int axis);
private slots:
    void on_xAccel_clicked();

    void on_yAccel_clicked();

    void on_zAccel_clicked();

    void on_xGyro_clicked();

    void on_yGyro_clicked();

    void on_zGyro_clicked();

    void on_clearAll_clicked();

private:
    Ui::ShowRecData *ui;
    void setupParametars();
};

#endif // SHOWRECDATA_H
