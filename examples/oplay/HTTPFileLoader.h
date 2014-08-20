#ifndef HTTPFILELOADER_H
#define HTTPFILELOADER_H

#include <QDockWidget>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QUrl>
#include <QFile>
#include <QNetworkAccessManager>

class HTTPFileLoader : public QObject
{
    Q_OBJECT
public:
    HTTPFileLoader(QString savePath);
    ~HTTPFileLoader(void);

    void getHTTPFile(QString url);
private slots:
    void httpReadyRead(void);
    void httpFinished(void);
private:
    QNetworkAccessManager   m_qnam;
    QNetworkReply           *m_reply;
    QFile                   *m_file;
    QString                 m_savePath;
public:
    bool                    m_ok;
};

#endif // HTTPFILELOADER_H

