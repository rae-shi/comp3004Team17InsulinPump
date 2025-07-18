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
    ui->refillCartridgeButton->setEnabled(true);
    ui->depleteBatteryButton->setEnabled(true);
    ui->depleteCartridgeButton->setEnabled(true);

    device->setupDevice();
    appendLog(QString("------------------"));
    appendLog("Profile defaulted to Morning.");
    emit profileUpdated(ui->morningBRSpinBox->value(), ui->morningCFSpinBox->value(), ui->morningCRSpinBox->value(), ui->morningBGSpinBox->value());

    initializeGraph();
    ui->logOutput->setVisible(false);
    ui->logFrame->setVisible(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

//helper functions

void MainWindow::connectAllSlots(){
    // Connections
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    connect(ui->submitProfileButton, &QPushButton::clicked, this, &MainWindow::onSubmitProfileClicked);
    connect(ui->editProfileButton, &QPushButton::clicked, this, &MainWindow::onEditProfileClicked);
    connect(ui->viewCalcButton, &QPushButton::clicked, this, &MainWindow::onCalculateBolus);
    connect(ui->checkHistory, &QPushButton::clicked, this, &MainWindow::checkHistory);


    // For battery and cartridge refill
    connect(ui->chargeButton, &QPushButton::clicked, this, &MainWindow::onChargeClicked);
    connect(ui->refillCartridgeButton, &QPushButton::clicked, this, &MainWindow::onRefillCartridgeClicked);

    // For battery and cartridge depletion (click functionality)
    connect(ui->depleteBatteryButton, &QPushButton::clicked, this, &MainWindow::onDepleteBatteryClicked);
    connect(ui->depleteCartridgeButton, &QPushButton::clicked, this, &MainWindow::onDepleteCartridgeClicked);

    connect(device, &Device::batteryLevelChanged, this, &MainWindow::updateBattery);
    connect(device, &Device::logEvent, this, &MainWindow::appendLog);
    connect(device, &Device::devicePoweredOff, this, &MainWindow::onBatteryDepleted);
    connect(this, &MainWindow::profileUpdated, device, &Device::applyProfile);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::glucoseChanged, this, &MainWindow::updateGlucose);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::IOBChanged, this, &MainWindow::updateIOB);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::cartChanged, this, &MainWindow::updateCart);
    connect(ui->pauseIns, &QPushButton::clicked, this, &MainWindow::onPauseInClicked);
    connect(ui->occlusion, &QPushButton::clicked, this, &MainWindow::onOcclusionClicked);

    connect(simulationTimer, &QTimer::timeout, device, &Device::runDevice);
    connect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::addPointy, this, &MainWindow::addPoint);
    connect(device, &Device::logError, this, &MainWindow::appendErrorLog);


}

void MainWindow::disconnectAllSlots(){
    disconnect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    disconnect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    disconnect(ui->submitProfileButton, &QPushButton::clicked, this, &MainWindow::onSubmitProfileClicked);
    disconnect(ui->editProfileButton, &QPushButton::clicked, this, &MainWindow::onEditProfileClicked);
    disconnect(ui->viewCalcButton, &QPushButton::clicked, this, &MainWindow::onCalculateBolus);
    disconnect(ui->pauseIns, &QPushButton::clicked, this, &MainWindow::onPauseInClicked);
    disconnect(ui->checkHistory, &QPushButton::clicked, this, &MainWindow::checkHistory);

    // For battery and cartridge refill
    disconnect(ui->chargeButton, &QPushButton::clicked, this, &MainWindow::onChargeClicked);
    disconnect(ui->refillCartridgeButton, &QPushButton::clicked, this, &MainWindow::onRefillCartridgeClicked);

    // For battery and cartridge depletion (click functionality)
    disconnect(ui->depleteBatteryButton, &QPushButton::clicked, this, &MainWindow::onDepleteBatteryClicked);
    disconnect(ui->depleteCartridgeButton, &QPushButton::clicked, this, &MainWindow::onDepleteCartridgeClicked);
    disconnect(ui->occlusion, &QPushButton::clicked, this, &MainWindow::onOcclusionClicked);

    disconnect(device, &Device::batteryLevelChanged, this, &MainWindow::updateBattery);
    disconnect(device, &Device::logEvent, this, &MainWindow::appendLog);
    disconnect(device, &Device::logError, this, &MainWindow::appendErrorLog);
    disconnect(device, &Device::devicePoweredOff, this, &MainWindow::onBatteryDepleted);
    disconnect(this, &MainWindow::profileUpdated, device, &Device::applyProfile);
    disconnect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::glucoseChanged, this, &MainWindow::updateGlucose);
    disconnect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::IOBChanged, this, &MainWindow::updateIOB);
    disconnect(device->findChild<InsulinControlSystem*>(), &InsulinControlSystem::addPointy, this, &MainWindow::addPoint);


    disconnect(simulationTimer, &QTimer::timeout, device, &Device::runDevice);
}

