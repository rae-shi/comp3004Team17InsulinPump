#include "insulinpump.h"
#include <QTimer>

// -------------------- Device Class --------------------
Device::Device(QObject *parent)
    : QObject(parent), batteryLevel(100), timeStep(0), isRunning(false) {
    ics = new InsulinControlSystem(this);
    logger = new Logger(this);

    connect(ics, SIGNAL(insulinDelivered(double)), this, SIGNAL(insulinInjected(double)));
    connect(ics, SIGNAL(logEvent(QString)), this, SIGNAL(logEvent(QString)));
    connect(ics, SIGNAL(logError(QString)), this, SIGNAL(logError(QString)));
}

void Device::setupDevice() {
    emit logEvent(QString("------------------"));
    ics->setBasalRate(1.0);
    ics->setCurrentGlucose(5.5); // mmol/L baseline
    ics->setState(InsulinControlSystem::Run);
    emit logEvent("Device setup complete.");
}

void Device::startDevice() {
    emit logEvent(QString("------------------"));
    isRunning = true;
    ics->setState(InsulinControlSystem::Run);
    emit logEvent("Device started.");
}

void Device::runDevice() {
    if (!isRunning) return;
    emit logEvent(QString("------------------"));

    timeStep++;
    emit logEvent(QString("Time Step: %1").arg(timeStep));
    ics->setTimeStep(timeStep);
    ics->updateInsulin();

    if (timeStep % 3 == 0) {
        batteryLevel -= 1;
        if (batteryLevel <= 0) {
            batteryLevel = 0;
            stopDevice();
            emit logEvent("Device automatically stopped due to depleted battery.");
            emit logError("Device automatically stopped due to depleted battery.");
            emit devicePoweredOff();
        }
        emit batteryLevelChanged(batteryLevel); // This will trigger setBatteryLevel indirectly via UI
    }
}

