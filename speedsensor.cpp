#include "SpeedSensor.h"

SpeedSensor::SpeedSensor(QObject *parent) : QObject(parent) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SpeedSensor::updateSpeed);
}

void SpeedSensor::startSimulation() {
    timer->start(1000); // 每秒触发一次
}

void SpeedSensor::updateSpeed() {
    double speed = QRandomGenerator::global()->bounded(120.0); // 0-120 km/h
    emit speedUpdated(speed);
}
