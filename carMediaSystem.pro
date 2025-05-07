QT       += core gui network
QT +=quick quickwidgets  multimedia sql  multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += -lm
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    exitfullscreeneventfilter.cpp \
    labelclickeventfilter.cpp \
    main.cpp \
    mainwindow.cpp \
    maplabel.cpp \
    mapwindow.cpp \
    musicwindow.cpp \
    speedsensor.cpp \
    videowindow.cpp \
    weatherwindow.cpp

HEADERS += \
    common.h \
    exitfullscreeneventfilter.h \
    labelclickeventfilter.h \
    mainwindow.h \
    maplabel.h \
    mapwindow.h \
    musicwindow.h \
    speedsensor.h \
    videowindow.h \
    weatherwindow.h

FORMS += \
    mainwindow.ui \
    mapwindow.ui \
    musicwindow.ui \
    videowindow.ui \
    weatherwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    image/map.svg \
    image/天气预报.svg \
    image/导航.svg \
    image/视频.svg \
    image/音乐.svg

SUBDIRS += \
    ../music/music.pro \
    ../music/music.pro
