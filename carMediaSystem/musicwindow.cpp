#include "musicwindow.h"
#include "ui_musicwindow.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QSqlError>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
MusicWindow::MusicWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MusicWindow)
{
    ui->setupUi(this);
    ui->btn_play->setProperty("mode","pause");
    this->player=new QMediaPlayer;
    this->playlist=new QMediaPlaylist;
    this->player->setPlaylist(this->playlist);
    this->initOnlineList();
    //修改播放模式为随机
    this->playlist->setPlaybackMode(QMediaPlaylist::Random);
    //给QMediaPlayer绑定信号与槽,去计算进度条
    connect(this->player,&QMediaPlayer::durationChanged,this,&MusicWindow::durationChanged);
    connect(this->player,&QMediaPlayer::positionChanged,this,&MusicWindow::positionChanged);
    // 添加歌词模型初始化
    lyricModel = new QStringListModel(this);
    ui->lyricView->setModel(lyricModel);
    ui->lyricView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //查询数据库t_music表，看表中有没有数据，如果有，就要查询出所有的数据，添加给QMediaPlaylist,并且还要展示到页面上
    //将数据库中的数据展示到listWidget控件
    initDatabase();
    loadFromDatabase(); // 加载数据库中的音乐
    int index = ui->list_local->currentRow();
    if(index == -1) return;

    // 获取文件路径
    QMediaContent media = playlist->media(index);
    QString path = media.request().url().toLocalFile();

    // 从数据库删除
    QSqlQuery query;
    query.prepare("DELETE FROM t_music WHERE file_path = ?");
    query.addBindValue(path);
    query.exec();

    // 从播放列表和界面删除
    playlist->removeMedia(index);
    delete ui->list_local->takeItem(index);
    connect(ui->btn_del, &QPushButton::clicked, this, &MusicWindow::on_btn_del_clicked);


}

MusicWindow::~MusicWindow()
{
    delete ui;
}

void MusicWindow::initOnlineList()
{
    //向后端服务器发起http请求
    QNetworkAccessManager *manager=new QNetworkAccessManager;
    manager->get(QNetworkRequest(QUrl("http://localhost/audio_list.json")));
    connect(manager,&QNetworkAccessManager::finished,this,&MusicWindow::showOnlineList);
}


void MusicWindow::on_btn_quit_clicked()
{
    this->hide();
    this->parentWidget()->show();
}
//歌词解析函数
void MusicWindow::parseLyrics(const QString &musicPath)
{   // 设置歌词背景
    setLyricsBackground(musicPath);
    lyrics.clear();
    QString lyricPath = musicPath.left(musicPath.lastIndexOf('.')) + ".lrc";
    QFile lyricFile(lyricPath);

    if(!lyricFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        lyricModel->setStringList({"<无歌词文件>"});
        return;
    }

    QTextStream stream(&lyricFile);
    stream.setCodec("UTF-8");  // 强制使用UTF-8编码
    QStringList lyricLines;
    QRegularExpression timeRegex("\\[(\\d+):(\\d+\\.?\\d*)\\](.*)");//精确到百分秒

    while(!stream.atEnd()){
        QString line = stream.readLine();
        QRegularExpressionMatch match = timeRegex.match(line);

        if(match.hasMatch()){
            qint64 minutes = match.captured(1).toInt();
            double seconds = match.captured(2).toDouble();
            qint64 timeMs = (minutes * 60 + seconds) * 1000;
            QString text = match.captured(3).trimmed();

            lyrics[timeMs] = text;
            lyricLines.append(text);
        }
    }

    lyricModel->setStringList(lyricLines);
    lyricFile.close();
}

