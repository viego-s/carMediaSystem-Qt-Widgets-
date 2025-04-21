/********************************************************************************
** Form generated from reading UI file 'musicwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MUSICWINDOW_H
#define UI_MUSICWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MusicWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab_local;
    QListWidget *list_local;
    QWidget *tab_online;
    QListWidget *list_online;
    QPushButton *btn_play;
    QSlider *play_progress_bar;
    QPushButton *btn_prev;
    QPushButton *btn_next;
    QPushButton *btn_file;
    QSlider *slider_voice;
    QPushButton *btn_quit;
    QListView *lyricView;

    void setupUi(QMainWindow *MusicWindow)
    {
        if (MusicWindow->objectName().isEmpty())
            MusicWindow->setObjectName(QString::fromUtf8("MusicWindow"));
        MusicWindow->resize(800, 600);
        MusicWindow->setStyleSheet(QString::fromUtf8("centralwidget{color: rgb(255, 235, 217);}\n"
"\n"
"QPushButton#btn_play[mode='pause']{border-image:url()}"));
        centralwidget = new QWidget(MusicWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setStyleSheet(QString::fromUtf8("\n"
"QPushButton#btn_play[mode='pause']{border-image:url(:/image/player/play.svg);}\n"
"QPushButton#btn_play[mode='play']{border-image:url(:/image/player/pause.svg);}\n"
""));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 201, 481));
        tab_local = new QWidget();
        tab_local->setObjectName(QString::fromUtf8("tab_local"));
        list_local = new QListWidget(tab_local);
        list_local->setObjectName(QString::fromUtf8("list_local"));
        list_local->setGeometry(QRect(0, 0, 201, 451));
        tabWidget->addTab(tab_local, QString());
        tab_online = new QWidget();
        tab_online->setObjectName(QString::fromUtf8("tab_online"));
        list_online = new QListWidget(tab_online);
        list_online->setObjectName(QString::fromUtf8("list_online"));
        list_online->setGeometry(QRect(0, 0, 201, 451));
        tabWidget->addTab(tab_online, QString());
        btn_play = new QPushButton(centralwidget);
        btn_play->setObjectName(QString::fromUtf8("btn_play"));
        btn_play->setGeometry(QRect(370, 520, 51, 51));
        btn_play->setStyleSheet(QString::fromUtf8(""));
        play_progress_bar = new QSlider(centralwidget);
        play_progress_bar->setObjectName(QString::fromUtf8("play_progress_bar"));
        play_progress_bar->setGeometry(QRect(0, 490, 801, 22));
        play_progress_bar->setOrientation(Qt::Horizontal);
        btn_prev = new QPushButton(centralwidget);
        btn_prev->setObjectName(QString::fromUtf8("btn_prev"));
        btn_prev->setGeometry(QRect(310, 530, 31, 31));
        btn_prev->setStyleSheet(QString::fromUtf8("border-image: url(:/image/player/next.svg);"));
        btn_next = new QPushButton(centralwidget);
        btn_next->setObjectName(QString::fromUtf8("btn_next"));
        btn_next->setGeometry(QRect(440, 530, 31, 31));
        btn_next->setStyleSheet(QString::fromUtf8("border-image: url(:/image/player/prev.svg);"));
        btn_file = new QPushButton(centralwidget);
        btn_file->setObjectName(QString::fromUtf8("btn_file"));
        btn_file->setGeometry(QRect(750, 510, 31, 31));
        btn_file->setStyleSheet(QString::fromUtf8("border-image: url(:/image/player/file.svg);"));
        slider_voice = new QSlider(centralwidget);
        slider_voice->setObjectName(QString::fromUtf8("slider_voice"));
        slider_voice->setGeometry(QRect(510, 540, 160, 22));
        slider_voice->setOrientation(Qt::Horizontal);
        btn_quit = new QPushButton(centralwidget);
        btn_quit->setObjectName(QString::fromUtf8("btn_quit"));
        btn_quit->setGeometry(QRect(750, 550, 31, 31));
        btn_quit->setStyleSheet(QString::fromUtf8("border-image: url(:/image/player/back.svg);"));
        lyricView = new QListView(centralwidget);
        lyricView->setObjectName(QString::fromUtf8("lyricView"));
        lyricView->setGeometry(QRect(200, 30, 601, 451));
        lyricView->setStyleSheet(QString::fromUtf8("QListView {\n"
"        background: rgba(255, 255, 255, 0.1);\n"
"        border-radius: 12px;\n"
"        padding: 20px 0;\n"
"        font-family: \"Microsoft YaHei\";\n"
"      font-size: 16px;\n"
"        outline: none;\n"
"    }\n"
"    \n"
"    QListView::item {\n"
"        height: 50px;\n"
"qproperty-alignment: AlignCenter;\n"
"        color: black;\n"
"        background: transparent;\n"
"        border: none;\n"
"        text-align: center;\n"
"    }\n"
"    \n"
"    QListView::item:selected {\n"
"        color: #e74c3c;\n"
"        background: rgba(231, 76, 60, 0.1);\n"
"        font-size: 20px;\n"
"        font-weight: bold;\n"
"        border-radius: 8px;\n"
"    }\n"
"    \n"
"    QScrollBar:vertical {\n"
"        width: 8px;\n"
"        background: rgba(200, 200, 200, 0.1);\n"
"    }\n"
"    \n"
"    QScrollBar::handle:vertical {\n"
"        background: rgba(231, 76, 60, 0.6);\n"
"        border-radius: 4px;\n"
"    }"));
        MusicWindow->setCentralWidget(centralwidget);

        retranslateUi(MusicWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MusicWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MusicWindow)
    {
        MusicWindow->setWindowTitle(QCoreApplication::translate("MusicWindow", "\350\275\246\350\275\275\345\244\232\345\252\222\344\275\223-\351\237\263\344\271\220\346\222\255\346\224\276\345\231\250", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_local), QCoreApplication::translate("MusicWindow", "\346\234\254\345\234\260\345\210\227\350\241\250", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_online), QCoreApplication::translate("MusicWindow", "\345\234\250\347\272\277\345\210\227\350\241\250", nullptr));
        btn_play->setText(QString());
        btn_prev->setText(QString());
        btn_next->setText(QString());
        btn_file->setText(QString());
        btn_quit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MusicWindow: public Ui_MusicWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUSICWINDOW_H
