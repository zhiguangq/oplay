#include "BottonDockWidget.h"

BottonDockWidget::BottonDockWidget(QWidget *parent, QtAV::AVPlayer* player)
    : QDockWidget(parent)
    , m_duration(0)
    , m_player(player)
{
    m_bottomWidget = new QWidget;
    m_pauseResumeButton = new QPushButton();
    m_stopButton    = new QPushButton();
    m_previousButton = new QPushButton();
    m_nextButton = new QPushButton();
    m_mutedButton = new QPushButton();
    m_slider = new ProgressQSlider(Qt::Horizontal);
    m_timeLabel = new QLabel("0:00:00/0:00:00");
    m_soundSlider = new QSlider(Qt::Horizontal);
    m_fullButton = new QPushButton();

    m_slider->setRange(0, 130);

    m_soundSlider->setRange(0, 10);
    m_soundSlider->setValue(5);
	m_soundSlider->setSingleStep(1);
	m_soundSlider->setPageStep(1);

    m_timeLabel->setStyleSheet("QLabel { color: blue}");
    m_pauseResumeButton->setStyleSheet(QString("QPushButton {color: red;  border-image: url(:/APlayer/Resources/pause.png); maz-height: 20px;    max-width: 20px;  }"));
    m_stopButton->setStyleSheet(QString("QPushButton {color: red;  border-image: url(:/APlayer/Resources/stop.png); maz-height: 20px;    max-width: 15px;  }"));
    m_previousButton->setStyleSheet(QString("QPushButton {color: red;  border-image: url(:/APlayer/Resources/pre.png); maz-height: 20px;    max-width: 20px;  }"));
    m_nextButton->setStyleSheet(QString("QPushButton {color: red;  border-image: url(:/APlayer/Resources/next.png); maz-height: 20px;    max-width: 20px;  }"));
    m_mutedButton->setStyleSheet(QString("QPushButton {border-image: url(:/APlayer/Resources/unMute.png); border-width: 2px;  max-height: 15px;    max-width: 15px;}"));
    m_fullButton->setStyleSheet(QString("QPushButton {border-image: url(:/APlayer/Resources/full.png); border-width: 2px;  max-height: 15px;    max-width: 15px;}"));

    m_slider->setStyleSheet(QString(
        "QSlider::groove:horizontal {                                "
        "     border: 1px solid #999999;                             "
        "     height: 2px;                                           "
        "     margin: 0px 0;                                         "
        "     left: 12px; right: 12px;                               "
        " }                                                          "
        "QSlider::handle:horizontal {                                "
        "     border: 1px solid #5c5c5c;                             "
        " border-image:url(:/APlayer/Resources/button.png);"
        "     width: 7px;                                           "
        "     height: 5px;                                           "
        "     margin: -5px -5px -5px -5px;                           "
        " }                                                          "
        "                                                            "
        "QSlider::sub-page:horizontal{                               "
        " background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(27, 5, 27, 255), stop:0.25 rgba(99, 20, 102, 255), stop:0.5 rgba(154, 30, 158, 255), stop:1 rgba(173, 57, 176, 255));                      "
        "}                                                           "
        "QSlider::add-page:horizontal{                               "
        //" background-image:url(pics/button.png)   "
        //" background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(27, 5, 27, 255), stop:0.25 rgba(99, 20, 102, 255), stop:0.5 rgba(154, 30, 158, 255), stop:1 rgba(173, 57, 176, 255));                      "
        "}"
        ));

    m_soundSlider->setStyleSheet(QString(
        "QSlider {max-width: 100px;}"
        "QSlider::groove:horizontal {                                "
        "     border: 1px solid #999999;                             "
        "     height: 2px;                                           "
        "     margin: 0px 0;                                         "
        "     left: 12px; right: 12px;                               "
        " }                                                          "
        "QSlider::handle:horizontal {                                "
        "     border: 1px solid #5c5c5c;                             "
        " border-image:url(:/APlayer/Resources/button.png);"
        "     width: 7px;                                           "
        "     height: 5px;                                           "
        "     margin: -5px -5px -5px -5px;                           "
        " }                                                          "
        "                                                            "
        "QSlider::sub-page:horizontal{                               "
        " background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(27, 5, 27, 255), stop:0.25 rgba(99, 20, 102, 255), stop:0.5 rgba(154, 30, 158, 255), stop:1 rgba(173, 57, 176, 255));                      "
        "}                                                           "
        "QSlider::add-page:horizontal{                               "
        //" background-image:url(pics/button.png)   "
        //" background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(27, 5, 27, 255), stop:0.25 rgba(99, 20, 102, 255), stop:0.5 rgba(154, 30, 158, 255), stop:1 rgba(173, 57, 176, 255));                      "
        "}"
        ));

    m_controlLayout = new QHBoxLayout;
    m_controlLayout->addWidget(m_pauseResumeButton);
    m_controlLayout->addWidget(m_previousButton);
    m_controlLayout->addWidget(m_stopButton);
    m_controlLayout->addWidget(m_nextButton);
    m_controlLayout->addWidget(m_slider);
    m_controlLayout->addWidget(m_timeLabel);
    m_controlLayout->addWidget(m_mutedButton);
    m_controlLayout->addWidget(m_soundSlider);
    m_controlLayout->addWidget(m_fullButton);

    m_bottomWidget->setLayout(m_controlLayout);

    connect(m_slider, SIGNAL(sliderMoved(int)), this, SLOT(updateStatus(int)));
    this->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::AllDockWidgetFeatures);
    this->setAllowedAreas(Qt::BottomDockWidgetArea);
    this->setWidget(m_bottomWidget);
    
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    palette.setBrush(QPalette::Background,QBrush(QPixmap(QString(":/APlayer/Resources/background.png"))));
    this->setPalette(palette);
    this->setAutoFillBackground(true);    
    //this->setFloating(true);
    
    QWidget * titleBarWidget = this->titleBarWidget();
    QWidget* lEmptyWidget = new QWidget();
    this->setTitleBarWidget(lEmptyWidget);
    delete titleBarWidget;

    connect(m_pauseResumeButton, SIGNAL(clicked()), this, SLOT(pauseResumeClick()));
    connect(m_mutedButton, SIGNAL(clicked()), this, SLOT(muteClick()));
    connect(m_soundSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeChange(int)));
	connect(m_slider, SIGNAL(prositionChange(int)), this, SLOT(prositionChange(int)));

    this->setHidden(false);

    this->resize(960, 45);
}