void Device::applyProfile(double pbasalRate, double correctionFactor, int carbRatio, double targetGlucose) {
    if (ics) {
        ics->setProfileBasalRate(pbasalRate);
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
    emit logError("Device stopped.");
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
    emit logError("Battery depleted.");
}

void Device::setBatteryLevel(int level) {
    batteryLevel = qBound(0, level, 100);
    if (batteryLevel == 10) {
        emit logEvent("Insulin cartridge depleted.\nWARNING: Battery level low (10%).");
        emit logError("Insulin cartridge depleted.\nWARNING: Battery level low (10%).");

    }
    emit batteryLevelChanged(batteryLevel);
}

int Device::getBatteryLevel() const {
    return batteryLevel;
}

double InsulinControlSystem::getCartridgeLevel() const {
    return cartLevel;
}

void Device::refillCartridge() {
    if (ics) {
        ics->refillCartridge();
    }
}

// -------------------- InsulinControlSystem --------------------
InsulinControlSystem::InsulinControlSystem(QObject *parent)
    : QObject(parent), basalRate(1.0), correctionFactor(1.0), carbRatio(1), targetGlucose(5.0), currentGlucose(5.5), insulinOnBoard(0.0), cartLevel(300.00), currentState(Run), profileBasalRate(0.0){}

void InsulinControlSystem::setState(State state) {
    currentState = state;
    emit logEvent("State changed.");
}

void InsulinControlSystem::setBasalRate(double rate) {
    basalRate = rate;
    emit logEvent(QString("Basal rate set to %1").arg(rate));
}

void InsulinControlSystem::setProfileBasalRate(double rate) {
    profileBasalRate = rate;
    emit logEvent(QString("Profile basal rate set to %1").arg(rate));
}

double InsulinControlSystem::getCorrectionFactor() const {
    return correctionFactor;
}

void InsulinControlSystem::setCorrectionFactor(double factor){
    correctionFactor = factor;
    emit logEvent(QString("Correction Factor set to %1").arg(factor));
}

double InsulinControlSystem::getCarbRatio() const {
    return carbRatio;
}

void InsulinControlSystem::setCarbRatio(int carb){
    carbRatio = carb;
    emit logEvent(QString("Carb Ratio set to %1").arg(carb));
}

double InsulinControlSystem::getTargetGlucose() const{
    return targetGlucose;
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
    double basalEffect = 0;

    if (currentState == Stop) return;
    if (currentState == Pause){
        setBasalRate(0);
    } else if (currentState == Resume){
        currentState = Run;
        setBasalRate(profileBasalRate);
    }

    // Adjust active basal rate based on profile basal rate
    if (currentState != Pause){
        // Simulate effect of basal insulin delivery
        // Calculate basal effect as 1/10th basal rate for simultation reasons
        basalEffect = basalRate * 0.1;
        insulinOnBoard += basalEffect;

        currentGlucose -= 0.1 * basalRate;
    } else if (currentState == Pause){
        // Increase the blood glucose since insulin stops
        currentGlucose += 0.05;
    }

    // Add random fluctuations to prevent stabilization
    double noiseFactor = QRandomGenerator::global()->generateDouble() * 0.2 - 0.1;
    currentGlucose += noiseFactor;


    // Ensure values remain stable and avoid floating-point errors
    // problematic logic - may keep numbers stuck
    currentGlucose = qRound(currentGlucose * 100) / 100.0;
    cartLevel -= basalEffect;
    basalEffect = qRound(basalEffect * 100) / 100.0;

    // insulin is absorbed 2% per minute
    insulinOnBoard *= 0.98;

    // calculate hours remaining for IOB
    double remainingTimeHours;

    // remaining time for consuming insulin on board
    if (insulinOnBoard > 0.01){
        double remainingTimeMinutes = log(100 / insulinOnBoard) / log(1.02); // Using a decay formula
        remainingTimeHours= remainingTimeMinutes / 60.0;  // Convert minutes to hours
    } else{
        insulinOnBoard = 0.0;
        remainingTimeHours = 0.0;
    }

    // Predict glucose trend 30 minutes ahead
    double predictedGlu = currentGlucose - (insulinOnBoard * 0.1667);
    // Small random fluctuation
    predictedGlu += QRandomGenerator::global()->generateDouble() * 0.2 - 0.1;
    predictedGlu = qRound(predictedGlu * 100) / 100.0;

    // Adjust insulin delivery based on Control-IQ technology rules
    if (predictedGlu <= targetGlucose-0.1) {
        basalRate = 0.0;  // Suspend insulin if glucose is too low
    } else if (predictedGlu <= targetGlucose+0.03) {
        basalRate = qMax(basalRate * 0.5, 0.1);  // Reduce basal insulin when predict glucose is in the range of target glucose
    } else if (predictedGlu >= targetGlucose+0.5) {
        double maxBasalRate = 2.0;
        basalRate = qMin(basalRate * 1.2, maxBasalRate);  // Increase insulin
    }

    if (currentGlucose < 3.9){
        emit logEvent("User is hypoglycemic");
        emit logError("User is hypoglycemic");
    } else if(currentGlucose >= 8.9){
        emit logEvent("User is hyperglycemic");
        emit logError("User is hyperglycemic");
    }

    emit addPointy(timeStep,currentGlucose);
    // Emit updated values - gui dependent - change as needed
    emit IOBChanged(insulinOnBoard, remainingTimeHours);
    emit insulinDelivered(basalEffect);
    emit glucoseChanged(currentGlucose);
    emit cartChanged(cartLevel);
    emit logEvent(QString("Basal insulin delivered: %1 | Glucose: %2 | Predicted: %3")
                  .arg(basalEffect).arg(currentGlucose).arg(predictedGlu));
}

void InsulinControlSystem::calculateBolus(double carbInput, double glucoseInput, double bolusDurationHour, double bolusDurationMin) {
    // Overwrite blood glucose using user input
    setCurrentGlucose(glucoseInput);

    double bolusDuration = bolusDurationHour + (bolusDurationMin / 60.0);

    // Bolus Calculation Logic
    double carbBolus = carbInput / carbRatio;
    double correctionBolus = glucoseInput > targetGlucose ? (glucoseInput - targetGlucose) / correctionFactor: 0;
    double totalBolus = carbBolus + correctionBolus;
    double finalBolus = totalBolus > insulinOnBoard ? totalBolus - insulinOnBoard : 0;
    double correctionPortion = correctionBolus > insulinOnBoard ? correctionBolus - insulinOnBoard : 0;

    emit logEvent(QString("Carb Value: %1, Carb Ratio: %2, Glucose Input: %3, TargetBGL %4, Correction Factor: %5, IOB: %6 | "
                          "Total Bolus: %7, Final Bolus: %8, Correction Portion: %9")
                          .arg(carbInput).arg(carbRatio).arg(glucoseInput).arg(targetGlucose).arg(correctionFactor)
                          .arg(insulinOnBoard).arg(totalBolus).arg(finalBolus).arg(correctionPortion));

    // Immediate and Extended Bolus (60% Immediate, 40% Extended over duration)
    double immediateFraction = 0.6;
    double immediateBolus = immediateFraction * finalBolus;
    double extendedBolus = (1 - immediateFraction) * finalBolus;
    double bolusPerHour = extendedBolus / bolusDuration;

    // Only apply correction bolus on glucose
    double immediateCorrection = 0.6 * correctionPortion;
    double correctionPerHour = (correctionPortion - immediateCorrection) / bolusDuration;

    simulateBolus(immediateBolus, immediateCorrection);
    scheduleExtendedBolus(bolusPerHour, correctionPerHour, bolusDuration);

    emit logEvent(QString("Immediate Bolus: %1 units | Extended: %2 units over 3 hrs")
                  .arg(immediateBolus, 0, 'f', 2)
                  .arg(extendedBolus, 0, 'f', 2));

}

void InsulinControlSystem::simulateBolus(double bolus, double correctionOnly) {
    // Simulate bolus effect
    insulinOnBoard += bolus;

    double glucoseDrop = correctionOnly * correctionFactor;
    // Apply only the correction effect on glucose
    currentGlucose = currentGlucose > glucoseDrop
                         ? currentGlucose - glucoseDrop
                         : 0;
    depleteCartridge(bolus);

    emit glucoseChanged(currentGlucose);
    emit logEvent(QString("Bolus injected: %1 | Glucose: %2").arg(bolus).arg(currentGlucose));
}

void InsulinControlSystem::scheduleExtendedBolus(double bolusPerHour, double correctioPerHour, int hours) {
    QTimer* timer = new QTimer(this);
    int* deliveryCount = new int(0);  // use heap to persist in lambda

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if(currentState  == Pause) {
            return;
        }

        if (*deliveryCount >= hours) {
            timer->stop();
            timer->deleteLater();
            delete deliveryCount;
        } else {
            simulateBolus(bolusPerHour, correctioPerHour);
            (*deliveryCount)++;
        }
    });

    timer->start(3600000); // every 1 hour
}

void InsulinControlSystem::refillCartridge() {
    cartLevel = 300.0;
    emit cartChanged(cartLevel);
}

void InsulinControlSystem::depleteCartridge(double amount) {
    cartLevel = qMax(0.0, cartLevel - amount);
    emit cartChanged(cartLevel);
    
    // Check for low insulin warning threshold (30 units)
    if (cartLevel == 30.0) {
        emit logEvent("WARNING: Insulin level low (30 units).");
        emit logError("WARNING: Insulin level low (30 units).");
    }
    
    if (cartLevel == 0) {
        emit logEvent("Cartridge is empty.");
        emit logError("Cartridge is empty.");
    }
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
