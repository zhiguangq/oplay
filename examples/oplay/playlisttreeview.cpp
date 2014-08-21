#include "playlisttreeview.h"

#include <QMouseEvent>
#include <QtGui>

PlaylistTreeView::PlaylistTreeView(QWidget *parent)
    : QTreeView(parent)
    , m_playingIndex(0)
{
    this->adjustSize();

    m_model = new QStandardItemModel();
    this->setModel(m_model);

    QStandardItem *item = new QStandardItem("oplay");
    m_model->setHorizontalHeaderItem(0, item);
}

void PlaylistTreeView::addItem(QString itemName, QString value)
{
    QStandardItem *item = new QStandardItem(itemName);
    item->setData(value);
    m_model->appendRow(item);
}

void PlaylistTreeView::addItemChildren(QString parameterItemName, QString itemName, QString value)
{
    QStandardItem *item = new QStandardItem(itemName);
    item->setData(value);
    m_model->findItems(parameterItemName).front()->appendRow(item);
}

PlaylistTreeView::~PlaylistTreeView()
{
    delete m_model;
}

// item是一个结点
// index 默认是0
// 1如果这个结点下面有字结点、就要找到它的了结点，从index开始播放
// 2如果这个结点已经是子结点、（电影为结点，每集电视剧为了结点）
// 3如果1,2都不是、直播它
ItemInfo PlaylistTreeView::getOneMovie(QStandardItem* item, int index)
{
    QStandardItem* parentItem = item;
    int         playIndex = index;
    m_playingIndex = 0;

    if(item->rowCount() > 0)
    {
        // 它下面有多个子结点
        parentItem = item;
        playIndex = index;
    }
    else if(item->text().toInt() != 0)
    {
        //它是多个子结点中的一个
        parentItem = item->parent();
        playIndex = item->text().toInt() - 1;
    }
    else
    {
        // 它下本身只有自己
        m_playListVector.clear();
        ItemInfo ret(item->text(), item->data().toString());
        return ret;
    }

    //找出所有结点
    m_playListVector.clear();
    for(int i = 0; i < parentItem->rowCount(); i++)
    {
        m_playListVector.push_back(ItemInfo(parentItem->text() + "(" + parentItem->child(i)->text() + ")", \
                                            parentItem->child(i)->data().toString()));
        qDebug("new playist size[%d]  %s ", m_playListVector.size(), m_playListVector[i].m_url.toStdString().c_str());
    }

    // 找出它的子结点， 如果不在范围内从第一个开始

    m_playingIndex = (playIndex >= 0 && playIndex < parentItem->rowCount()) ? playIndex : 0;
    ItemInfo ret = m_playListVector[m_playingIndex];

    return ret;
}

void PlaylistTreeView::onPlaylistLoad(PlayList* list, int playIndex)
{
    for(int i = 0; i < list->size(); i++)
    {
        PlayList::VideoNode* node = list->getNode(i);
        if(node->url.size() == 1)
        {
            this->addItem(node->name, node->url.front());
            node->url.pop_front();
        }
        else if(node->url.size() > 1)
        {
            this->addItem(node->name);
            int j = 1;
            while(!node->url.empty())
            {
                this->addItemChildren(node->name, QString("%1").arg(j++), node->url.front());
                node->url.pop_front();
            }
        }
    }

    //加载两个playlist后,recommend这个playlist选择一个默认的开始播放
    if(playIndex != -1)
    {
        // 0这个结点是recommend
        ItemInfo ret = getOneMovie(m_model->item(0), playIndex - 1);
        m_model->horizontalHeaderItem(0)->setText(ret.m_name);
        emit onPlay(ret.m_url);
    }
}

// 取下一个播放URL
void PlaylistTreeView::onNextUrl(void)
{
    if((m_playingIndex + 1) > 0 && (m_playingIndex + 1) < m_playListVector.size())
    {
        m_playingIndex++;
        ItemInfo ret = m_playListVector[m_playingIndex];
        m_model->horizontalHeaderItem(0)->setText(ret.m_name);
        emit onPlay(ret.m_url);
    }
}

// 取上一个播放URL
void PlaylistTreeView::onPreviousUrl(void)
{
    if((m_playingIndex - 1) >= 0 && (m_playingIndex - 1) < m_playListVector.size())
    {
        m_playingIndex--;
        ItemInfo ret = m_playListVector[m_playingIndex];
        m_model->horizontalHeaderItem(0)->setText(ret.m_name);
        emit onPlay(ret.m_url);
    }
}

void PlaylistTreeView::onBackgroundPictureClick(int index)
{
    ItemInfo ret = getOneMovie(m_model->item(index));
    m_model->horizontalHeaderItem(0)->setText(ret.m_name);

    qDebug("background click : %s",m_model->item(index)->text().toStdString().c_str());
    qDebug("background click : %s",m_model->item(index)->data().toString().toStdString().c_str());

	emit onPlay(ret.m_url);
}

void PlaylistTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QModelIndex index = currentIndex();
        if(index.isValid())
        {
            ItemInfo ret = getOneMovie(m_model->itemFromIndex(index));
            m_model->horizontalHeaderItem(0)->setText(ret.m_name);
            emit onPlay(ret.m_url);
            qDebug("click : %s",m_model->itemFromIndex(index)->text().toStdString().c_str());
            qDebug("click : %s",m_model->itemFromIndex(index)->data().toString().toStdString().c_str());
        }
    }
}

