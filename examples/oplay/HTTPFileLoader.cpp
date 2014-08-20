#include "HTTPFileLoader.h"
#include <QStyleOption>
#include <QPainter>
#include <QtNetwork>

HTTPFileLoader::HTTPFileLoader(QString savePath)
    : m_savePath(savePath)
    , m_ok(false)
{
}

void HTTPFileLoader::httpReadyRead(void)
{
    qDebug("httpReadyRead : ");
    if (m_file)
        m_file->write(m_reply->readAll());
}

void HTTPFileLoader::httpFinished(void)
{
    qDebug("httpFinished : ");

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
        m_ok = true;
        qDebug("http request success :");
        qDebug(m_reply->url().toString().toStdString().c_str());
    }
    m_reply->deleteLater();
    m_reply = NULL;
    delete m_file;
    m_file = NULL;
}

void HTTPFileLoader::getHTTPFile(QString name)
{
    QUrl url(name);
    m_ok = false;

    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();
    if (fileName.isEmpty())
        fileName = "index.html";

    if (QFile::exists(fileName)) 
    {
        QFile::remove(fileName);
    }

    m_file = new QFile(QString("%1/%2").arg(m_savePath, fileName));
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

HTTPFileLoader::~HTTPFileLoader(void)
{
}
