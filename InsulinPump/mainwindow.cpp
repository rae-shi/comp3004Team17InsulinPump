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

    connectAllSlots();
    disableAllInput();
    ui->startButton->setEnabled(true);
    ui->chargeButton->setEnabled(true);

    device->setupDevice();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectAllSlots(){
    // Connections
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->bolusButton, &QPushButton::clicked, this, &MainWindow::onBolusClicked);
    connect(ui->chargeButton, &QPushButton::clicked, this, &MainWindow::onChargeClicked);

    connect(device, &Device::batteryLevelChanged, this, &MainWindow::updateBattery);
    connect(device, &Device::logEvent, this, &MainWindow::appendLog);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::glucoseChanged, this, &MainWindow::updateGlucose);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::IOBChanged, this, &MainWindow::updateIOB);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::cartChanged, this, &MainWindow::updateCart);

    connect(simulationTimer, &QTimer::timeout, device, &Device::runDevice);
}

void MainWindow::disconnectAllSlots(){
    disconnect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    disconnect(ui->bolusButton, &QPushButton::clicked, this, &MainWindow::onBolusClicked);

    disconnect(device, &Device::batteryLevelChanged, this, &MainWindow::updateBattery);
    disconnect(device, &Device::logEvent, this, &MainWindow::appendLog);
    disconnect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::glucoseChanged, this, &MainWindow::updateGlucose);
    disconnect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::IOBChanged, this, &MainWindow::updateIOB);

    disconnect(simulationTimer, &QTimer::timeout, device, &Device::runDevice);
}

void MainWindow::enableAllInput(){
    //buttons
    ui->bolusButton->setEnabled(true);
    ui->chargeButton->setEnabled(true);
    ui->createProfileButton->setEnabled(true);
    ui->defaultProfileRadioButton->setEnabled(true);
    ui->deleteProfileButton->setEnabled(true);
    ui->disconnectButton->setEnabled(true);
    ui->startButton->setEnabled(true);
    ui->submitProfileButton->setEnabled(true);
    ui->bolusOverrideButton->setEnabled(true);

    //spin boxes
    ui->bolusHourSpinBox->setEnabled(true);
    ui->bolusMinuteSpinBox->setEnabled(true);
    ui->bolusUnitSpinBox->setEnabled(true);
    ui->deafultProfileGlucoseSpinBox->setEnabled(true);
    ui->defaultProfileBasalSpinBox->setEnabled(true);
}

void MainWindow::disableAllInput(){
    //buttons
    ui->bolusButton->setEnabled(false);
    ui->chargeButton->setEnabled(false);
    ui->createProfileButton->setEnabled(false);
    ui->defaultProfileRadioButton->setEnabled(false);
    ui->deleteProfileButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->submitProfileButton->setEnabled(false);
    ui->bolusOverrideButton->setEnabled(false);

    //spin boxes
    ui->bolusHourSpinBox->setEnabled(false);
    ui->bolusMinuteSpinBox->setEnabled(false);
    ui->bolusUnitSpinBox->setEnabled(false);
    ui->deafultProfileGlucoseSpinBox->setEnabled(false);
    ui->defaultProfileBasalSpinBox->setEnabled(false);
}

void MainWindow::onStartClicked() {
    if (simulationTimer->isActive()) {
        simulationTimer->stop();
        device->stopDevice();
        appendLog("Power off.");
        disableAllInput();
        ui->startButton->setEnabled(true);
        ui->chargeButton->setEnabled(true);
        ui->startButton->setText("Power On");
    } else {
        device->startDevice();
        simulationTimer->start(1000); // 1 second = 1 time step
        appendLog("Power on.");
        enableAllInput();
        ui->startButton->setText("Power Off");
    }
}

void MainWindow::onBolusClicked() {
    device->findChild<InsulinControlSystem*>()->simulateBolus(8.5); // Glucose level only, no carbs
}

void MainWindow::onChargeClicked(){
    if(ui->batteryBar->value()!=100){
        ui->batteryBar->setValue(100);
        appendLog("Battery charged to 100%.");
    }
}

void MainWindow::updateBattery(int level) {
    ui->batteryBar->setValue(level);
}

void MainWindow::updateCart(double level) {
    ui->cartridge->setValue(level);
    // use formating because value is a double
    ui->cartridge->setFormat(QString("Cartridge: %1 u").arg(level, 0, 'f', 2));
}


void MainWindow::updateGlucose(double level) {
    ui->glucoseLabel->setText(QString("Current Glucose: %1 mmol/L").arg(level, 0, 'f', 1));
}

void MainWindow::updateIOB(double level) {
    ui->IOBLabel->setText(QString("Insulin On Board (IOB): %1u | time hrs").arg(level, 0, 'f', 1));
}

void MainWindow::appendLog(const QString &msg) {
    ui->logOutput->appendPlainText(msg);
}
