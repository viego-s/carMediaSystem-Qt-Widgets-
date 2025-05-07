#include "maplabel.h"
#include <QPainter>
#include <cmath>

MapLabel::MapLabel(QWidget *parent) : QLabel(parent) {}

void MapLabel::setRoutePoints(const QVector<QPointF> &points) {
    routePoints = points;
    update(); // 触发重绘
}

void MapLabel::setMapParams(double lng, double lat, int z) {
    centerLng = lng;
    centerLat = lat;
    zoom = z;
}

void MapLabel::paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);
    if (!routePoints.isEmpty()) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen pen(Qt::red, 3);
        painter.setPen(pen);

        // 百度地图的经纬度转像素公式
        const int tileSize = 256; // 瓦片尺寸
        const double zoomFactor = pow(2, zoom);
        const double centerPixelX = (centerLng * tileSize * zoomFactor) / 360.0 + (width() / 2);
        const double centerPixelY = ( (1 - log(tan(centerLat * M_PI / 180) + 1 / cos(centerLat * M_PI / 180)) / M_PI) / 2 * tileSize * zoomFactor ) + (height() / 2);

        QPolygonF screenPoints;
        for (const QPointF &point : routePoints) {
            double lng = point.x();
            double lat = point.y();

            // 计算当前点的像素坐标
            double pixelX = (lng * tileSize * zoomFactor) / 360.0 + (width() / 2);
            double pixelY = ( (1 - log(tan(lat * M_PI / 180) + 1 / cos(lat * M_PI / 180)) / M_PI) / 2 * tileSize * zoomFactor ) + (height() / 2);

            // 计算相对于中心点的偏移
            pixelX -= (centerPixelX - width() / 2);
            pixelY -= (centerPixelY - height() / 2);

            screenPoints << QPointF(pixelX, pixelY);
        }

        painter.drawPolyline(screenPoints);
    }
}
