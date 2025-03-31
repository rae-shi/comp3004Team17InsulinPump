#include <QtTest/QtTest>
#include <QDebug>
#include <QtGlobal>
#include <cmath>
#include "insulinpump.h"

class InsulinPumpTest : public QObject {
    Q_OBJECT

private slots:
    // Device tests
    void testDeviceInitialization();
    void testBatteryManagement();
    void testDeviceStartStop();
    void testCartridgeOperations();

    // InsulinControlSystem tests
    void testBasalRateSettings();
    void testGlucoseManagement();
    void testInsulinBolus();
    void testProfiles();
    void testInsulinOnBoard();
    void testCartridgeLevel();
};

// Device tests implementation
void InsulinPumpTest::testDeviceInitialization() {
    qDebug() << "=== TEST: Device Initialization ===";
    Device device;
    
    bool batteryLevelCorrect = device.getBatteryLevel() == 100;
    if (batteryLevelCorrect) {
        qDebug() << "Device initializes with 100% battery";
    } else {
        qDebug() << "FAIL: Device battery level is" << device.getBatteryLevel() << "expected 100";
    }
    QVERIFY2(batteryLevelCorrect, "Initial battery level should be 100%");
}

void InsulinPumpTest::testBatteryManagement() {
    qDebug() << "=== TEST: Battery Management ===";
    Device device;
    
    // Test battery depletion
    device.depleteBattery();
    bool depletionWorks = device.getBatteryLevel() == 0;
    if (depletionWorks) {
        qDebug() << "Battery depletion function works";
    } else {
        qDebug() << "FAIL: Battery level after depletion is" << device.getBatteryLevel() << "expected 0";
    }
    QVERIFY2(depletionWorks, "Battery level should be 0 after depletion");
    
    // Test battery charging
    device.chargeBattery();
    bool chargingWorks = device.getBatteryLevel() == 100;
    if (chargingWorks) {
        qDebug() << "Battery charging function works";
    } else {
        qDebug() << "FAIL: Battery level after charging is" << device.getBatteryLevel() << "expected 100";
    }
    QVERIFY2(chargingWorks, "Battery level should be 100 after charging");
    
    // Test battery level setting
    device.setBatteryLevel(50);
    bool levelSettingWorks = device.getBatteryLevel() == 50;
    if (levelSettingWorks) {
        qDebug() << "Setting battery level works";
    } else {
        qDebug() << "FAIL: Battery level after setting to 50 is" << device.getBatteryLevel();
    }
    QVERIFY2(levelSettingWorks, "Battery level should be 50 after setting");
    
    // Test battery level upper bound
    device.setBatteryLevel(150);
    bool upperBoundWorks = device.getBatteryLevel() == 100;
    if (upperBoundWorks) {
        qDebug() << "Battery level upper bound check works";
    } else {
        qDebug() << "FAIL: Battery level after setting to 150 is" << device.getBatteryLevel() << "expected 100";
    }
    QVERIFY2(upperBoundWorks, "Battery level should be bounded to 100");
    
    // Test battery level lower bound
    device.setBatteryLevel(-10);
    bool lowerBoundWorks = device.getBatteryLevel() == 0;
    if (lowerBoundWorks) {
        qDebug() << "Battery level lower bound check works";
    } else {
        qDebug() << "FAIL: Battery level after setting to -10 is" << device.getBatteryLevel() << "expected 0";
    }
    QVERIFY2(lowerBoundWorks, "Battery level should be bounded to 0");
}

void InsulinPumpTest::testDeviceStartStop() {
    qDebug() << "=== TEST: Device Start/Stop ===";
    Device device;
    
    // Setup device - no direct verification, check for exceptions
    try {
        device.setupDevice();
        qDebug() << "Device can be setup";
    } catch (...) {
        qDebug() << "FAIL: Exception during device setup";
        QFAIL("Exception occurred during device setup");
    }
    
    // Start device - no direct verification, check for exceptions
    try {
        device.startDevice();
        qDebug() << "Device can be started";
    } catch (...) {
        qDebug() << "FAIL: Exception during device start";
        QFAIL("Exception occurred during device start");
    }
    
    // Run device - no direct verification, check for exceptions
    try {
        device.runDevice();
        qDebug() << "Device can run for a cycle";
    } catch (...) {
        qDebug() << "FAIL: Exception during device run cycle";
        QFAIL("Exception occurred during device run cycle");
    }
    
    // Stop device - no direct verification, check for exceptions
    try {
        device.stopDevice();
        qDebug() << "Device can be stopped";
    } catch (...) {
        qDebug() << "FAIL: Exception during device stop";
        QFAIL("Exception occurred during device stop");
    }
}

void InsulinPumpTest::testCartridgeOperations() {
    qDebug() << "=== TEST: Cartridge Operations ===";
    Device device;
    
    // Test cartridge refill - no direct verification, check for exceptions
    try {
        device.refillCartridge();
        qDebug() << "Cartridge can be refilled";
    } catch (...) {
        qDebug() << "FAIL: Exception during cartridge refill";
        QFAIL("Exception occurred during cartridge refill");
    }
}

// InsulinControlSystem tests implementation
void InsulinPumpTest::testBasalRateSettings() {
    qDebug() << "=== TEST: Basal Rate Settings ===";
    InsulinControlSystem ics;
    
    // Test basal rate setting - no direct verification, check for exceptions
    try {
        ics.setBasalRate(1.5);
        qDebug() << "Basal rate can be set";
    } catch (...) {
        qDebug() << "FAIL: Exception when setting basal rate";
        QFAIL("Exception occurred when setting basal rate");
    }
    
    // Test profile basal rate setting - no direct verification, check for exceptions
    try {
        ics.setProfileBasalRate(2.0);
        qDebug() << "Profile basal rate can be set";
    } catch (...) {
        qDebug() << "FAIL: Exception when setting profile basal rate";
        QFAIL("Exception occurred when setting profile basal rate");
    }
}

