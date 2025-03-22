#include "insulinpump.h"
#include <QDebug>
#include <QString>

// TODO-1: the updateInsulin logic not connected to the mainwindow.cpp and there is no GUI component for showing insulin now
// it should be shown at the top right corner

//TODO-2: the battery logic also not correctly set up in the mainwindow.cpp. Also no GUI component yet. It should be shown at the
// top left corner

//TODO-3: similar issue for the insulin on board.

// -------------------- Device Class --------------------
Device::Device(QObject *parent)
    : QObject(parent), batteryLevel(100), timeStep(0), isRunning(false) {
    ics = new InsulinControlSystem(this);
    logger = new Logger(this);

    connect(ics, SIGNAL(insulinDelivered(double)), this, SIGNAL(insulinInjected(double)));
    connect(ics, SIGNAL(logEvent(QString)), this, SIGNAL(logEvent(QString)));
}

void Device::setupDevice() {
    ics->setBasalRate(1.0);
    ics->setCurrentGlucose(5.5); // mmol/L baseline
    ics->setState(InsulinControlSystem::Start);
    emit logEvent("Device setup complete.");
}

void Device::startDevice() {
    isRunning = true;
    ics->setState(InsulinControlSystem::Start);
    emit logEvent("Device started.");
}

void Device::runDevice() {
    if (!isRunning) return;

    timeStep++;
    ics->updateInsulin();

    // Battery discharge rate to 1% every 3 steps
    if (timeStep % 3 == 0) {
        batteryLevel -= 1;

        // Log low battery warning at 10%
        if (batteryLevel == 10) {
            emit logEvent("WARNING: Battery level low (10%).");
        }

        // Stop device when battery is depleted
        if (batteryLevel <= 0) {
            batteryLevel = 0; // Ensure we don't go below 0
            stopDevice();
            emit logEvent("Device automatically stopped due to depleted battery.");
        }

        emit batteryLevelChanged(batteryLevel);
    }
}

void Device::stopDevice() {
    isRunning = false;
    ics->setState(InsulinControlSystem::Stop);
    emit logEvent("Device stopped.");
}

void Device::chargeBattery() {
    batteryLevel = 100;
    emit batteryLevelChanged(batteryLevel);
    emit logEvent("Battery charged to 100%.");
}

void Device::depleteBattery() {
    batteryLevel = 0;
    emit batteryLevelChanged(batteryLevel);
    emit logEvent("Battery depleted.");
}

int Device::getBatteryLevel() const {
    return batteryLevel;
}

// -------------------- InsulinControlSystem --------------------
InsulinControlSystem::InsulinControlSystem(QObject *parent)
    : QObject(parent), basalRate(1.0), currentGlucose(5.5), insulinOnBoard(0.0), currentState(Stop) {}

void InsulinControlSystem::setState(State state) {
    currentState = state;
    emit logEvent("State changed.");
}

void InsulinControlSystem::setBasalRate(double rate) {
    basalRate = rate;
    emit logEvent(QString("Basal rate set to %1").arg(rate));
}

void InsulinControlSystem::setCurrentGlucose(double level) {
    currentGlucose = level;
    emit glucoseChanged(currentGlucose);
}

double InsulinControlSystem::getInsulinOnBoard() const {
    return insulinOnBoard;
}

void InsulinControlSystem::updateInsulin() {
    if (currentState != Start) return;
    // Simulate effect of basal insulin delivery
    insulinOnBoard += basalRate * 0.1;
    currentGlucose -= 0.1 * basalRate;
    if (currentGlucose < 4.0) currentGlucose = 4.0;

    emit insulinDelivered(basalRate * 0.1);
    emit glucoseChanged(currentGlucose);
    emit logEvent(QString("Basal insulin delivered: %1 | Glucose: %2").arg(basalRate * 0.1).arg(currentGlucose));
}

void InsulinControlSystem::simulateBolus(double glucoseLevel) {
    double bolus = calculateBolus(glucoseLevel);
    insulinOnBoard += bolus;
    currentGlucose -= bolus * 0.3;      // drop effect
    if (currentGlucose < 4.0) currentGlucose = 4.0;

    emit bolusInjected(bolus);
    emit glucoseChanged(currentGlucose);
    emit logEvent(QString("Bolus injected: %1 | Glucose: %2").arg(bolus).arg(currentGlucose));
}

double InsulinControlSystem::calculateBolus(double glucose) {
    return (glucose > 7.0 ? (glucose - 7.0) * 0.5 : 0);
}

// -------------------- Logger --------------------
Logger::Logger(QObject *parent) : QObject(parent) {}

void Logger::logBasalRate(double rate) {
    eventLog.append(QString("Basal rate: %1").arg(rate));
}

void Logger::logBolus(double amount) {
    eventLog.append(QString("Bolus: %1").arg(amount));
}

void Logger::printToConsole() {
    for (const QString &event : eventLog) {
        qDebug() << event;
    }
}
