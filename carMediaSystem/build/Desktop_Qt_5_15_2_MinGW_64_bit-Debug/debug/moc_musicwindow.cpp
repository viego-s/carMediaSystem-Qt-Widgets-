/****************************************************************************
** Meta object code from reading C++ file 'musicwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../musicwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'musicwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MusicWindow_t {
    QByteArrayData data[16];
    char stringdata0[284];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MusicWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MusicWindow_t qt_meta_stringdata_MusicWindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MusicWindow"
QT_MOC_LITERAL(1, 12, 19), // "on_btn_quit_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 19), // "on_btn_play_clicked"
QT_MOC_LITERAL(4, 53, 19), // "on_btn_file_clicked"
QT_MOC_LITERAL(5, 73, 19), // "on_btn_prev_clicked"
QT_MOC_LITERAL(6, 93, 19), // "on_btn_next_clicked"
QT_MOC_LITERAL(7, 113, 31), // "on_list_local_itemDoubleClicked"
QT_MOC_LITERAL(8, 145, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(9, 162, 4), // "item"
QT_MOC_LITERAL(10, 167, 15), // "durationChanged"
QT_MOC_LITERAL(11, 183, 8), // "duration"
QT_MOC_LITERAL(12, 192, 15), // "positionChanged"
QT_MOC_LITERAL(13, 208, 8), // "position"
QT_MOC_LITERAL(14, 217, 35), // "on_play_progress_bar_sliderRe..."
QT_MOC_LITERAL(15, 253, 30) // "on_slider_voice_sliderReleased"

    },
    "MusicWindow\0on_btn_quit_clicked\0\0"
    "on_btn_play_clicked\0on_btn_file_clicked\0"
    "on_btn_prev_clicked\0on_btn_next_clicked\0"
    "on_list_local_itemDoubleClicked\0"
    "QListWidgetItem*\0item\0durationChanged\0"
    "duration\0positionChanged\0position\0"
    "on_play_progress_bar_sliderReleased\0"
    "on_slider_voice_sliderReleased"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MusicWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    1,   69,    2, 0x08 /* Private */,
      10,    1,   72,    2, 0x08 /* Private */,
      12,    1,   75,    2, 0x08 /* Private */,
      14,    0,   78,    2, 0x08 /* Private */,
      15,    0,   79,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::LongLong,   11,
    QMetaType::Void, QMetaType::LongLong,   13,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MusicWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MusicWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btn_quit_clicked(); break;
        case 1: _t->on_btn_play_clicked(); break;
        case 2: _t->on_btn_file_clicked(); break;
        case 3: _t->on_btn_prev_clicked(); break;
        case 4: _t->on_btn_next_clicked(); break;
        case 5: _t->on_list_local_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->durationChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 7: _t->positionChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 8: _t->on_play_progress_bar_sliderReleased(); break;
        case 9: _t->on_slider_voice_sliderReleased(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MusicWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MusicWindow.data,
    qt_meta_data_MusicWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MusicWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MusicWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MusicWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MusicWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
