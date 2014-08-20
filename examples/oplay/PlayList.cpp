#include "PlayList.h"
#include <QFile>
#include <QDomDocument>
#include <QXmlStreamReader>

PlayList::PlayList(void)
{
}

PlayList::~PlayList(void)
{
}

bool PlayList::reload(QString name)
{
    QDomDocument dom;
    QFile playlistFile(name);
    if (playlistFile.open(QIODevice::ReadOnly))
    {
        dom.setContent(&playlistFile); 
        
        // 此处需做错误判断
    }
    else
    {
        return false;
    }
    playlistFile.close();

    // 以下代码为解析XML文件中的内容
    QDomNodeList videos = dom.elementsByTagName("video");
    for (int i = 0; i < videos.count(); i++) 
    {
        QDomNode video = videos.item(i);
        QDomNodeList attribs = video.toElement().childNodes();

        VideoNode   videoNode;
        for (int j = 0; j < attribs.count(); j++) 
        {
            QDomNode node = attribs.item(j);
            QString tagName = node.toElement().tagName();

            if(tagName == QObject::tr("type"))
            {
                videoNode.type = node.toElement().text();
            }
            else if(tagName == QObject::tr("name"))
            {
                videoNode.name = node.toElement().text();
            }
            else if(tagName == QObject::tr("pictureUrl"))
            {
                videoNode.pictureUrl = node.toElement().text();
            }
            else if(tagName == QObject::tr("url"))
            {
                QString url = node.toElement().text();
                videoNode.url.push_back(url);
            }
        }

        m_nodeVector.push_back(videoNode);
    }

    return true;
}


PlayList::VideoNode* PlayList::getNode(int index)
{
    if(index >= m_nodeVector.size())
        return NULL;

    return &m_nodeVector[index];
}

int PlayList::size(void)
{
    return m_nodeVector.size();
}
