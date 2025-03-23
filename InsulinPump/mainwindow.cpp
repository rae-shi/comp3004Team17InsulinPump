#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , device(new Device(this))
    , simulationTimer(new QTimer(this))
{
    ui->setupUi(this);
    ui->batteryBar->setValue(100);

    // Connections
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->bolusButton, &QPushButton::clicked, this, &MainWindow::onBolusClicked);

    connect(device, &Device::batteryLevelChanged, this, &MainWindow::updateBattery);
    connect(device, &Device::logEvent, this, &MainWindow::appendLog);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::glucoseChanged, this, &MainWindow::updateGlucose);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::IOBChanged, this, &MainWindow::updateIOB);

    connect(simulationTimer, &QTimer::timeout, device, &Device::runDevice);

    device->setupDevice();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onStartClicked() {
    if (simulationTimer->isActive()) {
        simulationTimer->stop();
        device->stopDevice();
        appendLog("Simulation paused.");
        ui->startButton->setText("Start");
    } else {
        device->startDevice();
        simulationTimer->start(1000); // 1 second = 1 time step
        appendLog("Simulation started.");
        ui->startButton->setText("Pause");
    }
}

void MainWindow::onBolusClicked() {
    device->findChild<InsulinControlSystem*>()->simulateBolus(8.5); // Glucose level only, no carbs
}

void MainWindow::updateBattery(int level) {
    ui->batteryBar->setValue(level);
}

void MainWindow::updateGlucose(double level) {
    ui->glucoseLabel->setText(QString("Current Glucose: %1 mmol/L").arg(level, 0, 'f', 1));
}

void MainWindow::updateIOB(double level) {
    ui->IOB->setText(QString("IOB: %1 mmol/L").arg(level, 0, 'f', 1));
}

void MainWindow::appendLog(const QString &msg) {
    ui->logOutput->appendPlainText(msg);
}
