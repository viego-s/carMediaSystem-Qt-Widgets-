#include "mapwindow.h"
#include "ui_mapwindow.h"
#include <QNetworkAccessManager>
#include "common.h"
#include <QFile>
#include <QPixmap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QMouseEvent>
#include <cmath>
#include <QJsonArray>
MapWindow::MapWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MapWindow)
{
    ui->setupUi(this);
    mapLabel = qobject_cast<MapLabel*>(ui->label_map);
    if (!mapLabel) {
        qDebug() << "Error: label_map is not a MapLabel!";
        return;
    }
    //先获取公网IP
    this->getIp();
    // // 通过IP获取经纬度
    //  this->getLngLat();
    // // 发起请求调用百度静态图API接口
    //  this->initMap();
    setMouseTracking(true);    // 启用鼠标跟踪
    ui->label_map->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    // 初始化坐标
    lastLng = lng.toDouble();
    lastLat = lat.toDouble();
}

MapWindow::~MapWindow()
{
    delete ui;
}

void MapWindow::initMap()
{   //发起请求
    QNetworkAccessManager *manager=new QNetworkAccessManager;
    QString url=QString(BAIDU_MAP_URL)
                      .arg(BAIDU_MAP_AK).arg(this->lng).arg(this->lat).arg(this->zoom);
    manager->get(QNetworkRequest(QUrl(url)));

    //通过信号和槽获取响应结果
    connect(manager,&QNetworkAccessManager::finished,this,&MapWindow::showMap);
    mapLabel->setMapParams(lng.toDouble(), lat.toDouble(), zoom);
    mapLabel->update(); // 强制立即重绘
}

void MapWindow::getIp()
{
    QNetworkAccessManager *manager =new QNetworkAccessManager;
    manager->get(QNetworkRequest(QUrl("http://ipinfo.io/ip")));//获取IP
    connect(manager,&QNetworkAccessManager::finished,this,&MapWindow::readIP);
}

void MapWindow::getLngLat()
{
    QNetworkAccessManager *manager =new QNetworkAccessManager;
    QString url=QString(BAIDU_MAP_IP_URL).arg(this->ip).arg(BAIDU_MAP_AK);
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,&QNetworkAccessManager::finished,this,&MapWindow::readLngLat);
}

void MapWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
        isDragging = true;
        lastLng = lng.toDouble();  // 记录拖拽前的坐标
        lastLat = lat.toDouble();
    }
    QMainWindow::mousePressEvent(event);
}

void MapWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging) {
        // 实时更新地图（可选，性能要求较高）
        // 或仅记录偏移量，在release时处理
    }
    QMainWindow::mouseMoveEvent(event);
}

void MapWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDragging) {
        QPoint dragEndPosition = event->pos();
        QPoint offset = dragEndPosition - dragStartPosition;

        // 百度地图分辨率计算（单位：米/像素）
        double resolution = 156543.03392 * cos(lastLat * M_PI / 180) / (1 << zoom);

        // 转换为经纬度变化（1度≈111公里）
        double deltaLng = -offset.x() * resolution / 111000;//经度方向相反
        double deltaLat = offset.y() * resolution / 111000;  // 纬度方向反向

        // 更新坐标
        lng = QString::number(lastLng + deltaLng);
        lat = QString::number(lastLat + deltaLat);

        // 重新加载地图
        initMap();
        isDragging = false;
    }
    // 独立处理路径规划的点击事件
    if (event->button() == Qt::LeftButton && !isDragging) {
        // 确保点击在 label_map 区域内
        if (ui->label_map->geometry().contains(event->pos())) {
            QPoint clickPos = event->pos() - ui->label_map->pos();
            double centerLng = lng.toDouble();
            double centerLat = lat.toDouble();

            //百度经纬度坐标系的分辨率计算（单位：度/像素）
            double lngPerPixel = 360.0 / (256 * pow(2, zoom)); // 经度每像素代表的度数
            double latPerPixel = lngPerPixel; // 近似值（纬度需根据实际位置调整）

            // 计算经纬度偏移
            int mapWidth = ui->label_map->width();
            int mapHeight = ui->label_map->height();
            double deltaLng = (clickPos.x() - mapWidth/2.0) * lngPerPixel;
            double deltaLat = (mapHeight/2.0 - clickPos.y()) * latPerPixel;

            if (isSelectingStart) {
                // 设置起点
                startPoint.setX(centerLng + deltaLng);
                startPoint.setY(centerLat + deltaLat);
                isSelectingStart = false;
                isSelectingEnd = true;
                QMessageBox::information(this, "导航", "起点已设置，请点击地图选择终点");
            } else if (isSelectingEnd) {
                // 设置终点并请求路径
                endPoint.setX(centerLng + deltaLng);
                endPoint.setY(centerLat + deltaLat);

                QNetworkAccessManager *manager = new QNetworkAccessManager;
                QString url = QString(BAIDU_ROUTE_URL)
                                  .arg(BAIDU_MAP_AK)
                                  .arg(startPoint.y()).arg(startPoint.x())
                                  .arg(endPoint.y()).arg(endPoint.x());
                manager->get(QNetworkRequest(QUrl(url)));
                connect(manager, &QNetworkAccessManager::finished,
                        this, &MapWindow::handleRouteResponse);
                isSelectingEnd = false;
            }
        }
    }
    QMainWindow::mouseReleaseEvent(event);
}


