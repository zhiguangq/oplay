#include "BackgroundWidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QtNetwork>

BackgroundWidget::BackgroundWidget(QString tempPath, QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
    , m_mainWidget(parent)
    , m_loadPictureIndex(-1)
    , m_tempPath(tempPath)
{
    m_lableLayout = new QGridLayout;

    for(int i = 0; i < MAX_VIEW_PORT; i++)
    {

        m_labelVector[i] = new DramaLabel(*this, i);
        m_lableLayout->addWidget(m_labelVector[i], i / (int)sqrt((double)MAX_VIEW_PORT), i % (int)(sqrt((double)MAX_VIEW_PORT)));

        if(i < 36 || i >= 41)
        {
            m_labelVector[i]->setAttribute(Qt::WA_TranslucentBackground, true);
        }
        else
        {
            QString sheet = "QLabel {border-image: url(:/APlayer/Resources/background-picture.png);}";
            m_labelVector[i]->setStyleSheet(sheet);
        }

    }

    this->setLayout(m_lableLayout);
}

void BackgroundWidget::loadPlaylist(QString recommendPlaylistUrl, QString currentPlaylistUrl, int index)
{
    getHTTPFile(currentPlaylistUrl);
    m_recommendPlaylistUrl = recommendPlaylistUrl;
    m_defaultPlayIndex  = index;
}

void BackgroundWidget::httpReadyRead(void)
{
    if (m_file)
        m_file->write(m_reply->readAll());
}

void BackgroundWidget::httpFinished(void)
{
    QString finishFileName = m_file->fileName();

    m_file->flush();
    m_file->close();

    QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (m_reply->error()) 
    {
        qDebug("http request error :");
        qDebug(m_reply->url().toString().toStdString().c_str());
        qDebug(m_reply->errorString().toStdString().c_str());
        m_file->remove();
    } 
    else if (!redirectionTarget.isNull()) 
    {
        // TODO redirection
    }
    else
    {
        qDebug("http request success :");
        qDebug(m_reply->url().toString().toStdString().c_str());
    }
    m_reply->deleteLater();
    m_reply = NULL;
    delete m_file;
    m_file = NULL;

    if(finishFileName.endsWith("xml"))
    {
        if(m_loadPictureIndex == -1)
        {
            // 先把当前要播放的playlist加载进来
            m_loadPictureIndex = 0;
            m_currentPlaylist.reload(finishFileName);
            emit onPlaylistLoad(&m_currentPlaylist, m_defaultPlayIndex);

            getHTTPFile(m_recommendPlaylistUrl);
        }
        else
        {
            m_recommendPlaylist.reload(finishFileName);
            emit onPlaylistLoad(&m_recommendPlaylist, -1);

            PlayList::VideoNode* node = m_recommendPlaylist.getNode(m_loadPictureIndex);
            if(node)
            {
                getHTTPFile(node->pictureUrl);
            }
        }
    }
    else
    {
        PlayList::VideoNode* node = m_recommendPlaylist.getNode(m_loadPictureIndex);
        if(node)
        {
            QFileInfo fileInfo(node->pictureUrl);
            QString fileName = fileInfo.fileName();
            if (QFile::exists(QString("%1/%2").arg(m_tempPath, fileName)))
            {
                qDebug("set Qlabel image: %s",QString("%1/%2").arg(m_tempPath, fileName).toStdString().c_str());
                QString sheet = QString(QString("QLabel {border-image: url(%1/%2);}").arg(m_tempPath, fileName));
                m_labelVector[36 + m_loadPictureIndex]->setStyleSheet(sheet);
            }
        }
        if(++m_loadPictureIndex < 5)
        {
            PlayList::VideoNode* node = m_recommendPlaylist.getNode(m_loadPictureIndex);
            if(node)
            {
                getHTTPFile(node->pictureUrl);
            }
        }
    }
}

void BackgroundWidget::getHTTPFile(QString name)
{
    QUrl url(name);

    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();
    if (fileName.isEmpty())
        fileName = "index.html";

    if (QFile::exists(fileName)) 
    {
        QFile::remove(fileName);
    }

    m_file = new QFile(QString("%1/%2").arg(m_tempPath, fileName));
    if (!m_file->open(QIODevice::WriteOnly)) 
    {
        qDebug("can not open file : ");
        qDebug(m_file->fileName().toStdString().c_str());
        delete m_file;
        m_file = 0;
        return;
    }
    qDebug("download file to local  file : ");
    qDebug(m_file->fileName().toStdString().c_str());

    qDebug("send http request : ");
    qDebug(name.toStdString().c_str());
    m_reply = m_qnam.get(QNetworkRequest(url));
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
    connect(m_reply, SIGNAL(finished()), this, SLOT(httpFinished()));    
}

void BackgroundWidget::dramaLabelClick(int index)
{
    //emit onProgramClick(m_recommendPlaylist.getNode(index), 1);
}

BackgroundWidget::~BackgroundWidget(void)
{
    delete m_lableLayout;
}

void BackgroundWidget::paintEvent(QPaintEvent *) 
{ 
    QStyleOption opt; 
    opt.init(this); 
    QPainter p(this); 
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
} 

void BackgroundWidget::locationSizeChanged(bool isFullScreen, QPoint pos, QSize size)
{
    int x, y, width, height;
    double deltX = size.width() / 9;
    double deltY = size.height() / 9;
    
    if(isFullScreen)
    {
        x = pos.x();
        y = pos.y();
    }
    else
    {
        x = pos.x() + 8;
        y = pos.y() + 30;
    }

    x += (int)(deltX * 2);
    y += (int)(deltY * 4);
    
    width = (int) (deltX * 5);
    height = (int) (deltY * 3);

    this->resize(width, height);
    this->move(x, y);
}
