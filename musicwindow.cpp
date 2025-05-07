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
#include <QTextCodec>
MusicWindow::MusicWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MusicWindow)
{
    ui->setupUi(this);
    ui->btn_play->setProperty("mode","pause");
    this->player=new QMediaPlayer;
    this->playlist=new QMediaPlaylist;//初始化本地播放列表
    this->onlinePlaylist = new QMediaPlaylist;  // 初始化在线播放列表
    this->player->setPlaylist(this->playlist);
    this->initOnlineList();


    // 初始化播放模式
    localPlaybackMode = QMediaPlaylist::Random;
    onlinePlaybackMode = QMediaPlaylist::CurrentItemInLoop;
    playlist->setPlaybackMode(localPlaybackMode);
    onlinePlaylist->setPlaybackMode(onlinePlaybackMode);
    updatePlayModeButton(); // 初始更新按钮图标


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
    //绑定删除信号
    connect(ui->btn_del, &QPushButton::clicked, this, &MusicWindow::on_btn_del_clicked);
    // 确保播放列表模式同步
    connect(playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
        ui->list_local->setCurrentRow(index);
    });


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
void MusicWindow::parseLyrics(const QString &musicPathOrUrl) {
    // 取消之前的网络请求
    if (currentLyricReply) {
        currentLyricReply->abort();
        currentLyricReply->deleteLater();
        currentLyricReply = nullptr;
    }

    lyrics.clear();
    QStringList lyricLines;


    if (musicPathOrUrl.startsWith("http")||musicPathOrUrl.startsWith("https")) {
        // ==================== 在线歌词处理 ====================
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkRequest request;
        request.setUrl(QUrl(musicPathOrUrl));

        currentLyricReply = manager->get(request); // 记录当前请求

        connect(currentLyricReply, &QNetworkReply::finished, [this, manager]() {
            if (currentLyricReply->error() == QNetworkReply::NoError) {
                QByteArray data = currentLyricReply->readAll();

                // 自动检测编码
                QTextCodec *codec = QTextCodec::codecForUtfText(data, QTextCodec::codecForName("UTF-8"));
                QString content = codec->toUnicode(data);
                QTextStream stream(&content);

                // 改进后的正则表达式
                QRegularExpression timeRegex(R"(^\[(\d+):(\d{1,2}(?:\.\d{1,3})?)\]\s*(.*?)\s*$)");
                QList<QPair<qint64, QString>> tempLyrics;

                while (!stream.atEnd()) {
                    QString line = stream.readLine().trimmed();
                    QRegularExpressionMatch match = timeRegex.match(line);

                    if (match.hasMatch()) {
                        // 解析时间
                        qint64 minutes = match.captured(1).toInt();
                        double seconds = match.captured(2).toDouble();
                        qint64 timeMs = static_cast<qint64>((minutes * 60 + seconds) * 1000);

                        // 过滤非歌词行
                        QString text = match.captured(3).trimmed();
                        if (text.isEmpty() ||
                            text.contains("词：") ||
                            text.contains("曲：") ||
                            text.contains("编曲：")) {
                            continue;
                        }

                        tempLyrics.append(qMakePair(timeMs, text));
                    }
                }

                // 按时间排序（允许重复时间戳）
                std::sort(tempLyrics.begin(), tempLyrics.end(),
                          [](const QPair<qint64, QString> &a, const QPair<qint64, QString> &b) {
                              return a.first < b.first;
                          });

                // 更新歌词数据
                lyrics = tempLyrics;

                // 更新界面
                QStringList displayLines;
                for (const auto &entry : lyrics) {
                    displayLines.append(entry.second);
                }
                lyricModel->setStringList(displayLines);

            } else { // 处理网络错误
                lyricModel->setStringList({"<无法加载歌词>"});
            }

            // 清理资源
            manager->deleteLater();
            currentLyricReply->deleteLater();
            currentLyricReply = nullptr;
        });

    } else {
        // ==================== 本地歌词处理 ====================
        QString lyricPath = musicPathOrUrl.left(musicPathOrUrl.lastIndexOf('.')) + ".lrc";
        QFile lyricFile(lyricPath);

        if (!lyricFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            lyricModel->setStringList({"<无歌词文件>"});
            return;
        }

        // 检测文件编码
        QTextCodec *codec = QTextCodec::codecForUtfText(lyricFile.peek(1024),
                                                        QTextCodec::codecForName("UTF-8"));
        QTextStream stream(&lyricFile);
        stream.setCodec(codec);

        QList<QPair<qint64, QString>> tempLyrics;
        QRegularExpression timeRegex(R"(^\[(\d+):(\d{1,2}(?:\.\d{1,3})?)\]\s*(.*?)\s*$)");

        while (!stream.atEnd()) {
            QString line = stream.readLine().trimmed();
            QRegularExpressionMatch match = timeRegex.match(line);

            if (match.hasMatch()) {
                qint64 minutes = match.captured(1).toInt();
                double seconds = match.captured(2).toDouble();
                qint64 timeMs = static_cast<qint64>((minutes * 60 + seconds) * 1000);

                QString text = match.captured(3).trimmed();
                if (text.isEmpty() ||
                    text.contains("词：") ||
                    text.contains("曲：")) {
                    continue;
                }

                tempLyrics.append(qMakePair(timeMs, text));
            }
        }

        std::sort(tempLyrics.begin(), tempLyrics.end(),
                  [](const QPair<qint64, QString> &a, const QPair<qint64, QString> &b) {
                      return a.first < b.first;
                  });

        lyrics = tempLyrics;
        QStringList displayLines;
        for (const auto &entry : lyrics) {
            displayLines.append(entry.second);
        }
        lyricModel->setStringList(displayLines);

        lyricFile.close();
    }
}

