#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QPainter>
#include "maplabel.h"
namespace Ui {
class MapWindow;
}

class MapWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MapWindow(QWidget *parent = nullptr);
    ~MapWindow();
    void initMap();
    void getIp();
    void getLngLat();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private slots:
    void showMap(QNetworkReply *reply);

    void on_btn_back_clicked();
    void readIP(QNetworkReply *reply);
    void readLngLat(QNetworkReply *reply);

    void on_btn_enlarge_clicked();

    void on_btn_reduce_clicked();

    void handleRouteResponse(QNetworkReply *reply);  // 新增路径响应处理

    void on_navigate_clicked();

private:
    Ui::MapWindow *ui;
    MapLabel *mapLabel;
    QString lng="104.064328";//经度
    QString lat="30.573457";//纬度
    int zoom=11;//地图级别
    QString ip;

    QPoint dragStartPosition; // 拖拽起始点
    bool isDragging = false;  // 是否正在拖拽

    double lastLng = 0;  // 记录拖拽前的坐标
    double lastLat = 0;

    // 导航相关成员变量
    QVector<QPointF> routePoints;  // 存储路径坐标
    bool isSelectingStart = false;
    bool isSelectingEnd = false;
    QPointF startPoint;
    QPointF endPoint;
};

#endif // MAPWINDOW_H
