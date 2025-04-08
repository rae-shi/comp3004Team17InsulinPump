#ifndef INSULINPUMP_H
#define INSULINPUMP_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QString>
#include <QtCore/QRandomGenerator>

// -------------------- Device Class --------------------
class Device : public QObject {
    Q_OBJECT

public:
    explicit Device(QObject *parent = nullptr);
    void setupDevice();
    void startDevice();
    void runDevice();
    void stopDevice();
    void chargeBattery();
    void depleteBattery();
    void refillCartridge();
    void setBatteryLevel(int level);
    int getBatteryLevel() const;

public slots:
    void applyProfile(double basalRate, double correctionFactor, int carbRatio, double targetGlucose);

signals:
    void batteryLevelChanged(int level);
    void insulinInjected(double amount);
    void logEvent(const QString &event);
    void logError(const QString &event);
    void devicePoweredOff(); // New signal for battery depletion power off

private:
    int batteryLevel; // 0-100%
    int timeStep;
    bool isRunning;

    class InsulinControlSystem *ics;
    class Logger *logger;
};

// -------------------- Insulin Control System --------------------
class InsulinControlSystem : public QObject {
    Q_OBJECT

public:
    //enum Mode { Normal, Morning, Sleep, Exercise };
    enum State { Run, Stop, Pause, Resume };

    explicit InsulinControlSystem(QObject *parent = nullptr);

    void updateInsulin(); // runs every time step
    double getCarbRatio() const;
    double getCorrectionFactor() const;
    double getTargetGlucose() const;
    double getInsulinOnBoard() const;
    double getCartridgeLevel() const;

    void setState(State state);
    void setBasalRate(double rate);
    void setProfileBasalRate(double rate);
    void setCarbRatio(int carb);
    void setCorrectionFactor(double factor);
    void setTargetGlucose(double level);
    void setCurrentGlucose(double level);
    void calculateBolus(double carbInput, double glucoseInput, double bolusDurationHour, double bolusDurationMin);
    void simulateBolus(double bolus);
    void scheduleExtendedBolus(double bolusPerHour, int hours);
    void setTimeStep(int ts);
    void refillCartridge();
    void depleteCartridge(double amount);

signals:
    void insulinDelivered(double amount);
    void glucoseChanged(double level);
    void cartChanged(double level);
    void IOBChanged(double amount, double hours);
    void logEvent(const QString &event);
    void logError(const QString &event);
    void addPointy(int t, double g);

private:
    int timeStep;
    double basalRate;
    double profileBasalRate;
    double correctionFactor;
    int carbRatio;
    double targetGlucose;
    double currentGlucose;
    double insulinOnBoard;
    double cartLevel;
    State currentState;

};

// -------------------- Logger --------------------
class Logger : public QObject {
    Q_OBJECT

public:
    explicit Logger(QObject *parent = nullptr);

    void logBasalRate(double rate);
    void logBolus(double amount);
    //void logCorrectionFactor(double factor);
    void printToConsole();

private:
    QVector<QString> eventLog;
};

#endif // INSULINPUMP_H