void MusicWindow::setLyricsBackground(const QString &coverPathOrUrl) {
    // 取消之前的封面请求
    if (currentCoverReply) {
        currentCoverReply->abort();
        currentCoverReply->deleteLater();
        currentCoverReply = nullptr;
    }
    // 判断是否是在线路径
    if (coverPathOrUrl.startsWith("http")) {
        // 在线封面处理
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkRequest request;
        request.setUrl(QUrl(coverPathOrUrl));
        currentCoverReply = manager->get(request); // 记录当前请求
        connect(manager, &QNetworkAccessManager::finished, [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                QPixmap albumCover;
                albumCover.loadFromData(data);

                if (!albumCover.isNull()) {
                    QSize labelSize = ui->label_background->size();
                    QPixmap scaled = albumCover.scaledToWidth(labelSize.width(), Qt::SmoothTransformation);

                    QPixmap finalPixmap(labelSize);
                    finalPixmap.fill(Qt::transparent);
                    QPainter painter(&finalPixmap);
                    int y = (labelSize.height() - scaled.height()) / 2;
                    painter.drawPixmap(0, y, scaled);
                    painter.fillRect(finalPixmap.rect(), QColor(0, 0, 0, 100));
                    painter.end();

                    ui->label_background->setPixmap(finalPixmap);
                } else {
                    ui->label_background->setPixmap(QPixmap("D:/music/default_cover.jpg"));
                }
            } else {
                ui->label_background->setPixmap(QPixmap("D:/music/default_cover.jpg"));
            }
            reply->deleteLater();

        });

        manager->get(request);
    } else {
        // 本地封面处理
        QString albumCoverPath = coverPathOrUrl.left(coverPathOrUrl.lastIndexOf('.')) + ".jpg";
        if (QFileInfo::exists(albumCoverPath)) {
            QPixmap albumCover(albumCoverPath);

            if (!albumCover.isNull()) {
                QSize labelSize = ui->label_background->size();
                QPixmap scaled = albumCover.scaledToWidth(labelSize.width(), Qt::SmoothTransformation);

                QPixmap finalPixmap(labelSize);
                finalPixmap.fill(Qt::transparent);
                QPainter painter(&finalPixmap);
                int y = (labelSize.height() - scaled.height()) / 2;
                painter.drawPixmap(0, y, scaled);
                painter.fillRect(finalPixmap.rect(), QColor(0, 0, 0, 100));
                painter.end();

                ui->label_background->setPixmap(finalPixmap);
            } else {
                qDebug() << "无法加载专辑封面：" << albumCoverPath;
                ui->label_background->setPixmap(QPixmap("D:/music/default_cover.jpg"));
            }
        } else {
            ui->label_background->setPixmap(QPixmap("D:/music/default_cover.jpg"));
        }
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
    //歌词设置
    parseLyrics(playlist->currentMedia().request().url().toLocalFile());
    //背景设置
    setLyricsBackground(playlist->currentMedia().request().url().toLocalFile());
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
{   if(player->playlist()==playlist){//判断是在线列表还是本地列表
        // 改为使用播放列表的内置导航
        playlist->previous();
        ui->list_local->setCurrentRow(playlist->currentIndex());
        /*//歌词设置
        parseLyrics(playlist->currentMedia().request().url().toLocalFile());
        //背景设置
        setLyricsBackground(playlist->currentMedia().request().url().toLocalFile());*/}
    else{

            onlinePlaylist->previous();
            ui->list_online->setCurrentRow(onlinePlaylist->currentIndex());
    }
    updateLyricsAndCover();
}

//下一首
void MusicWindow::on_btn_next_clicked()
{   if(player->playlist()==playlist){
        // 改为使用播放列表的内置导航
        playlist->next();
        ui->list_local->setCurrentRow(playlist->currentIndex());
/*        parseLyrics(playlist->currentMedia().request().url().toLocalFile());
        setLyricsBackground(playlist->currentMedia().request().url().toLocalFile());*/}
    else{
        onlinePlaylist->next();
        ui->list_online->setCurrentRow(onlinePlaylist->currentIndex());
    }
    updateLyricsAndCover();
}
// 新增辅助函数
void MusicWindow::updateLyricsAndCover()
{
    if (player->playlist() == playlist) {
        parseLyrics(playlist->currentMedia().request().url().toLocalFile());
        setLyricsBackground(playlist->currentMedia().request().url().toLocalFile());
    } else {
        QMediaContent media = onlinePlaylist->currentMedia();
        QString path = media.request().url().toString();
        parseLyrics(onlineLyricMap.value(path, ""));
        setLyricsBackground(onlineCoverMap.value(path, ""));
    }
}
void MusicWindow::on_list_local_itemDoubleClicked(QListWidgetItem *item)
{   this->player->setPlaylist(playlist);//放在第一行，当双击列表时切换到本地列表
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
    //更新列表切换时的按钮显示
    updatePlayModeButton();
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
    // 更新歌词显示
    if (!lyrics.isEmpty()) {
        // 二分查找当前时间对应的歌词行
        auto it = std::lower_bound(lyrics.begin(), lyrics.end(), position,
                                   [](const QPair<qint64, QString> &entry, qint64 value) {
                                       return entry.first < value;
                                   });

        // 如果找到有效行
        if (it != lyrics.begin()) {
            if (it == lyrics.end() || it->first > position) {
                --it;
            }
            int row = std::distance(lyrics.begin(), it);
            QModelIndex index = lyricModel->index(row);
            ui->lyricView->setCurrentIndex(index);
            ui->lyricView->scrollTo(index, QAbstractItemView::PositionAtCenter);
        }
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
    //qDebug()<<QString(data);
    //解析Json文件
    QJsonDocument doc=QJsonDocument::fromJson(data);
    QJsonArray array=doc.array();
    // 清空在线列表，避免重复加载
    ui->list_online->clear();
    for(int i=0;i<array.size();i++){
        QJsonObject obj=array.at(i).toObject();
        QString name= obj.value("name").toString();
        QString path=obj.value("path").toString();
        QString lrcPath=obj.value("lrcPath").toString();
        QString coverPath=obj.value("coverPath").toString();

        if (lrcPath.isEmpty()) {
            qDebug() << "歌词路径为空，跳过：" << name;
            continue;  // 如果歌词路径为空，跳过该歌曲
        }
        //存储歌词和封面信息
        onlineLyricMap[path] = lrcPath;
        onlineCoverMap[path] = coverPath;
        //添加到在线列表
        ui->list_online->addItem(name);
        this->onlinePlaylist->addMedia(QUrl(path));
    }
    reply->deleteLater();
}


void MusicWindow::on_list_online_itemDoubleClicked(QListWidgetItem *item)
{   this->player->setPlaylist(onlinePlaylist);  // 切换到在线播放列表
    updatePlayModeButton();
    int index = ui->list_online->row(item); // 获取双击行的索引
    if (index == -1) return;

    // 获取在线音乐文件路径
    QMediaContent media = onlinePlaylist->media(index);
    QString path = media.request().url().toString();

    // 检查路径是否有效
    if (path.isEmpty()) {
        qDebug() << "无法播放在线音乐: 路径为空";
        return;
    }

    // 播放在线音乐
    this->onlinePlaylist->setCurrentIndex(index);
    this->player->play();

    // 切换播放按钮的模式
    ui->btn_play->setProperty("mode", "play");
    ui->btn_play->style()->unpolish(ui->btn_play);
    ui->btn_play->style()->polish(ui->btn_play);
    ui->btn_play->update();


    // 加载歌词和背景
    QString lyricPath = onlineLyricMap.value(path, "");  // 从 onlineLyricMap 获取歌词路径
    QString coverPath = onlineCoverMap.value(path, "");  // 从 onlineCoverMap 获取封面路径

    if (!lyricPath.isEmpty()) {
        parseLyrics(lyricPath);  // 加载歌词
    } else {
        lyricModel->setStringList({"<无歌词文件>"});  // 无歌词文件时
    }

    if (!coverPath.isEmpty()) {
        setLyricsBackground(coverPath);  // 加载封面
    } else {
        ui->label_background->setPixmap(QPixmap("D:/music/default_cover.jpg"));
    }
}

void MusicWindow::on_btn_mode_clicked()
{
    // 获取当前播放列表
    QMediaPlaylist *currentList = qobject_cast<QMediaPlaylist*>(player->playlist());

    if (currentList == playlist) {
        // 本地列表模式切换
        switch (localPlaybackMode) {
        case QMediaPlaylist::Random:
            localPlaybackMode = QMediaPlaylist::Sequential;//随机到顺序
            break;
        case QMediaPlaylist::Sequential:
            localPlaybackMode = QMediaPlaylist::Loop;//顺序到列表循环
            break;
        case QMediaPlaylist::Loop:
            localPlaybackMode = QMediaPlaylist::CurrentItemInLoop;//列表循环到单曲循环
            break;
        case QMediaPlaylist::CurrentItemInLoop:
            localPlaybackMode = QMediaPlaylist::Random;
            break;
        }
        playlist->setPlaybackMode(localPlaybackMode);
        updatePlayModeButton();
        qDebug() << "本地模式切换至：" << localPlaybackMode; // 调试输出
    } else if (currentList == onlinePlaylist) {
        // 在线列表模式切换
        switch (onlinePlaybackMode) {
        case QMediaPlaylist::CurrentItemInLoop:
            onlinePlaybackMode = QMediaPlaylist::Sequential;
            break;
        case QMediaPlaylist::Sequential:
            onlinePlaybackMode = QMediaPlaylist::Loop;
            break;
        case QMediaPlaylist::Loop:
            onlinePlaybackMode = QMediaPlaylist::Random;
            break;
        case QMediaPlaylist::Random:
            onlinePlaybackMode = QMediaPlaylist::CurrentItemInLoop;
            break;
        default:
            onlinePlaybackMode = QMediaPlaylist::CurrentItemInLoop;
        }
        onlinePlaylist->setPlaybackMode(onlinePlaybackMode);
    }

}
//更新模式按钮图标
void MusicWindow::updatePlayModeButton()
{
    QMediaPlaylist *currentList = qobject_cast<QMediaPlaylist*>(player->playlist());
    QMediaPlaylist::PlaybackMode currentMode;

    if (currentList == playlist) {
        currentMode = localPlaybackMode;
    } else {
        currentMode = onlinePlaybackMode;
    }

    QString iconPath;
    switch (currentMode) {
    case QMediaPlaylist::Random:
        iconPath = ":/image/player/mode_random.svg";
        break;
    case QMediaPlaylist::Loop:
        iconPath = ":/image/player/mode_loop.svg";
        break;
    case QMediaPlaylist::Sequential:
        iconPath = ":/image/player/mode_sequential.svg";
        break;
    case QMediaPlaylist::CurrentItemInLoop:
        iconPath = ":/image/player/mode_single.svg";
        break;
    default:
        iconPath = ":/image/player/mode_sequential.svg";
    }
    ui->btn_mode->setStyleSheet(QString("border-image: url(%1);").arg(iconPath));
}

