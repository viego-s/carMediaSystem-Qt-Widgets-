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

private:
    Ui::MusicWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    int total;
    QMap<qint64, QString> lyrics;
    QStringListModel *lyricModel;
    void parseLyrics(const QString &musicPath);//歌词分析
    void setLyricsBackground(const QString &musicPath);//封面
    QLabel *label_background;

    //数据库
    void initDatabase();        // 初始化数据库
    void loadFromDatabase();    // 从数据库加载音乐
    void addToDatabase(const QString &path); // 添加到数据库
    void deleteFromDatabase(int index); // 从数据库删除
    QSqlDatabase db;            // 数据库连接

};

#endif // MUSICWINDOW_H
