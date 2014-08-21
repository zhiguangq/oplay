#include "oplaye.h"
#include "BaseHeader.h"
#include <QApplication>
#include <QUrl>

APlayer::APlayer(BackgroundWidget* back, QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
    , m_intervalTimer(new QTimer(this))
    , m_backgroundWidget(back)
{
    this->setFixedSize(960,576);

    m_stackedWidget = new QStackedWidget();
    m_player = new QtAV::AVPlayer(this);
    m_renderer = new QtAV::WidgetRenderer();
    m_player->setRenderer(m_renderer);

    m_pMainSplitter = new QSplitter(Qt::Vertical, this);
    m_pMainSplitter->setFixedSize(this->size());

    m_pSplitter = new QSplitter(Qt::Horizontal,m_pMainSplitter);
    m_playList = new PlaylistTreeView();

    connect(m_playList,SIGNAL(onPlay(QString)),this,SLOT(onPlay(QString)));
	connect(m_backgroundWidget, SIGNAL(onBackgroundPictureClick(int)), m_playList, SLOT(onBackgroundPictureClick(int)));

    connect(m_backgroundWidget, SIGNAL(onPlaylistLoad(PlayList*, int)), \
            this->m_playList, SLOT(onPlaylistLoad(PlayList*, int)));
    QString sheet = QString("QWidget {border-image: url(:/APlayer/Resources/background-picture.png); }");
    m_backgroundWidget->setStyleSheet(sheet);
    m_stackedWidget->addWidget(m_renderer);
    m_stackedWidget->addWidget(m_backgroundWidget);
    m_stackedWidget->setCurrentIndex(0);

    m_pSplitter->addWidget(m_stackedWidget);
    m_pSplitter->addWidget(m_playList);
    m_pSplitter->handle(1)->installEventFilter(this);
    m_pSplitter->setHandleWidth(1);
    connect(m_pSplitter,SIGNAL(splitterMoved(int,int)),this,SLOT(onSplitterMoved(int,int)));

    m_pButton = new QPushButton(this);
    m_pButton->setVisible(false);
    m_pButton->setIcon(QIcon(":/APlayer/Resources/right.bmp"));
    m_pButton->setFixedSize(13,42);
    m_pButton->setIconSize(QSize(13,42));
    m_pButton->setStyleSheet("border:none;");
    m_pButton->installEventFilter(this);
    connect(m_pButton,SIGNAL(clicked()),this,SLOT(onButtonClick()));

    connect(m_intervalTimer, SIGNAL(timeout()), SLOT(intervalTimerExpired()));
    m_intervalTimer->start(500);

    QList<int> size;
    size.append(m_pMainSplitter->width() - 180);
    size.append(180);
    m_pSplitter->setSizes(size);
    
    // bottom Dock Widget
    m_bottomDockWidget = new BottonDockWidget(m_pMainSplitter, m_player);
    connect(this, SIGNAL(updateStatus(int, int)), m_bottomDockWidget, SLOT(updateStatus(int, int)));

    connect(m_bottomDockWidget->getStopButton(), SIGNAL(clicked()), this, SLOT(onStop()));
    connect(m_bottomDockWidget->getFullscreenButton(), SIGNAL(clicked()), this, SLOT(onFullscreen()));
    connect(m_bottomDockWidget->getPreviousButton(), SIGNAL(clicked()), this->m_playList, SLOT(onPreviousUrl()));
    connect(m_bottomDockWidget->getNextButton(), SIGNAL(clicked()), this->m_playList, SLOT(onNextUrl()));
    connect(m_player,SIGNAL(stopped()), this, SLOT(onAutoStop()));

	connect(m_player,SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChange(qint64)));

    m_pMainSplitter->setHandleWidth(1);

    QList<int> mainSize;
    mainSize.append(m_pMainSplitter->height() - 30);
    mainSize.append(30);
    m_pMainSplitter->setSizes(mainSize);
}

