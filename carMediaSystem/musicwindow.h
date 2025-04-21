#ifndef MUSICWINDOW_H
#define MUSICWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>
#include <QStringListModel>
#include <QStyledItemDelegate>
namespace Ui {
class MusicWindow;
}

class MusicWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MusicWindow(QWidget *parent = nullptr);
    ~MusicWindow();
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

private:
    Ui::MusicWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    int total;
    QMap<qint64, QString> lyrics;
    QStringListModel *lyricModel;
    void parseLyrics(const QString &musicPath);
};

#endif // MUSICWINDOW_H
