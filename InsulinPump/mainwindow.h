#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "insulinpump.h"
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>


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
    void onChargeClicked();
    void onPauseInClicked();
    void onDisconnectClicked();
    void onOcclusionClicked();
    void onSubmitProfileClicked();
    void onEditProfileClicked();
    void onBatteryDepleted(); // New slot for battery depletion
    void updateBattery(int level);
    void updateCart(double level);
    void updateGlucose(double level);
    void updateIOB(double level, double hours);
    void appendLog(const QString &msg);
    void appendErrorLog (const QString &msg);
    void onRefillCartridgeClicked();
    void onDepleteBatteryClicked();
    void onDepleteCartridgeClicked();
    void decrementBattery();
    void decrementCartridge();
    void checkHistory();

signals:
    void profileUpdated(double basalRate, double correctionFactor, int carbRatio, double targetGlucose);


private:
    Ui::MainWindow *ui;
    Device *device;
    QTimer *simulationTimer;
    QChart *chart;
    QChartView *chartView;
    QLineSeries *series;

    void connectAllSlots();
    void disconnectAllSlots();
    void enableAllInput();
    void disableAllInput();
    void onCalculateBolus();
    void initializeGraph();
    void addPoint(int time, double glucose);
};

#endif // MAINWINDOW_H
