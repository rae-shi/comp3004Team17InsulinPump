#ifndef INSULINPUMP_H
#define INSULINPUMP_H

#include <QObject>
#include <QString>
#include <QVector>

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

    int getBatteryLevel() const;

signals:
    void batteryLevelChanged(int level);
    void insulinInjected(double amount);
    void logEvent(const QString &event);

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
    enum State { Start, Stop, Resume };

    explicit InsulinControlSystem(QObject *parent = nullptr);

    void updateInsulin(); // runs every time step
    void setState(State state);
    //void setMode(Mode mode);
    void setBasalRate(double rate);
    void setCurrentGlucose(double level);
    void simulateBolus(double glucoseLevel);

    double getInsulinOnBoard() const;

signals:
    void insulinDelivered(double amount);
    void glucoseChanged(double level);
    void cartChanged(double level);
    void IOBChanged(double amount);
    void bolusInjected(double units);
    void logEvent(const QString &event);

private:
    double basalRate;
    double currentGlucose;
    double insulinOnBoard;
    double cartLevel;
    //Mode currentMode;
    State currentState;

    double calculateBolus(double glucose);
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
