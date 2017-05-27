#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qRegisterMetaType<MainWindow::data>("MainWinow::data");
    w.show();

    return a.exec();
}