void MainWindow::enableAllInput(){
    //buttons
    ui->chargeButton->setEnabled(true);
    ui->disconnectButton->setEnabled(true);
    ui->occlusion->setEnabled(true);
    ui->startButton->setEnabled(true);
    ui->editProfileButton->setEnabled(true);
    ui->submitProfileButton->setEnabled(true);
    ui->viewCalcButton->setEnabled(true);
    ui->refillCartridgeButton->setEnabled(true);
    ui->depleteBatteryButton->setEnabled(true);
    ui->depleteCartridgeButton->setEnabled(true);
    ui->extendedDurationHourSpinBox->setValue(3);
    ui->extendedDurationMinSpinBox->setValue(0);
    ui->pauseIns->setEnabled(true);

    //radio buttons
    ui->morningProfileRadioButton->setEnabled(true);
    ui->afternoonProfileRadioButton->setEnabled(true);
    ui->nightProfileRadioButton->setEnabled(true);

    //spin boxes
    ui->carbsInputSpinBox->setEnabled(true);
    ui->glucoseInputSpinBox->setEnabled(true);
    ui->extendedDurationHourSpinBox->setEnabled(true);
    ui->extendedDurationMinSpinBox->setEnabled(true);
    //leaving personal profile spin boxes out because counterproductive
}

void MainWindow::disableAllInput(){
    //buttons
    ui->chargeButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
    ui->occlusion->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->editProfileButton->setEnabled(false);
    ui->submitProfileButton->setEnabled(false);
    ui->viewCalcButton->setEnabled(false);
    ui->refillCartridgeButton->setEnabled(false);
    ui->depleteBatteryButton->setEnabled(false);
    ui->depleteCartridgeButton->setEnabled(false);
    ui->pauseIns->setEnabled(false);

    //radio buttons
    ui->morningProfileRadioButton->setEnabled(false);
    ui->afternoonProfileRadioButton->setEnabled(false);
    ui->nightProfileRadioButton->setEnabled(false);

    //spin boxes
    ui->carbsInputSpinBox->setEnabled(false);
    ui->glucoseInputSpinBox->setEnabled(false);
    ui->extendedDurationHourSpinBox->setEnabled(false);
    ui->extendedDurationMinSpinBox->setEnabled(false);
    //leaving personal profile spin boxes out because counterproductive
}

//slots

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

void MainWindow::onChargeClicked(){
    if(ui->batteryBar->value()!=100){
        device->chargeBattery();
        // Enable the start button after charging
        ui->startButton->setEnabled(true);
        appendLog("Battery charged. Device can now be powered on.");
    }
}

void MainWindow::onDisconnectClicked(){
    if (ui->disconnectButton->text() == "Disconnect Device"){
        simulationTimer->stop();
        device->stopDevice();
        appendLog("Power off.");
        disableAllInput();
        ui->disconnectButton->setEnabled(true);
        ui->disconnectButton->setText("Reconnect Device");
        appendErrorLog("Device disconnected, reconnect device to user.");
        appendLog("Device disconnected, reconnect device to user.");
    } else if (ui->disconnectButton->text() == "Reconnect Device"){
        device->startDevice();
        simulationTimer->start(1000); // 1 second = 1 time step
        appendLog("Power on.");
        enableAllInput();
        ui->disconnectButton->setText("Disconnect Device");
        appendErrorLog("Device reconnected to user.");
        appendLog("Device reconnected to user.");

    }

}


void MainWindow::onOcclusionClicked(){
    if (ui->occlusion->text() == "Cause Occlusion"){
        simulationTimer->stop();
        device->stopDevice();
        appendLog("Power off.");
        disableAllInput();
        ui->occlusion->setEnabled(true);
        ui->occlusion->setText("Resolve Occlusion");
        appendErrorLog("Occlusion occured, check infusion site for blockages.");
        appendLog("Occlusion occured, check infusion site for blockages.");
    } else if (ui->occlusion->text() == "Resolve Occlusion"){
        device->startDevice();
        simulationTimer->start(1000); // 1 second = 1 time step
        appendLog("Power on.");
        enableAllInput();
        ui->occlusion->setText("Cause Occlusion");
        appendErrorLog("Occlusion resolved, infusion site has no blockages.");
        appendLog("Occlusion resolved, infusion site has no blockages.");

    }
}

