#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QUrl>
#include <QFile>
#include <QNetworkAccessManager>
#include "PlayList.h"
#include "DramaLabel.h"

#define MAX_VIEW_PORT 49

class BackgroundWidget : public QWidget
{
    Q_OBJECT
public:
    BackgroundWidget(QString tempPath, QWidget *parent = 0, Qt::WFlags flags = 0);
    ~BackgroundWidget(void);

    void loadPlaylist(QString recommendPlaylistUrl, QString currentPlaylistUrl, int index);

Q_SIGNALS:
    void onPlaylistLoad(PlayList* list, int PlayIndex);
private slots:
    void httpReadyRead(void);
    void httpFinished(void);
    void dramaLabelClick(int index);
    void locationSizeChanged(bool isFullScreen, QPoint pos, QSize size);
private:
    void paintEvent(QPaintEvent *);
    void getHTTPFile(QString url);
private:
    PlayList                m_currentPlaylist;
    PlayList                m_recommendPlaylist;
    QNetworkAccessManager   m_qnam;
    QNetworkReply           *m_reply;
    int                     m_loadPictureIndex;
    QFile                   *m_file;
    DramaLabel              *m_labelVector[MAX_VIEW_PORT];
    QGridLayout             *m_lableLayout;
    QWidget                 *m_backgroundWidget;
    QWidget                 *m_mainWidget;
    QString                 m_recommendPlaylistUrl;
    QString                 m_tempPath;
    int                     m_defaultPlayIndex;
};

#endif // BACKGROUNDWIDGET_H

