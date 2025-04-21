#include "musicwindow.h"
#include "ui_musicwindow.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
MusicWindow::MusicWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MusicWindow)
{
    ui->setupUi(this);
    ui->btn_play->setProperty("mode","pause");
    this->player=new QMediaPlayer;
    this->playlist=new QMediaPlaylist;
    this->player->setPlaylist(this->playlist);
    //修改播放模式为随机
    this->playlist->setPlaybackMode(QMediaPlaylist::Random);
    //给QMediaPlayer绑定信号与槽,去计算进度条
    connect(this->player,&QMediaPlayer::durationChanged,this,&MusicWindow::durationChanged);
    connect(this->player,&QMediaPlayer::positionChanged,this,&MusicWindow::positionChanged);
    // 添加歌词模型初始化
    lyricModel = new QStringListModel(this);
    ui->lyricView->setModel(lyricModel);
    ui->lyricView->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

MusicWindow::~MusicWindow()
{
    delete ui;
}


void MusicWindow::on_btn_quit_clicked()
{
    this->hide();
    this->parentWidget()->show();
}
//歌词解析函数
void MusicWindow::parseLyrics(const QString &musicPath)
{
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
    for(QString path:files){
        //将选中的文件放入媒体播放列表
        this->playlist->addMedia(QMediaContent(QUrl::fromLocalFile(path)));
        QFileInfo file(path);
        ui->list_local->addItem(file.fileName());
        // 自动加载歌词文件
        parseLyrics(path);
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

