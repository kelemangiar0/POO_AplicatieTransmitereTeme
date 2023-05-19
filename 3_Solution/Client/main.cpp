#include "mainwindow.h"
#include "afterlogin.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("D:/Downloads/logo-removebg-preview.png"));
    w.show();

    return a.exec();
}
