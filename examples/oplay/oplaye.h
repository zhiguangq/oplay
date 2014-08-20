#ifndef OPLAYE_H
#define OPLAYE_H

#include <QtGui/QWidget>
#include <QtGui/QMainWindow>
#include <QSplitter> 
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>
#include <QEvent>
#include <QStackedWidget>
#include <QListWidget>
#include <QtAV/GLWidgetRenderer.h>
#include <QtAV/WidgetRenderer.h>
#include <QtAV/AVPlayer.h>
#include "PlayList.h"
#include "BottonDockWidget.h"
#include "playlisttreeview.h"
#include "BackgroundWidget.h"

class APlayer : public QMainWindow
{
    Q_OBJECT

public:
    APlayer(BackgroundWidget* back, QWidget *parent = 0, Qt::WFlags flags = 0);
    ~APlayer();

    void setPlayListURL(QString url);
Q_SIGNALS:
    void updateStatus(int currentTime, int duration);
private slots:
    void onSplitterMoved(int,int);
    void onButtonClick();
    void intervalTimerExpired();

    void onPlay(QString url);
    void onStop(void);
    void onAutoStop(void);
    void onFullscreen(void);
	void onPositionChange(qint64 pos);
private:
    void showEvent(QShowEvent *e);
    void setButtonIcon(void);
    virtual void resizeEvent(QResizeEvent *event);
private:
    QSplitter*   m_pMainSplitter;  
    QSplitter*   m_pSplitter;
    PlaylistTreeView    *m_playList;
    QPushButton* m_pButton;
    QTimer*      m_intervalTimer;
    QStackedWidget*          m_stackedWidget;
    QtAV::WidgetRenderer* m_renderer;
    QtAV::AVPlayer*         m_player;
    BottonDockWidget            *m_bottomDockWidget;
    int                 m_seekTime;
    BackgroundWidget*   m_backgroundWidget;
};

#endif // OPLAYE_H
