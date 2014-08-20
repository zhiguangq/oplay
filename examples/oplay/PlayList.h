#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QString>
#include <vector>
#include <QQueue>

class PlayList
{
public:
    struct VideoNode
    {
        QString type;
        QString name;
        QString pictureUrl;
        QQueue<QString> url;
    };
public:
    PlayList(void);
    ~PlayList(void);

    bool reload(QString name);

    VideoNode* getNode(int index);
    int size(void);
private:
    std::vector<VideoNode> m_nodeVector;
};

#endif // PLAYLIST_H


