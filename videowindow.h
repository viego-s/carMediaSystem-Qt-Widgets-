#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>
#include <QVideoWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QNetworkReply>
#include "exitfullscreeneventfilter.h"
namespace Ui {
class videoWindow;
}

class videoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit videoWindow(QWidget *parent = nullptr);
    ~videoWindow();
public slots:
    void onSpeedChanged(double speed); // 处理车速变化的槽函数
private slots:
    void on_btn_quit_clicked();

    void on_btn_play_clicked();

    void on_btn_file_clicked();

    void on_btn_prev_clicked();

    void on_btn_next_clicked();

    void on_list_local_3_itemDoubleClicked(QListWidgetItem *item);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);

    void on_play_progress_bar_sliderReleased();

    void on_btn_fullscreen_clicked();
    void exitFullScreen();


    void on_list_online_3_itemDoubleClicked(QListWidgetItem *item);
    void showOnlineList(QNetworkReply *reply);
    void on_btn_del_clicked();

private:
    Ui::videoWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    int total;
    QVideoWidget *videowidget;
    ExitFullScreenEventFilter *eventFilter;
    double speedThreshold = 120.0;

    QMediaPlaylist *onlinePlaylist; // 在线播放列表
    QSqlDatabase db;
    QMap<QString, QString> onlineCoverMap; // 在线视频封面映射
    //数据库操作
    void initDatabase();
    void loadFromDatabase();
    void addToDatabase(const QString &path);
    void deleteFromDatabase(int index);
    //在线列表
    void initOnlineList();

    QMediaPlaylist::PlaybackMode localPlaybackMode;  // 本地列表播放模式
    QMediaPlaylist::PlaybackMode onlinePlaybackMode; // 在线列表播放模式

};

#endif // VIDEOWINDOW_H
