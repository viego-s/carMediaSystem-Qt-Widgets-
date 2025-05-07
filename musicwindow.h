#ifndef MUSICWINDOW_H
#define MUSICWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QPixmap>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QNetworkReply>
namespace Ui {
class MusicWindow;
}

class MusicWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MusicWindow(QWidget *parent = nullptr);
    ~MusicWindow();
    void initOnlineList();
private slots:
    void on_btn_quit_clicked();

    void on_btn_play_clicked();

    void on_btn_file_clicked();

    void on_btn_prev_clicked();

    void on_btn_next_clicked();

    void on_list_local_itemDoubleClicked(QListWidgetItem *item);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);


    void on_play_progress_bar_sliderReleased();

    void on_slider_voice_sliderReleased();

    void on_btn_del_clicked();
    void showOnlineList(QNetworkReply *reply);

    void on_list_online_itemDoubleClicked(QListWidgetItem *item);

    void on_btn_mode_clicked();//模式切换按钮点击事件

private:
    Ui::MusicWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;//本地列表
    QMediaPlaylist *onlinePlaylist;//在线列表

    QMediaPlaylist::PlaybackMode localPlaybackMode;  // 本地列表播放模式
    QMediaPlaylist::PlaybackMode onlinePlaybackMode; // 在线列表播放模式
    void updatePlayModeButton(); // 更新模式按钮图标
    void updateLyricsAndCover();

    int total;
    QList<QPair<qint64, QString>> lyrics;
    QNetworkReply* currentLyricReply = nullptr; // 跟踪当前歌词请求
    QNetworkReply* currentCoverReply = nullptr;//跟踪当前背景请求
    QStringListModel *lyricModel;
    void parseLyrics(const QString &musicPathOrUrl);//歌词分析
    void setLyricsBackground(const QString &coverPathOrUrl);//封面
    QLabel *label_background;

    //数据库
    void initDatabase();        // 初始化数据库
    void loadFromDatabase();    // 从数据库加载音乐
    void addToDatabase(const QString &path); // 添加到数据库
    void deleteFromDatabase(int index); // 从数据库删除
    QSqlDatabase db;            // 数据库连接

    //在线列表
    QMap<QString, QString> onlineLyricMap; // 存储在线歌曲歌词路径 <音乐路径, 歌词URL>
    QMap<QString, QString> onlineCoverMap; // 存储在线歌曲封面路径 <音乐路径, 封面URL>

};

#endif // MUSICWINDOW_H