void MusicWindow::setLyricsBackground(const QString &musicPath)
{
    // 根据歌曲路径构造专辑封面路径
    QString albumCoverPath = musicPath.left(musicPath.lastIndexOf('.')) + ".jpg";
    // 检查专辑封面文件是否存在
    if (QFileInfo::exists(albumCoverPath)) {
        QPixmap albumCover(albumCoverPath);

        if (!albumCover.isNull()) {
            QSize labelSize = ui->label_background->size();
            // 强制缩放到背景标签的宽度，高度按比例计算
            QPixmap scaled = albumCover.scaledToWidth(labelSize.width(), Qt::SmoothTransformation);

            // 创建与标签尺寸相同的画布
            QPixmap finalPixmap(labelSize);
            finalPixmap.fill(Qt::transparent);
            QPainter painter(&finalPixmap);

            // 计算垂直居中位置（水平方向已铺满）
            int y = (labelSize.height() - scaled.height()) / 2;
            painter.drawPixmap(0, y, scaled); // x=0确保宽度铺满

            // 添加半透明黑色遮罩
            painter.fillRect(finalPixmap.rect(), QColor(0, 0, 0, 100));
            painter.end();

            ui->label_background->setPixmap(finalPixmap);
        } else {
            qDebug() << "无法加载专辑封面：" << albumCoverPath;
        }
    } else {
        ui->label_background->setPixmap(QPixmap("D:/music/找不到封面.jpg"));
    }
}

void MusicWindow::initDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("music.db");//未指定完整路径，就存在应用程序启动时的工作目录中。

    if (!db.open()) {
        qDebug() << "数据库打开失败:" << db.lastError().text();
        return;
    }

    // 创建表
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS t_music ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "file_path TEXT UNIQUE NOT NULL, "
               "file_name TEXT NOT NULL)");
}

void MusicWindow::loadFromDatabase()
{
    QSqlQuery query("SELECT file_path, file_name FROM t_music");
    while (query.next()) {
        QString path = query.value(0).toString();
        QString name = query.value(1).toString();

        playlist->addMedia(QUrl::fromLocalFile(path));
        ui->list_local->addItem(name);
    }
}

void MusicWindow::addToDatabase(const QString &path)
{
    QFileInfo file(path);
    QSqlQuery query;
    query.prepare("INSERT INTO t_music (file_path, file_name) VALUES (?, ?)");
    query.addBindValue(path);
    query.addBindValue(file.fileName());
    query.exec();
}
void MusicWindow::on_btn_play_clicked()
{
    QString mode=ui->btn_play->property("mode").toString() ;
    if(mode=="pause"){
        ui->btn_play->setProperty("mode","play");
        this->player->play();
    }else{
        ui->btn_play->setProperty("mode","pause");
        this->player->pause();
    }
    ui->list_local->setCurrentRow(this->playlist->currentIndex());
    //当属性mode发生改变以后，重新刷新样式
    ui->btn_play->style()->unpolish(ui->btn_play);
    ui->btn_play->style()->polish(ui->btn_play);
    ui->btn_play->update();
}


void MusicWindow::on_btn_file_clicked()
{
    QStringList files=QFileDialog::getOpenFileNames(this,"选择音乐文件","D:/music","Audios(*.mp3)");
    //判断本次选择的文件，之前是否存在，如果已经存在就不添加。
    //怎么判断存不存在？
    //通过name去数据库查询，如果能够查询到，那就说明它存在，就跳过不处理，如果不存在才加入播放列表，存入数据库。

    for(QString path:files){
        //将选中的文件放入媒体播放列表
        this->playlist->addMedia(QMediaContent(QUrl::fromLocalFile(path)));
        QFileInfo file(path);
        // 检查是否已存在(数据库)
        QSqlQuery query;
        query.prepare("SELECT id FROM t_music WHERE file_path = ?");
        query.addBindValue(path);
        query.exec();
        if(!query.next()) { // 不存在则添加
            addToDatabase(path);
            playlist->addMedia(QUrl::fromLocalFile(path));

        ui->list_local->addItem(file.fileName());
        // 自动加载歌词文件
        parseLyrics(path);
        setLyricsBackground(path);
        //将选中的文件存入sqlite数据库
        //封装music对象
        //dao.addMusic(music);
        }
    }
}

//上一首
void MusicWindow::on_btn_prev_clicked()
{
    //1 获取当前播放列表的索引
    int index=this->playlist->currentIndex();
    //2.index--
    //判断如果是第一首，要跳转到其他歌曲
    if(index==0){
        index=this->playlist->mediaCount()-1;
    }else{
        index--;
    }
    //3 重新给播放列表设置当前索引
    this->playlist->setCurrentIndex(index);
    //给页面上的列表设置当前索引，列表选中歌曲
    ui->list_local->setCurrentRow(index);
    parseLyrics(playlist->currentMedia().request().url().toLocalFile());
    setLyricsBackground(playlist->currentMedia().request().url().toLocalFile());
}