void MainWindow::onSubmitProfileClicked(){
    appendLog(QString("------------------"));
    ui->morningProfileRadioButton->setEnabled(true);
    ui->afternoonProfileRadioButton->setEnabled(true);
    ui->nightProfileRadioButton->setEnabled(true);
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
    ui->morningProfileRadioButton->setEnabled(false);
    ui->afternoonProfileRadioButton->setEnabled(false);
    ui->nightProfileRadioButton->setEnabled(false);
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

void MainWindow::appendErrorLog(const QString &msg) {
    ui->errorLog->clear();
    ui->errorLog->appendPlainText(msg);
}


void MainWindow::onBatteryDepleted() {
    simulationTimer->stop();
    disableAllInput();
    // Only enable the charge button, not the start button
    ui->chargeButton->setEnabled(true);
    ui->startButton->setEnabled(false); // Disable power button when battery is dead
    ui->startButton->setText("Power On");
    appendLog("System powered off due to battery depletion.");
    appendLog("Please charge the battery to power on the device.");
    appendErrorLog("System powered off due to battery depletion.\nPlease charge the battery to power on the device.");
}

void MainWindow::onCalculateBolus() {
    double carbInput = ui->carbsInputSpinBox->value();
    double glucoseInput = ui->glucoseInputSpinBox->value();

    int bolusDurationHour = ui->extendedDurationHourSpinBox->value();
    int bolusDurationMin = ui->extendedDurationMinSpinBox->value();

    device->findChild<InsulinControlSystem*>()->calculateBolus(carbInput, glucoseInput, bolusDurationHour, bolusDurationMin);

}

void MainWindow::onRefillCartridgeClicked() {
    device->refillCartridge();
    appendLog("Cartridge refilled to 300 units.");
}

void MainWindow::onDepleteBatteryClicked() {
    // Call the existing decrement function directly
    appendLog("Battery depleted.");
    appendErrorLog("Battery depleted.");
    decrementBattery();
}

void MainWindow::onDepleteCartridgeClicked() {
    // Call the existing decrement function directly
    decrementCartridge();
}

void MainWindow::decrementBattery() {
    // Get current battery level
    int currentLevel = ui->batteryBar->value();

    // Two-step depletion: above low -> low -> empty
    if (currentLevel > 10) {
        // If above low threshold, set to low
        currentLevel = 10;
    } else {
        // If already at or below low threshold, set to empty
        currentLevel = 0;
    }

    // Update battery level
    device->setBatteryLevel(currentLevel);
    ui->batteryBar->setValue(currentLevel);
}

void MainWindow::decrementCartridge() {
    // Get current cartridge level from UI
    double currentLevel = ui->cartridge->value();

    // Two-step depletion: above low -> low -> empty
    if (currentLevel > 30.0) {
        // If above low threshold, set to low
        currentLevel = 30.0;
    } else {
        // If already at or below low threshold, set to empty
        currentLevel = 0.0;
    }

    // Update cartridge level
    InsulinControlSystem *ics = device->findChild<InsulinControlSystem*>();
    if (ics) {
        // Set the cartridge to the new level directly, rather than depleting by units
        double amountToReduce = ics->getCartridgeLevel() - currentLevel;
        ics->depleteCartridge(amountToReduce); // This will emit signals to update UI
    } else {
        // Fallback in case ICS not found
        ui->cartridge->setValue(currentLevel);
        ui->cartridge->setFormat(QString("Cartridge: %1 u").arg(currentLevel, 0, 'f', 2));
    }
}

void MainWindow::initializeGraph(){
    // Set graph up
    series = new QLineSeries();
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);


    // Set x-axis info
    QValueAxis *xAxis = new QValueAxis;
    xAxis->setLabelFormat("%.0f");
    xAxis->setLabelsBrush(Qt::transparent);
    // Set graph to only show hour of glucose levels
    xAxis->setRange(0, 60);
    xAxis->setTitleText("Time (m)");

    // Sset y-axis info
    QValueAxis *yAxis = new QValueAxis;
    yAxis->setLabelFormat("%.2f");
    yAxis->setRange(3.0, 8.0);
    yAxis->setTitleText("Glucose (mmol/L)");
    yAxis->setTickCount(6);

    // Adjust graph display
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignRight);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);


    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(ui->horizontalFrame->size());
    chartView->setParent(ui->horizontalFrame);

}

void MainWindow::addPoint(int x, double y){
    series->append(x, y);

   // Get the current x-axis range
   QValueAxis *xAxis = qobject_cast<QValueAxis *>(chart->axes(Qt::Horizontal).first());

   if (x > xAxis->max()) {  // If new point exceeds the max range
       xAxis->setRange(xAxis->min() + 1, x);  // Shift the range dynamically
   }

   chartView->repaint();
   ui->timeStep->display(QString::number(x));
}

void MainWindow::checkHistory(){

    if (ui->checkHistory->text() == "Check History Logs"){
        ui->logOutput->setVisible(true);
        ui->logFrame->setVisible(true);
        ui->checkHistory->setText("Close History Logs");
    } else if (ui->checkHistory->text() == "Close History Logs"){
        ui->logOutput->setVisible(false);
        ui->logFrame->setVisible(false);
        ui->checkHistory->setText("Check History Logs");
    }

}
