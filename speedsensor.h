#ifndef SPEEDSENSOR_H
#define SPEEDSENSOR_H

#include <QObject>
#include <QTimer>
#include <QRandomGenerator>

class SpeedSensor : public QObject {
    Q_OBJECT
public:
    explicit SpeedSensor(QObject *parent = nullptr);
    void startSimulation(); // 启动模拟

signals:
    void speedUpdated(double speed);

private slots:
    void updateSpeed();

private:
    QTimer *timer;
};

#endif // SPEEDSENSOR_H
