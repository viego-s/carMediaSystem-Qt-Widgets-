#ifndef MAPLABEL_H
#define MAPLABEL_H

#include <QLabel>
#include <QVector>
#include <QPointF>

class MapLabel : public QLabel {
    Q_OBJECT
public:
    explicit MapLabel(QWidget *parent = nullptr);
    void setRoutePoints(const QVector<QPointF> &points);
    void setMapParams(double centerLng, double centerLat, int zoom);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPointF> routePoints;
    double centerLng = 0;
    double centerLat = 0;
    int zoom = 11;
};

#endif // MAPLABEL_H
