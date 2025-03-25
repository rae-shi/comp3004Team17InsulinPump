#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "insulinpump.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();
    void onBolusClicked();
    void onChargeClicked();
    void updateBattery(int level);
    void updateCart(double level);
    void updateGlucose(double level);
    void updateIOB(double level);
    void appendLog(const QString &msg);

private:
    Ui::MainWindow *ui;
    Device *device;
    QTimer *simulationTimer;

    void connectAllSlots();
    void disconnectAllSlots();
    void enableAllInput();
    void disableAllInput();
};

#endif // MAINWINDOW_H