//下一首
void MusicWindow::on_btn_next_clicked()
{
    //1 获取当前播放列表的索引
    int index=this->playlist->currentIndex();
    //2.index--
    //判断如果是最后一首，要跳转到其他歌曲
    if(index==this->playlist->mediaCount()-1){
        index=0;
    }else{
        index++;
    }
    //3 重新给播放列表设置当前索引
    this->playlist->setCurrentIndex(index);
    //给页面上的列表设置当前索引，列表选中歌曲
    ui->list_local->setCurrentRow(index);
    parseLyrics(playlist->currentMedia().request().url().toLocalFile());
    setLyricsBackground(playlist->currentMedia().request().url().toLocalFile());
}

void MusicWindow::on_list_local_itemDoubleClicked(QListWidgetItem *item)
{
    int index=ui->list_local->currentRow();
    this->playlist->setCurrentIndex(index);
    this->player->play();
    ui->btn_play->setProperty("mode","play");
    ui->list_local->setCurrentRow(this->playlist->currentIndex());
    //当属性mode发生改变以后，重新刷新样式
    ui->btn_play->style()->unpolish(ui->btn_play);
    ui->btn_play->style()->polish(ui->btn_play);
    ui->btn_play->update();
    parseLyrics(playlist->currentMedia().request().url().toLocalFile());
    setLyricsBackground(playlist->currentMedia().request().url().toLocalFile());
}


//获取当前播放歌曲的总长度
void MusicWindow::durationChanged(qint64 duration)
{
    this->total=duration;
}

//获取当前歌曲的播放位置
void MusicWindow::positionChanged(qint64 position)
{
   // 计算播放进度百分比=当前位置/总长度*100
    //在歌曲切换时总长度会可能为0，这时候需要进行一个判断
    if(this->total>0){
    int value=position*100/this->total;
        ui->play_progress_bar->setValue(value);
    }
    // 歌词滚动逻辑
    if(!lyrics.isEmpty()){
        qint64 currentTime = position;
        QMap<qint64, QString>::const_iterator it = lyrics.upperBound(currentTime);
        if(it != lyrics.constBegin()) --it;

        int row = lyrics.keys().indexOf(it.key());
        QModelIndex index = lyricModel->index(row);
        ui->lyricView->setCurrentIndex(index);
        ui->lyricView->scrollTo(index, QAbstractItemView::PositionAtCenter);
    }
}



void MusicWindow::on_play_progress_bar_sliderReleased()
{
    int value=ui->play_progress_bar->value();
    int position= value*this->total/100;
    player->setPosition(position);

}


void MusicWindow::on_slider_voice_sliderReleased()
{
    int value=ui->slider_voice->value();
    this->player->setVolume(value);
}

void MusicWindow::on_btn_del_clicked()
{
    int index = ui->list_local->currentRow();
    if(index == -1) return;

    // 获取文件路径
    QMediaContent media = playlist->media(index);
    QString path = media.request().url().toLocalFile();

    // 从数据库删除
    QSqlQuery query;
    query.prepare("DELETE FROM t_music WHERE file_path = ?");
    query.addBindValue(path);
    query.exec();

    // 从播放列表和界面删除
    playlist->removeMedia(index);
    delete ui->list_local->takeItem(index);
}
//在线列表
void MusicWindow::showOnlineList(QNetworkReply *reply)
{
    QByteArray data=reply->readAll();
    qDebug()<<QString(data);
    //解析Json文件
    QJsonDocument doc=QJsonDocument::fromJson(data);
    QJsonArray array=doc.array();
    for(int i=0;i<array.size();i++){
        QJsonObject obj=array.at(i).toObject();
        QString name= obj.value("name").toString();
        QString path=obj.value("path").toString();
        QString lrcPath=obj.value("lrcPath").toString();
        ui->list_online->addItem(name);
        this->playlist->addMedia(QMediaContent(QUrl(path)));
    }
}





