#include "exitfullscreeneventfilter.h"
#include <QKeyEvent>
ExitFullScreenEventFilter::ExitFullScreenEventFilter(QObject *parent)
    : QObject{parent}
{}
//要捕获esc按键，或者捕获双击事件，如果捕获到，就要退出全屏
bool ExitFullScreenEventFilter::eventFilter(QObject *watched, QEvent *event)
{   QKeyEvent *keyEvent=static_cast<QKeyEvent*>(event);
    //捕获双击事件
    if(event->type()==QEvent::MouseButtonDblClick){
        emit exitFullScreen();
    }
    if(keyEvent->key()==Qt::Key_Escape){
        emit exitFullScreen();
    }
}
