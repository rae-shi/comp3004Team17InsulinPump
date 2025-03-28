#include "insulinpump.h"

// -------------------- Device Class --------------------
Device::Device(QObject *parent)
    : QObject(parent), batteryLevel(100), timeStep(0), isRunning(false) {
    ics = new InsulinControlSystem(this);
    logger = new Logger(this);

    connect(ics, SIGNAL(insulinDelivered(double)), this, SIGNAL(insulinInjected(double)));
    connect(ics, SIGNAL(logEvent(QString)), this, SIGNAL(logEvent(QString)));
}

void Device::setupDevice() {
    emit logEvent(QString("------------------"));
    ics->setBasalRate(1.0);
    ics->setCurrentGlucose(5.5); // mmol/L baseline
    ics->setState(InsulinControlSystem::Start);
    emit logEvent("Device setup complete.");
}

void Device::startDevice() {
    emit logEvent(QString("------------------"));
    isRunning = true;
    ics->setState(InsulinControlSystem::Start);
    emit logEvent("Device started.");
}

void Device::runDevice() {
    if (!isRunning) return;
    emit logEvent(QString("------------------"));

    timeStep++;
    emit logEvent(QString("Time Step: %1").arg(timeStep));
    ics->setTimeStep(timeStep);
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

void Device::applyProfile(double basalRate, double correctionFactor, int carbRatio, double targetGlucose) {
    if (ics) {
        ics->setBasalRate(basalRate);
        ics->setCorrectionFactor(correctionFactor);
        ics->setCarbRatio(carbRatio);
        ics->setTargetGlucose(targetGlucose);
    }
}

void Device::stopDevice() {
    isRunning = false;
    ics->setState(InsulinControlSystem::Stop);
    emit logEvent(QString("------------------"));
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
    : QObject(parent), basalRate(1.0), correctionFactor(1.0), carbRatio(1), targetGlucose(5.0), currentGlucose(5.5), insulinOnBoard(0.0), currentState(Stop), cartLevel(300.00) {}

void InsulinControlSystem::setState(State state) {
    currentState = state;
    emit logEvent("State changed.");
}

void InsulinControlSystem::setBasalRate(double rate) {
    basalRate = rate;
    emit logEvent(QString("Basal rate set to %1").arg(rate));
}

void InsulinControlSystem::setCorrectionFactor(double factor){
    correctionFactor = factor;
    emit logEvent(QString("Correction Factor set to %1").arg(factor));
}

void InsulinControlSystem::setCarbRatio(int carb){
    carbRatio = carb;
    emit logEvent(QString("Carb Ratio set to %1").arg(carb));
}

void InsulinControlSystem::setTargetGlucose(double level){
    targetGlucose = level;
    emit logEvent(QString("Target Glucose set to %1").arg(level));
}

void InsulinControlSystem::setCurrentGlucose(double level) {
    currentGlucose = level;
    emit glucoseChanged(currentGlucose);
}

// for anything time related, ics has an udpated timestep
void InsulinControlSystem:: setTimeStep(int ts){
    timeStep = ts;
}

double InsulinControlSystem::getInsulinOnBoard() const {
    return insulinOnBoard;
}


void InsulinControlSystem::updateInsulin() {
    // by ICS logic, each time step is a minute
    if (currentState != Start) return;

    // Simulate effect of basal insulin delivery
    double basalEffect = basalRate * 0.1;
    insulinOnBoard += basalEffect;
    currentGlucose -= 0.1 * basalRate;

    // Add random fluctuations to prevent stabilization
    // Range: -0.15 to 0.15
    double noiseFactor = QRandomGenerator::global()->generateDouble() * 0.3 - 0.15;
    currentGlucose += noiseFactor;


    // Ensure values remain stable and avoid floating-point errors
    insulinOnBoard = qRound(insulinOnBoard * 100) / 100.0;
    currentGlucose = qRound(currentGlucose * 100) / 100.0;
    cartLevel -= basalEffect;
    basalEffect = qRound(basalEffect * 100) / 100.0;

    // insulin is absorbed 2% per minute
    insulinOnBoard *= 0.98;

    // calculate hours remaining for IOB
    double remainingTimeMinutes = log(100 / insulinOnBoard) / log(1.02); // Using a decay formula
    double remainingTimeHours = remainingTimeMinutes / 60.0;  // Convert minutes to hours

    // Predict glucose trend 30 minutes ahead
    double predictedGlu = currentGlucose - (insulinOnBoard * 0.5);
    // Small random fluctuation
    predictedGlu += QRandomGenerator::global()->generateDouble() * 0.2 - 0.1;
    predictedGlu = qRound(predictedGlu * 100) / 100.0;

    // Adjust insulin delivery based on Control-IQ technology rules
    if (predictedGlu <= 3.9) {
        basalRate = 0.0;  // Suspend insulin if glucose is too low
    } else if (predictedGlu <= 6.25) {
        basalRate = qMax(basalRate * 0.5, 0.1);  // Reduce insulin
    } else if (predictedGlu >= 8.9) {
        double maxBasalRate = 2.0;
        basalRate = qMin(basalRate * 1.2, maxBasalRate);  // Increase insulin
    }
    // If lowest glucose is min 4.0
    if (currentGlucose < 4.0) currentGlucose = 4.0;

    // Emit updated values - gui dependent - change as needed
    emit IOBChanged(insulinOnBoard, remainingTimeHours);
    emit insulinDelivered(basalEffect);
    emit glucoseChanged(currentGlucose);
    emit cartChanged(cartLevel);
    emit logEvent(QString("Basal insulin delivered: %1 | Glucose: %2 | Predicted: %3")
                  .arg(basalEffect).arg(currentGlucose).arg(predictedGlu));
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
    return (glucose > targetGlucose ? (glucose - targetGlucose)/correctionFactor : 0); //calculate bolus based off of correction factor and target glucose
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
