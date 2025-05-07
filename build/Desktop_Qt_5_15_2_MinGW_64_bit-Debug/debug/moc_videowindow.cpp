/****************************************************************************
** Meta object code from reading C++ file 'videowindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../videowindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videowindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_videoWindow_t {
    QByteArrayData data[24];
    char stringdata0[407];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_videoWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_videoWindow_t qt_meta_stringdata_videoWindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "videoWindow"
QT_MOC_LITERAL(1, 12, 14), // "onSpeedChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "speed"
QT_MOC_LITERAL(4, 34, 19), // "on_btn_quit_clicked"
QT_MOC_LITERAL(5, 54, 19), // "on_btn_play_clicked"
QT_MOC_LITERAL(6, 74, 19), // "on_btn_file_clicked"
QT_MOC_LITERAL(7, 94, 19), // "on_btn_prev_clicked"
QT_MOC_LITERAL(8, 114, 19), // "on_btn_next_clicked"
QT_MOC_LITERAL(9, 134, 33), // "on_list_local_3_itemDoubleCli..."
QT_MOC_LITERAL(10, 168, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(11, 185, 4), // "item"
QT_MOC_LITERAL(12, 190, 15), // "durationChanged"
QT_MOC_LITERAL(13, 206, 8), // "duration"
QT_MOC_LITERAL(14, 215, 15), // "positionChanged"
QT_MOC_LITERAL(15, 231, 8), // "position"
QT_MOC_LITERAL(16, 240, 35), // "on_play_progress_bar_sliderRe..."
QT_MOC_LITERAL(17, 276, 25), // "on_btn_fullscreen_clicked"
QT_MOC_LITERAL(18, 302, 14), // "exitFullScreen"
QT_MOC_LITERAL(19, 317, 34), // "on_list_online_3_itemDoubleCl..."
QT_MOC_LITERAL(20, 352, 14), // "showOnlineList"
QT_MOC_LITERAL(21, 367, 14), // "QNetworkReply*"
QT_MOC_LITERAL(22, 382, 5), // "reply"
QT_MOC_LITERAL(23, 388, 18) // "on_btn_del_clicked"

    },
    "videoWindow\0onSpeedChanged\0\0speed\0"
    "on_btn_quit_clicked\0on_btn_play_clicked\0"
    "on_btn_file_clicked\0on_btn_prev_clicked\0"
    "on_btn_next_clicked\0"
    "on_list_local_3_itemDoubleClicked\0"
    "QListWidgetItem*\0item\0durationChanged\0"
    "duration\0positionChanged\0position\0"
    "on_play_progress_bar_sliderReleased\0"
    "on_btn_fullscreen_clicked\0exitFullScreen\0"
    "on_list_online_3_itemDoubleClicked\0"
    "showOnlineList\0QNetworkReply*\0reply\0"
    "on_btn_del_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_videoWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x0a /* Public */,
       4,    0,   92,    2, 0x08 /* Private */,
       5,    0,   93,    2, 0x08 /* Private */,
       6,    0,   94,    2, 0x08 /* Private */,
       7,    0,   95,    2, 0x08 /* Private */,
       8,    0,   96,    2, 0x08 /* Private */,
       9,    1,   97,    2, 0x08 /* Private */,
      12,    1,  100,    2, 0x08 /* Private */,
      14,    1,  103,    2, 0x08 /* Private */,
      16,    0,  106,    2, 0x08 /* Private */,
      17,    0,  107,    2, 0x08 /* Private */,
      18,    0,  108,    2, 0x08 /* Private */,
      19,    1,  109,    2, 0x08 /* Private */,
      20,    1,  112,    2, 0x08 /* Private */,
      23,    0,  115,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::LongLong,   13,
    QMetaType::Void, QMetaType::LongLong,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void,

       0        // eod
};

void videoWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<videoWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onSpeedChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->on_btn_quit_clicked(); break;
        case 2: _t->on_btn_play_clicked(); break;
        case 3: _t->on_btn_file_clicked(); break;
        case 4: _t->on_btn_prev_clicked(); break;
        case 5: _t->on_btn_next_clicked(); break;
        case 6: _t->on_list_local_3_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->durationChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 8: _t->positionChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 9: _t->on_play_progress_bar_sliderReleased(); break;
        case 10: _t->on_btn_fullscreen_clicked(); break;
        case 11: _t->exitFullScreen(); break;
        case 12: _t->on_list_online_3_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 13: _t->showOnlineList((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 14: _t->on_btn_del_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject videoWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_videoWindow.data,
    qt_meta_data_videoWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *videoWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *videoWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_videoWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int videoWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
