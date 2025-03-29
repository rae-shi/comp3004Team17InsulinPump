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
    appendLog(QString("------------------"));
    appendLog("Profile defaulted to Morning.");
    emit profileUpdated(ui->morningBRSpinBox->value(), ui->morningCFSpinBox->value(), ui->morningCRSpinBox->value(), ui->morningBGSpinBox->value());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectAllSlots(){
    // Connections
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->bolusButton, &QPushButton::clicked, this, &MainWindow::onBolusClicked);
    connect(ui->chargeButton, &QPushButton::clicked, this, &MainWindow::onChargeClicked);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    connect(ui->submitProfileButton, &QPushButton::clicked, this, &MainWindow::onSubmitProfileClicked);
    connect(ui->editProfileButton, &QPushButton::clicked, this, &MainWindow::onEditProfileClicked);

    connect(device, &Device::batteryLevelChanged, this, &MainWindow::updateBattery);
    connect(device, &Device::logEvent, this, &MainWindow::appendLog);
    connect(device, &Device::devicePoweredOff, this, &MainWindow::onBatteryDepleted);
    connect(this, &MainWindow::profileUpdated, device, &Device::applyProfile);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::glucoseChanged, this, &MainWindow::updateGlucose);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::IOBChanged, this, &MainWindow::updateIOB);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::cartChanged, this, &MainWindow::updateCart);
    connect(ui->pauseIns, &QPushButton::clicked, this, &MainWindow::onPauseInClicked);

    connect(simulationTimer, &QTimer::timeout, device, &Device::runDevice);
}

void MainWindow::disconnectAllSlots(){
    disconnect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    disconnect(ui->bolusButton, &QPushButton::clicked, this, &MainWindow::onBolusClicked);
    disconnect(ui->chargeButton, &QPushButton::clicked, this, &MainWindow::onChargeClicked);
    disconnect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    disconnect(ui->submitProfileButton, &QPushButton::clicked, this, &MainWindow::onSubmitProfileClicked);
    disconnect(ui->editProfileButton, &QPushButton::clicked, this, &MainWindow::onEditProfileClicked);
    disconnect(ui->pauseIns, &QPushButton::clicked, this, &MainWindow::onPauseInClicked);

    disconnect(device, &Device::batteryLevelChanged, this, &MainWindow::updateBattery);
    disconnect(device, &Device::logEvent, this, &MainWindow::appendLog);
    disconnect(device, &Device::devicePoweredOff, this, &MainWindow::onBatteryDepleted);
    disconnect(this, &MainWindow::profileUpdated, device, &Device::applyProfile);
    disconnect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::glucoseChanged, this, &MainWindow::updateGlucose);
    disconnect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::IOBChanged, this, &MainWindow::updateIOB);

    disconnect(simulationTimer, &QTimer::timeout, device, &Device::runDevice);
}

void MainWindow::enableAllInput(){
    //buttons
    ui->bolusButton->setEnabled(true);
    ui->chargeButton->setEnabled(true);
    ui->disconnectButton->setEnabled(true);
    ui->startButton->setEnabled(true);
    ui->editProfileButton->setEnabled(true);
    ui->submitProfileButton->setEnabled(true);
    ui->bolusOverrideButton->setEnabled(true);

    //radio buttons
    ui->morningProfileRadioButton->setEnabled(true);
    ui->afternoonProfileRadioButton->setEnabled(true);
    ui->nightProfileRadioButton->setEnabled(true);

    //spin boxes
    ui->bolusHourSpinBox->setEnabled(true);
    ui->bolusMinuteSpinBox->setEnabled(true);
    ui->bolusUnitSpinBox->setEnabled(true);
    //leaving personal profile spin boxes out because counterproductive
}

