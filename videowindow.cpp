#include "videowindow.h"
#include "ui_videowindow.h"
#include <QFileDialog>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
videoWindow::videoWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::videoWindow)
{
    ui->setupUi(this);
    //给播放按钮添加mode属性
    ui->btn_play->setProperty("mode","pause");
    // 初始化播放组件
    this->player=new QMediaPlayer;
    this->playlist=new QMediaPlaylist;
    this->onlinePlaylist=new QMediaPlaylist;
    this->player->setPlaylist(this->playlist);

    //给视频播放器设置视频输出窗口
    this->videowidget=new QVideoWidget();
    //要告诉程序，videoWidget它是要在什么地方
    ui->video_Layout->addWidget(videowidget);
    this->player->setVideoOutput(videowidget);

    //给videowidget安装事件过滤器
    this->eventFilter=new ExitFullScreenEventFilter();
    this->videowidget->installEventFilter(this->eventFilter);

    //给事件过滤器对象绑定信号和槽
    connect(this->eventFilter,&ExitFullScreenEventFilter::exitFullScreen,this,&videoWindow::exitFullScreen);

    //给QMediaPlayer绑定信号与槽,去计算进度条
    connect(this->player,&QMediaPlayer::durationChanged,this,&videoWindow::durationChanged);
    connect(this->player,&QMediaPlayer::positionChanged,this,&videoWindow::positionChanged);

    //初始化数据库和在线列表
    initDatabase();
    loadFromDatabase();
    initOnlineList();

    // 初始化播放模式
    localPlaybackMode = QMediaPlaylist::Loop;
    onlinePlaybackMode = QMediaPlaylist::Loop;
    playlist->setPlaybackMode(localPlaybackMode);
    onlinePlaylist->setPlaybackMode(onlinePlaybackMode);
}

videoWindow::~videoWindow()
{
    delete ui;
}

void videoWindow::on_btn_quit_clicked()
{
    this->hide();
    this->parentWidget()->show();
}


void videoWindow::on_btn_play_clicked()
{
    QString mode=ui->btn_play->property("mode").toString();
    if(mode=="pause"){
        ui->btn_play->setProperty("mode","play");
        this->player->play();
    }else{
        ui->btn_play->setProperty("mode","pause");
        this->player->pause();
    }
    ui->btn_play->style()->unpolish(ui->btn_play);
    ui->btn_play->style()->polish(ui->btn_play);
    ui->btn_play->update();
}


void videoWindow::on_btn_file_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this, "选择视频文件", QDir::homePath(),
        "视频文件 (*.mp4 *.avi *.mkv)");

    foreach (const QString &path, files) {
        QSqlQuery query(db);
        query.prepare("SELECT id FROM t_video WHERE file_path = ?");
        query.addBindValue(path);

        if (query.exec() && !query.next()) {
            addToDatabase(path);
            playlist->addMedia(QUrl::fromLocalFile(path));
            ui->list_local_3->addItem(QFileInfo(path).fileName());
        }
    }
}

//上一个视频
void videoWindow::on_btn_prev_clicked()
{
    if(player->playlist()==playlist){//判断是在线列表还是本地列表
        // 改为使用播放列表的内置导航
        playlist->previous();
        ui->list_local_3->setCurrentRow(playlist->currentIndex());}
    else{

        onlinePlaylist->previous();
        ui->list_online_3->setCurrentRow(onlinePlaylist->currentIndex());
    }
}


void videoWindow::on_btn_next_clicked()
{
 if(player->playlist()==playlist){
        // 改为使用播放列表的内置导航
        playlist->next();
        ui->list_local_3->setCurrentRow(playlist->currentIndex());}
    else{
        onlinePlaylist->next();
        ui->list_online_3->setCurrentRow(onlinePlaylist->currentIndex());
    }
}


void videoWindow::on_list_local_3_itemDoubleClicked(QListWidgetItem *item)
{   player->setPlaylist(playlist);
    int index=ui->list_local_3->currentRow();
    this->playlist->setCurrentIndex(index);
    this->player->play();
    ui->btn_play->setProperty("mode","play");
    ui->list_local_3->setCurrentRow(this->playlist->currentIndex());
    //当属性mode发生改变以后，重新刷新样式
    ui->btn_play->style()->unpolish(ui->btn_play);
    ui->btn_play->style()->polish(ui->btn_play);
    ui->btn_play->update();
}
//获取当前播放视频的总长度
void videoWindow::durationChanged(qint64 duration)
{
    this->total=duration;
}
//获取当前歌曲的播放位置
void videoWindow::positionChanged(qint64 position)
{
    // 计算播放进度百分比=当前位置/总长度*100
    //在歌曲切换时总长度会可能为0，这时候需要进行一个判断
    if(this->total>0){
        int value=position*100/this->total;
        ui->play_progress_bar->setValue(value);
    }
}

