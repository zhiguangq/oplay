#ifndef BASEHEADER_H
#define BASEHEADER_H
#include <QDir>
#include <QDesktopServices>

class BaseHeader
{
public:
static QString getTempPath(void)
{
    QString m_tempPath;
    m_tempPath = QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/oplay";
    QDir *tempDir = new QDir(m_tempPath);
    if(!tempDir->exists())
    {
        tempDir->mkdir(m_tempPath);
    }
    else
    {
        /*
        QStringList filter;
        QList<QFileInfo> *fileInfo = new QList<QFileInfo>(tempDir->entryInfoList(filter));
        int count = fileInfo->count();
        for(int i = 0; i < count; i++)
            tempDir->remove(fileInfo->at(i).filePath());
         */
    }
    return m_tempPath;
}
};

#endif // BASEHEADER_H