void MainWindow::disableAllInput(){
    //buttons
    ui->bolusButton->setEnabled(false);
    ui->chargeButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->editProfileButton->setEnabled(false);
    ui->submitProfileButton->setEnabled(false);
    ui->bolusOverrideButton->setEnabled(false);

    //radio buttons
    ui->morningProfileRadioButton->setEnabled(false);
    ui->afternoonProfileRadioButton->setEnabled(false);
    ui->nightProfileRadioButton->setEnabled(false);

    //spin boxes
    ui->bolusHourSpinBox->setEnabled(false);
    ui->bolusMinuteSpinBox->setEnabled(false);
    ui->bolusUnitSpinBox->setEnabled(false);
    //leaving personal profile spin boxes out because counterproductive
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

void MainWindow::onPauseInClicked() {

    if (ui->pauseIns->text() == "Pause Insulin"){
        appendLog("Insulin delivery paused.");
        device->findChild<InsulinControlSystem*>()->setState(InsulinControlSystem::Pause);
        ui->pauseIns->setEnabled(true);
        ui->pauseIns->setText("Resume Insulin");
    } else if (ui->pauseIns->text() == "Resume Insulin"){
        appendLog("Insulin delivery resumed.");
        device->findChild<InsulinControlSystem*>()->setState(InsulinControlSystem::Resume);
        ui->pauseIns->setEnabled(true);
        ui->pauseIns->setText("Pause Insulin");
    }

}

void MainWindow::onBolusClicked() {
    device->findChild<InsulinControlSystem*>()->simulateBolus(8.5); // Glucose level only, no carbs
}

void MainWindow::onChargeClicked(){
    if(ui->batteryBar->value()!=100){
        device->chargeBattery();
    }
}

void MainWindow::onDisconnectClicked(){
    appendLog(QString("------------------"));
    appendLog("!! Device Disconnected from Person !!\nPlease reconnect and power the system back on.");
    onStartClicked();
}

void MainWindow::onSubmitProfileClicked(){
    appendLog(QString("------------------"));
    if(ui->morningProfileRadioButton->isChecked()){
        appendLog("Profile set to Morning.");
        ui->morningBRSpinBox->setEnabled(false);
        ui->morningCFSpinBox->setEnabled(false);
        ui->morningCRSpinBox->setEnabled(false);
        ui->morningBGSpinBox->setEnabled(false);
        emit profileUpdated(ui->morningBRSpinBox->value(), ui->morningCFSpinBox->value(), ui->morningCRSpinBox->value(), ui->morningBGSpinBox->value());
    }else if(ui->afternoonProfileRadioButton->isChecked()){
        appendLog("Profile set to Afternoon.");
        ui->afternoonBRSpinBox->setEnabled(false);
        ui->afternoonCFSpinBox->setEnabled(false);
        ui->afternoonCRSpinBox->setEnabled(false);
        ui->afternoonBGSpinBox->setEnabled(false);
        emit profileUpdated(ui->afternoonBRSpinBox->value(), ui->afternoonCFSpinBox->value(), ui->afternoonCRSpinBox->value(), ui->afternoonBGSpinBox->value());
    }else{
        appendLog("Profile set to Night.");
        ui->nightBRSpinBox->setEnabled(false);
        ui->nightCFSpinBox->setEnabled(false);
        ui->nightCRSpinBox->setEnabled(false);
        ui->nightBGSpinBox->setEnabled(false);
        emit profileUpdated(ui->nightBRSpinBox->value(), ui->nightCFSpinBox->value(), ui->nightCRSpinBox->value(), ui->nightBGSpinBox->value());
    }
}

void MainWindow::onEditProfileClicked(){
    if(ui->morningProfileRadioButton->isChecked()){
        ui->morningBRSpinBox->setEnabled(true);
        ui->morningCFSpinBox->setEnabled(true);
        ui->morningCRSpinBox->setEnabled(true);
        ui->morningBGSpinBox->setEnabled(true);
    }else if(ui->afternoonProfileRadioButton->isChecked()){
        ui->afternoonBRSpinBox->setEnabled(true);
        ui->afternoonCFSpinBox->setEnabled(true);
        ui->afternoonCRSpinBox->setEnabled(true);
        ui->afternoonBGSpinBox->setEnabled(true);
    }else{
        ui->nightBRSpinBox->setEnabled(true);
        ui->nightCFSpinBox->setEnabled(true);
        ui->nightCRSpinBox->setEnabled(true);
        ui->nightBGSpinBox->setEnabled(true);
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

void MainWindow::updateIOB(double level, double hours) {
    ui->IOBLabel->setText(QString("Insulin On Board (IOB): %1 u | %2 hrs")
                           .arg(level, 0, 'f', 2)
                           .arg(hours, 0, 'f', 2));
}

void MainWindow::appendLog(const QString &msg) {
    ui->logOutput->appendPlainText(msg);
}

void MainWindow::onBatteryDepleted() {
    simulationTimer->stop();
    disableAllInput();
    ui->startButton->setEnabled(true);
    ui->chargeButton->setEnabled(true);
    ui->startButton->setText("Power On");
    appendLog("System powered off due to battery depletion.");
}
