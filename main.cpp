#include "MainWindow.h"
#include "FileReader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qRegisterMetaType<MainWindow::data>("MainWinow::data");
    qRegisterMetaType<FileReader::recDataVariables>("FileReader::recDataVariables");

    w.show();

    return a.exec();
}
