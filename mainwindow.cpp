#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QObject>
#include<QQuickItem>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    speedSensor(new SpeedSensor(this))
{
    ui->setupUi(this);
    //使用定时器，每秒做一件事
    this->timer=new QTimer();
    connect(this->timer,&QTimer::timeout,this,&MainWindow::showDateTime);
    //实时获取时间
    this->showDateTime();

    //安装事件过滤器
    this->eventFilter=new labelClickEventFilter();
    ui->label_weather->installEventFilter(this->eventFilter);
    ui->label_music->installEventFilter(this->eventFilter);
    ui->label_video->installEventFilter(this->eventFilter);
    ui->label_map->installEventFilter(this->eventFilter);

    //绑定事件过滤器信号和当前类的槽函数
    connect(this->eventFilter,&labelClickEventFilter::goOtherWindow,this,&MainWindow::goPage);

    //实例化各个窗口对象
    this->weaWin=new WeatherWindow(this);
    this->musicWin=new MusicWindow(this);
    this->videoWin=new videoWindow(this);
    this->mapWin=new MapWindow(this);

    // 连接车速信号到视频窗口
    connect(speedSensor, &SpeedSensor::speedUpdated,
            videoWin, &videoWindow::onSpeedChanged,
            Qt::QueuedConnection); // 确保线程安全

    // 启动车速模拟
    speedSensor->startSimulation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showDateTime()
{
    //实时获取时间
    QDateTime currentDateTime =QDateTime::currentDateTime();
    QString time=currentDateTime.toString("HH:mm:ss");
    QString date=currentDateTime.toString("yyyy-MM-dd ddd");
    ui->label_time->setText(time);
    ui->label_date->setText(date);
    this->timer->start(1000);
}
/**
* @brief MainWindow::goPage
* @param watched  通过这个参数去判断，要跳转到哪个页面
*/
void MainWindow::goPage(QObject *watched)
{
    QString objName=watched->objectName();
    if(objName=="label_weather"){
        this->weaWin->show();
    }else if(objName=="label_music"){
        this->musicWin->show();
    }else if(objName=="label_video"){
        this->videoWin->show();
    }else if(objName=="label_map"){
        this->mapWin->show();

    }
    this->hide();
}


