#ifndef EXITFULLSCREENEVENTFILTER_H
#define EXITFULLSCREENEVENTFILTER_H

#include <QObject>
#include <QEvent>
class ExitFullScreenEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit ExitFullScreenEventFilter(QObject *parent = nullptr);
    bool eventFilter(QObject *watched,QEvent *event);

signals:
    void exitFullScreen();
};

#endif // EXITFULLSCREENEVENTFILTER_H
