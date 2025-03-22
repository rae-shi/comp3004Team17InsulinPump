#include <QApplication>
#include <QMainWindow>
#include "mainwindow.h"  // if you’re using MainWindow UI

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