//调用静态图接口，返回来的是一个图片的流
void MapWindow::showMap(QNetworkReply *reply)
{
    QByteArray data=reply->readAll();
    //qDebug()<<QString(data);
    //1.将图片的流写到本地的一个图片文件中去
    QFile file("./temp.png");
    bool open=file.open(QIODevice::ReadWrite);
    if(!open){
        qDebug()<<"文件打开失败";
        return;
    }
    file.write(data);
    file.close();
    //2.加载这个图片为一个QPixmap对象
    QPixmap map("./temp.png");
    //3.将QPixmap设置给QLabel
    ui->label_map->setPixmap(map);
}


void MapWindow::on_btn_back_clicked()
{
    this->hide();
    this->parentWidget()->show();
}

void MapWindow::readIP(QNetworkReply *reply)
{
    QByteArray data=reply->readAll();
    this->ip=QString(data);
    qDebug()<<"ip"<<this->ip;
    //要确认等拿到IP才能继续发送请求，获取经纬度
    this->getLngLat();

}

/*{
  "status": 0,
  "address": "CN|四川省|成都市|武侯区|None|100|100|100",
  "content": {
    "address": "四川省成都市武侯区",
    "address_detail": {
      "adcode": "510107",
      "city": "成都市",
      "city_code": 75,
      "district": "武侯区",
      "province": "四川省",
      "street": "",
      "street_number": ""
    },
    "point": {
      "x": "104.04968813094315",
      "y": "30.648208749222277"
    }
  }
}*/
void MapWindow::readLngLat(QNetworkReply *reply)
{
    QByteArray data=reply->readAll();
    QJsonDocument doc=QJsonDocument::fromJson(data);
    if(doc.isObject()){
        QJsonObject object=doc.object();
        QJsonObject content=object.value("content").toObject();
        QJsonObject point=content.value("point").toObject();
        this->lng=point.value("x").toString();
        this->lat=point.value("y").toString();
        //确认拿到经纬度以后，才去获取静态图
        this->initMap();
    }
}



void MapWindow::on_btn_enlarge_clicked()
{   if(this->zoom>=18){
        QMessageBox::warning(this,"警告","已经最大化，不能继续放大");
        return;
    }
    this->zoom++;
    //重新发起请求，刷新
    this->initMap();
}


void MapWindow::on_btn_reduce_clicked()
{   if(this->zoom<=3)
    {
        QMessageBox::warning(this,"警告","已经最小化，不能继续缩小");
    }
    this->zoom--;
    this->initMap();
}



void MapWindow::handleRouteResponse(QNetworkReply *reply)
{   if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "API请求失败：" << reply->errorString();
        return;
    }
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    routePoints.clear();

    if(doc.object().value("status").toInt() == 0){
        QJsonArray routes = doc.object().value("result").toObject().value("routes").toArray();

        if(!routes.isEmpty()){
            QJsonArray steps = routes.first().toObject()
            .value("steps").toArray();
            routePoints.clear();

            for(const QJsonValue &step : steps){
                QString pathStr = step.toObject().value("path").toString();
                QStringList points = pathStr.split(';'); // 按分号拆分坐标点
                for (const QString &point : points) {
                    QStringList coords = point.split(','); // 按逗号拆分经纬度
                    if (coords.size() == 2) {
                        bool okLng, okLat;
                        double lng = coords[0].toDouble(&okLng);
                        double lat = coords[1].toDouble(&okLat);
                        if (okLng && okLat) {
                            routePoints.append(QPointF(lng, lat));
                        }
                    }
                }

            }
            qDebug() << "Parsed route points:";
            for (const QPointF &point : routePoints) {
                qDebug() << "Lng:" << point.x() << "Lat:" << point.y();
            }
            mapLabel->setRoutePoints(routePoints);
            mapLabel->update(); // 强制立即重绘
            QMessageBox::information(this, "成功", "路径规划完成！");

        }
    }else{
        QString errorMsg = doc.object().value("message").toString();
        QMessageBox::warning(this, "错误", "路径规划失败: " + errorMsg);
    }

}


void MapWindow::on_navigate_clicked()
{
    QMessageBox::information(this, "导航", "请先点击地图选择起点");
    isSelectingStart = true;
    isSelectingEnd = false;
}