//进度条滑动
void videoWindow::on_play_progress_bar_sliderReleased()
{
        int value=ui->play_progress_bar->value();
        int position= value*this->total/100;
        player->setPosition(position);

}


void videoWindow::on_btn_fullscreen_clicked()
{
   //将子窗口设置为顶层窗口
    this->videowidget->setWindowFlags(Qt::Window);
   //设置全屏
    this->videowidget->showFullScreen();
}

void videoWindow::exitFullScreen()
{
    //将窗口设置为子窗口
    this->videowidget->setWindowFlags(Qt::SubWindow);
    //退出全屏
    this->videowidget->showNormal();
}
//数据库初始化
void videoWindow::initDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "video_connection");
    db.setDatabaseName("video.db");

    if (!db.open()) {
        qDebug() << "视频数据库打开失败:" << db.lastError().text();
        return;
    }

    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS t_video ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "file_path TEXT UNIQUE NOT NULL, "
               "file_name TEXT NOT NULL)");
}
//从数据库加载
void videoWindow::loadFromDatabase()
{
    QSqlQuery query(db);
    if (!query.exec("SELECT file_path, file_name FROM t_video")) {
        qDebug() << "数据库查询失败:" << query.lastError();
        return;
    }
    while (query.next()) {
        QString path = query.value(0).toString();
        QString name = query.value(1).toString();
        playlist->addMedia(QUrl::fromLocalFile(path));
        ui->list_local_3->addItem(name);
    }
}
//添加到数据库
void videoWindow::addToDatabase(const QString &path)
{
    QFileInfo file(path);
    QSqlQuery query(db);
    query.prepare("INSERT INTO t_video (file_path, file_name) VALUES (?, ?)");
    query.addBindValue(path);
    query.addBindValue(file.fileName());

    if (!query.exec()) {
        qDebug() << "插入数据失败:" << query.lastError();
    }
}
//初始化在线列表
void videoWindow::initOnlineList()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &videoWindow::showOnlineList);
    manager->get(QNetworkRequest(QUrl("http://localhost/video_list.json")));
}


void videoWindow::onSpeedChanged(double speed)
{
    // 更新车速显示
    QString speedText = QString("车速：%1 km/h").arg(speed, 0, 'f', 1);
    ui->label_speed->setText(speedText);

    // 安全控制逻辑
    if (speed > speedThreshold) {
        if (player->state() == QMediaPlayer::PlayingState) {
            player->pause();
            ui->btn_play->setProperty("mode", "pause");
            ui->label_speed->setStyleSheet("color: #e74c3c;"); // 超速时红色
        }
    } else {
        if (player->state() == QMediaPlayer::PausedState) {
            ui->label_speed->setStyleSheet("color: #2ecc71;"); // 正常时绿色
        }
    }
    // 刷新按钮样式
    ui->btn_play->style()->polish(ui->btn_play);
}

//在线列表双击事件处理
void videoWindow::on_list_online_3_itemDoubleClicked(QListWidgetItem *item)
{
    player->setPlaylist(onlinePlaylist);
    int index = ui->list_online_3->row(item);
    onlinePlaylist->setCurrentIndex(index);
    player->play();

    // 更新播放按钮状态
    ui->btn_play->setProperty("mode", "play");
    ui->btn_play->style()->polish(ui->btn_play);
}
//处理在线列表数据
void videoWindow::showOnlineList(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "网络请求错误:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();

    ui->list_online_3->clear();
    onlinePlaylist->clear();

    foreach (const QJsonValue &value, array) {
        QJsonObject obj = value.toObject();
        QString name = obj["name"].toString();
        QString url = obj["url"].toString();

        onlinePlaylist->addMedia(QUrl(url));
        ui->list_online_3->addItem(name);
    }
    reply->deleteLater();

}


void videoWindow::on_btn_del_clicked()
{
    int index = ui->list_local_3->currentRow();
    if (index == -1) return;

    QMediaContent media = playlist->media(index);
    QString path = media.request().url().toLocalFile();

    QSqlQuery query(db);
    query.prepare("DELETE FROM t_video WHERE file_path = ?");
    query.addBindValue(path);

    if (query.exec()) {
        playlist->removeMedia(index);
        delete ui->list_local_3->takeItem(index);
    }
}

