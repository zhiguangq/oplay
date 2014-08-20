TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = oplay

INCLUDEPATH += C:/Qt/4.8.5/include/QtXml
INCLUDEPATH += C:/Qt/4.8.5/include/QtNetwork

LIBS += -L$$PWD/C:/Qt/4.8.5/lib/ -lQtXmld4
LIBS += -L$$PWD/C:/Qt/4.8.5/lib/ -lQtNetworkd4

STATICLINK = 0
PROJECTROOT = $$PWD/../..
include($$PROJECTROOT/src/libQtAV.pri)
preparePaths($$OUT_PWD/../../out)

SOURCES += main.cpp \
    PlayList.cpp \
    oplaye.cpp \
    BottonDockWidget.cpp \
    HTTPFileLoader.cpp \
    playlisttreeview.cpp \
    BackgroundWidget.cpp \
    DramaLabel.cpp
HEADERS += \ 
    PlayList.h \
    oplaye.h \
    BottonDockWidget.h \
    HTTPFileLoader.h \
    BaseHeader.h \
    playlisttreeview.h \
    BackgroundWidget.h \
    DramaLabel.h

RESOURCES += \
    oplay.qrc

RC_FILE = ico.rc

FORMS +=



