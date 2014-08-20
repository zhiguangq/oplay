#include <cstdio>
#include <cstdlib>
#include <QApplication>
#include <QMessageBox>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>
#include <QTextCodec>
#include <QThread>
#include "oplaye.h"
#include "BaseHeader.h"
#include "BackgroundWidget.h"

using namespace QtAV;

static FILE *sLogfile = 0; //'log' is a function in msvc math.h

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define qInstallMessageHandler qInstallMsgHandler
void Logger(QtMsgType type, const char *msg)
{
#else
void Logger(QtMsgType type, const QMessageLogContext &, const QString& qmsg)
{
	const char* msg = qPrintable(qmsg);
#endif
    fprintf(stdout, "%s", QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz]  ").toStdString().c_str());
    if (sLogfile)
       fprintf(sLogfile, "%s", QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz]  ").toStdString().c_str());

	 switch (type) {
     case QtDebugMsg:
		 fprintf(stdout, "Debug: %s\n", msg);
         if (sLogfile)
            fprintf(sLogfile, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
		 fprintf(stdout, "Warning: %s\n", msg);
         if (sLogfile)
            fprintf(sLogfile, "Warning: %s\n", msg);
		 break;
     case QtCriticalMsg:
		 fprintf(stderr, "Critical: %s\n", msg);
         if (sLogfile)
            fprintf(sLogfile, "Critical: %s\n", msg);
		 break;
     case QtFatalMsg:
		 fprintf(stderr, "Fatal: %s\n", msg);
         if (sLogfile)
            fprintf(sLogfile, "Fatal: %s\n", msg);
		 abort();
     }
     fflush(0);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator ts;
    if (ts.load(qApp->applicationDirPath() + "/i18n/QtAV_" + QLocale::system().name()))
        a.installTranslator(&ts);
    QTranslator qtts;
    if (qtts.load("qt_" + QLocale::system().name()))
        a.installTranslator(&qtts);

    QString sPath = a.applicationDirPath();
    sPath += QString("/plugins");
    a.addLibraryPath(sPath);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));

    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    sLogfile = fopen(QString(BaseHeader::getTempPath() + "/log.txt").toUtf8().constData(), "w+");
    if (!sLogfile) {
        qWarning("Failed to open log file");
        sLogfile = stdout;
    }
    qInstallMessageHandler(Logger);

    BackgroundWidget* backgroundWidget = new BackgroundWidget(BaseHeader::getTempPath());

    qDebug("set nomal style.");
    QFile styleSheet(":/APlayer/Resources/qss/style.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
        return -1;
    }
    qApp->setStyleSheet(styleSheet.readAll());

    qDebug("argv : ");
    for(int i = 0; i < argc; i++)
    {
        qDebug(argv[i]);
    }

    QString inputURL(argv[1]);
    QString pattern("oplay://(http.*.xml)\\?(http.*.xml)\\?index=(\\d+)");
    QRegExp rx(pattern);
    int pos = inputURL.indexOf(rx);
    if(pos < 0)
    {
        qDebug("input playlist error. so exit.");
        //qDebug(playlistUrl);
        exit(-1);
    }

    backgroundWidget->loadPlaylist(rx.cap(1), rx.cap(2), rx.cap(3).toInt());

    APlayer w(backgroundWidget);
    w.setWindowTitle("oplay");
    w.show();

    return a.exec();
}
