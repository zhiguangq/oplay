#ifndef PLAYLISTTREEVIEW_H
#define PLAYLISTTREEVIEW_H

#include <QTreeView>
#include <QtGui>
#include "PlayList.h"

class ItemInfo
{
public:
    ItemInfo(QString name, QString url)
        : m_name(name)
        , m_url(url)
    {
    }
    ItemInfo(const ItemInfo &item)
    {
        this->m_name = item.m_name;
        this->m_url = item.m_url;
    }

    ItemInfo & operator = (const ItemInfo &item)
    {
        this->m_name = item.m_name;
        this->m_url = item.m_url;
        return *this;
    }
    ItemInfo(){}

    QString m_name;
    QString m_url;
};

class PlaylistTreeView : public QTreeView
{
    Q_OBJECT
public:
    PlaylistTreeView(QWidget *parent = 0);
    ~PlaylistTreeView();

    void addItem(QString itemName, QString value = "");
    void addItemChildren(QString parameterItemName, QString itemName, QString value = "");

    // item是一个结点
    // index 默认是0
    // 1如果这个结点下面有字结点、就要找到它的了结点，从index开始播放
    // 2如果这个结点已经是子结点、（电影为结点，每集电视剧为了结点）
    // 3如果1,2都不是、直播它
    ItemInfo getOneMovie(QStandardItem* item, int index = 0);

Q_SIGNALS:
    void onPlay(QString url);
public slots:
    void onPlaylistLoad(PlayList* list, int PlayIndex);
    void onNextUrl(void);
    void onPreviousUrl(void);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    QStandardItemModel *m_model;
    QVector<ItemInfo>   m_playListVector;
    int                 m_playingIndex;    // 相对于本剧集
};

#endif // PLAYLISTTREEVIEW_H