APlayer::~APlayer()
{

}

void APlayer::resizeEvent(QResizeEvent *event )
{
    m_pButton->setVisible(false);
    onSplitterMoved(1,1);
    m_pMainSplitter->setFixedSize(this->size());
}

void APlayer::onStop(void)
{
    m_player->stop();

    m_stackedWidget->setCurrentIndex(1);
}

void APlayer::onFullscreen(void)
{
	if(this->isFullScreen())
	{
		this->showNormal();
	}
	else
	{
		this->showFullScreen();
	}
}

void APlayer::onPlay(QString url)
{
    qDebug("onPlay %s",url.toStdString().c_str());

    if(m_player->isPlaying())
    {
        // 上一个节目停止时不要自动启动一下集
        m_player->stop();
    }

    m_player->play(url);
	m_stackedWidget->setCurrentIndex(0);
}

void APlayer::onAutoStop(void)
{
	 if(m_player->mediaStatus() == QtAV::MediaStatus::EndOfMedia)
	 {
		 // 播放到尾
		 m_playList->onNextUrl();
	 }
}

void APlayer::onPositionChange(qint64 pos)
{
	if(m_player->isPlaying())
    {
		emit updateStatus(pos / 1000,m_player->duration()/1000);
    }
}

void APlayer::intervalTimerExpired()
{
	// 如果没有播放的视频了，就显示背景
	if(m_player->mediaStatus() == QtAV::MediaStatus::EndOfMedia ||
		m_player->mediaStatus() == QtAV::MediaStatus::InvalidMedia)
	{
		m_stackedWidget->setCurrentIndex(1);
	}

    // 调整左方和下方两个控件可见
    int currentX = QWidget::mapFromGlobal(QCursor::pos()).rx();
    int currentY = QWidget::mapFromGlobal(QCursor::pos()).ry();

    static QPoint currentPos(QWidget::mapFromGlobal(QCursor::pos()));
    static int notMoveCount = 0;

    notMoveCount = (currentPos == QWidget::mapFromGlobal(QCursor::pos())) ? notMoveCount + 1 : 0;
    currentPos = QWidget::mapFromGlobal(QCursor::pos());

    if(this->isFullScreen() && notMoveCount > 10)
    {
            m_pButton->setVisible(false);
            m_bottomDockWidget->hide();
            m_pSplitter->widget(1)->hide();
    }
    else if(currentX < 0 || currentY < 0 || currentX > this->size().width() || currentY > this->size().height())
    {
        m_pButton->setVisible(false);
    }
    else
    {
        onSplitterMoved(1,1);
        m_pButton->setVisible(true);
        m_bottomDockWidget->show();
    }

    //qDebug("g-x: %d  , g-y : %d",QCursor::pos().rx(),QCursor::pos().ry());
    //qDebug("g-x: %d  , g-y : %d",this->size().width(),this->size().height());
    //qDebug("x: %d  , y : %d",currentX,currentY);
}

void APlayer::onButtonClick()
{
    if(m_playList->width() != 0)
    {   
        m_pSplitter->widget(1)->hide();
    }
    else
    {
        m_pSplitter->widget(1)->show();
    }

    m_pButton->setVisible(false);
    setButtonIcon();
    onSplitterMoved(1,1);
}

void APlayer::setButtonIcon(void)
{
    if(m_playList->width() != 0)
    {
        m_pButton->setIcon(QIcon(":/APlayer/Resources/left.bmp"));
    }
    else
    {
        m_pButton->setIcon(QIcon(":/APlayer/Resources/right.bmp"));
    }
}

void APlayer::showEvent(QShowEvent *e)
{
    onSplitterMoved(1,1);
}

void APlayer::onSplitterMoved(int pos,int index)
{    
    m_pButton->move(m_stackedWidget->width()-m_pButton->width(),(m_stackedWidget->height()-m_pButton->height())/2);
}
