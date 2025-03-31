#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QtTest/QtTest>
#include "mainwindow.h"  // if you're using MainWindow UI

// Forward declaration of test class
class InsulinPumpTest;

// Function to run the tests
void runTests();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Run the unit tests first
    qDebug() << "===== STARTING INSULIN PUMP UNIT TESTS =====";
    runTests();
    qDebug() << "===== INSULIN PUMP UNIT TESTS COMPLETED =====";

    MainWindow w;
    w.show();

    return app.exec();
}