void InsulinPumpTest::testGlucoseManagement() {
    qDebug() << "=== TEST: Glucose Management ===";
    InsulinControlSystem ics;
    
    // Test target glucose setting
    double targetGlucose = 5.5;
    ics.setTargetGlucose(targetGlucose);
    bool targetGlucoseCorrect = ics.getTargetGlucose() == targetGlucose;
    if (targetGlucoseCorrect) {
        qDebug() << "Target glucose can be set";
    } else {
        qDebug() << "FAIL: Target glucose is" << ics.getTargetGlucose() << "expected" << targetGlucose;
    }
    QVERIFY2(targetGlucoseCorrect, "Target glucose should match the set value");
    
    // Test current glucose setting - no direct verification, check for exceptions
    try {
        double currentGlucose = 6.0;
        ics.setCurrentGlucose(currentGlucose);
        qDebug() << "Current glucose can be set";
    } catch (...) {
        qDebug() << "FAIL: Exception when setting current glucose";
        QFAIL("Exception occurred when setting current glucose");
    }
}

void InsulinPumpTest::testInsulinBolus() {
    qDebug() << "=== TEST: Insulin Bolus ===";
    InsulinControlSystem ics;
    
    // Set initial values
    double initialGlucose = 7.0;
    ics.setCurrentGlucose(initialGlucose);
    
    // Test bolus injection
    double bolusAmount = 2.0;
    try {
        ics.simulateBolus(bolusAmount);
        qDebug() << "Bolus injection can be simulated";
    } catch (...) {
        qDebug() << "FAIL: Exception when simulating bolus";
        QFAIL("Exception occurred when simulating bolus");
    }
    
    // Verify that bolus affects insulin on board
    bool iobIncreased = ics.getInsulinOnBoard() > 0;
    if (iobIncreased) {
        qDebug() << "Bolus affects insulin on board";
    } else {
        qDebug() << "FAIL: Insulin on board is" << ics.getInsulinOnBoard() << "expected > 0";
    }
    QVERIFY2(iobIncreased, "Insulin on board should increase after bolus");
}

void InsulinPumpTest::testProfiles() {
    qDebug() << "=== TEST: Profiles ===";
    Device device;
    
    // Test profile application - no direct verification, check for exceptions
    try {
        double basalRate = 1.2;
        double correctionFactor = 1.8;
        int carbRatio = 10;
        double targetGlucose = 5.5;
        
        device.applyProfile(basalRate, correctionFactor, carbRatio, targetGlucose);
        qDebug() << "Profiles can be applied";
    } catch (...) {
        qDebug() << "FAIL: Exception when applying profile";
        QFAIL("Exception occurred when applying profile");
    }
}

void InsulinPumpTest::testInsulinOnBoard() {
    qDebug() << "=== TEST: Insulin On Board ===";
    InsulinControlSystem ics;
    
    // Test initial IOB state
    bool initialIobCorrect = ics.getInsulinOnBoard() == 0.0;
    if (initialIobCorrect) {
        qDebug() << "Initial insulin on board is zero";
    } else {
        qDebug() << "FAIL: Initial insulin on board is" << ics.getInsulinOnBoard() << "expected 0.0";
    }
    QVERIFY2(initialIobCorrect, "Initial insulin on board should be zero");
    
    // Add insulin via bolus
    ics.simulateBolus(3.0);
    bool iobIncreased = ics.getInsulinOnBoard() > 0;
    if (iobIncreased) {
        qDebug() << "Insulin on board increases after bolus";
    } else {
        qDebug() << "FAIL: Insulin on board after bolus is" << ics.getInsulinOnBoard() << "expected > 0";
    }
    QVERIFY2(iobIncreased, "Insulin on board should increase after bolus");
}

void InsulinPumpTest::testCartridgeLevel() {
    qDebug() << "=== TEST: Cartridge Level ===";
    InsulinControlSystem ics;
    
    // Test cartridge refill
    ics.refillCartridge();
    bool refillCorrect = ics.getCartridgeLevel() == 300.0;
    if (refillCorrect) {
        qDebug() << "Cartridge can be refilled to 300 units";
    } else {
        qDebug() << "FAIL: Cartridge level after refill is" << ics.getCartridgeLevel() << "expected 300.0";
    }
    QVERIFY2(refillCorrect, "Cartridge level should be 300.0 after refill");
    
    // Test cartridge depletion
    double depletionAmount = 50.0;
    ics.depleteCartridge(depletionAmount);
    bool depletionCorrect = ics.getCartridgeLevel() == 300.0 - depletionAmount;
    if (depletionCorrect) {
        qDebug() << "Cartridge level decreases after depletion";
    } else {
        qDebug() << "FAIL: Cartridge level after depletion is" << ics.getCartridgeLevel() 
                 << "expected" << (300.0 - depletionAmount);
    }
    QVERIFY2(depletionCorrect, "Cartridge level should decrease by the depletion amount");
    
    // Test cartridge level bounds
    ics.depleteCartridge(500.0);
    bool lowerBoundCorrect = ics.getCartridgeLevel() == 0.0;
    if (lowerBoundCorrect) {
        qDebug() << "Cartridge level cannot go below zero";
    } else {
        qDebug() << "FAIL: Cartridge level after excessive depletion is" << ics.getCartridgeLevel() << "expected 0.0";
    }
    QVERIFY2(lowerBoundCorrect, "Cartridge level should be bounded to 0");
}

// Function that will be called from main.cpp to run the tests
void runTests() {
    InsulinPumpTest testInstance;
    QTest::qExec(&testInstance);
}

#include "tests.moc"
