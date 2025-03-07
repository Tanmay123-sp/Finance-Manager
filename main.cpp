#include "mainwindow.h"
#include<QCoreApplication>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // QCoreApplication::setApplicationName("Personal Finance Manager");
    MainWindow w;
    w.show();
    return a.exec();
}