BottonDockWidget::~BottonDockWidget(void)
{
    delete m_pauseResumeButton;
    delete m_stopButton;
    delete m_nextButton;
    delete m_previousButton;
    delete m_slider;
    delete m_timeLabel;
    delete m_mutedButton;
    delete m_soundSlider;
    delete m_bottomWidget;
}

void BottonDockWidget::pauseResumeClick(void)
{
	m_player->pause(!m_player->isPaused());
    if(m_player->isPaused())
    {
        m_pauseResumeButton->setStyleSheet(QString("QPushButton {border-image: url(:/APlayer/Resources/play.png); maz-height: 20px;    max-width: 20px;  }"));
    }
    else
    {
        m_pauseResumeButton->setStyleSheet(QString("QPushButton {border-image: url(:/APlayer/Resources/pause.png); maz-height: 20px;    max-width: 20px;  }"));
    }
    
}

void BottonDockWidget::muteClick(void)
{
    m_player->setMute(!m_player->isMute());

    if(m_player->isMute())
    {
        m_mutedButton->setStyleSheet(QString("QPushButton {border-image: url(:/APlayer/Resources/mute.png); border-width: 2px;  max-height: 15px;    max-width: 15px;}"));
    }
    else
    {
        m_mutedButton->setStyleSheet(QString("QPushButton {border-image: url(:/APlayer/Resources/unMute.png); border-width: 2px;  max-height: 15px;    max-width: 15px;}"));
    }
}

void BottonDockWidget::volumeChange(int volume)
{    
    if (m_player->audio()) 
	{
        m_player->audio()->setVolume(static_cast<qreal>(qreal(volume)*0.05));
    }

    if(volume == 0)
    {
        m_mutedButton->setStyleSheet(QString("QPushButton {border-image: url(:/APlayer/Resources/mute.png); border-width: 2px;  max-height: 15px;    max-width: 15px;}"));
    }
    else
    {
        m_mutedButton->setStyleSheet(QString("QPushButton {border-image: url(:/APlayer/Resources/unMute.png); border-width: 2px;  max-height: 15px;    max-width: 15px;}"));
    }
}

void BottonDockWidget::prositionChange(int value)
{
	qDebug("seeking to new position = %d, duration = %d",value , m_duration);

	m_player->seek(value/(qreal)m_duration);
}

void BottonDockWidget::updateStatus(int currentTime, int durationTime)
{
    int hh, mm, ss, dhh, dmm, dss;
    char timeValue[32];
    hh   = currentTime / 3600;
    mm   = (currentTime % 3600) / 60;
    ss   = currentTime % 60;

    if(durationTime != -1)
    {
        m_duration = durationTime;
    }

    dhh   = m_duration / 3600;
    dmm   = (m_duration % 3600) / 60;
    dss   = m_duration % 60;

    sprintf(timeValue, "%.1d:%.2d:%.2d/%.1d:%.2d:%.2d", hh, mm, ss, dhh, dmm, dss);
    m_timeLabel->setText(timeValue);

	if(!m_slider->isSliderDown())
	{
		m_slider->setRange(0, m_duration);
		m_slider->setValue(currentTime);
	}
}

void BottonDockWidget::locationSizeChanged(bool isFullScreen, QPoint pos, QSize size)
{
    int x, y;
    if(isFullScreen)
    {
        x = pos.x();
        y = pos.y() + size.height() - this->size().height();
    }
    else
    {
        x = pos.x() + 8;
        y = pos.y() + size.height() - 15;
    }

    this->resize(size.width(), 45);
    this->move(x, y);
}

void BottonDockWidget::showBottonDock(bool flag)
{
    this->setHidden(!flag);
}

void BottonDockWidget::streamOpen(void)
{
    m_pauseResumeButton->setStyleSheet(QString("QPushButton {color: red;  border-image: url(:/APlayer/Resources/pause.png); maz-height: 20px;    max-width: 20px;  }"));
}
