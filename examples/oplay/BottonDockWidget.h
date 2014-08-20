#ifndef BOTTOMDOCKWIDGET_H
#define BOTTOMDOCKWIDGET_H

#include <QSlider>
#include <QDockWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QtAV/AVPlayer.h>

class ProgressQSlider : public QSlider
{
Q_OBJECT
public:
    ProgressQSlider(Qt::Orientation orientation, QWidget *parent = 0) 
		: QSlider(orientation, parent)
    {
		// 关闭点击进度条时的步进功能
		this->setSingleStep(0);
		this->setPageStep(0);
    }
protected:
    void mousePressEvent(QMouseEvent *ev)
    {
        //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
        QSlider::mousePressEvent(ev);

        //设置点击进度条时，进度点定位到鼠标位置
        double pos = ev->pos().x() / (double)width();
        setValue(pos * (maximum() - minimum()) + minimum());
    }
	void mouseReleaseEvent(QMouseEvent *ev)
	{
		QSlider::mouseReleaseEvent(ev);

		//鼠标松开时，出发一个事件，以便做seek操作
		emit prositionChange(this->value());
	}
Q_SIGNALS:
	void prositionChange(int value);
};

class BottonDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    BottonDockWidget(QWidget *parent, QtAV::AVPlayer* player);
    ~BottonDockWidget(void);

    inline QPushButton* getStopButton(void)
    {
        return m_stopButton;
    }
    inline QPushButton* getFullscreenButton(void)
    {
        return m_fullButton;
    }
    inline QPushButton* getPreviousButton(void)
    {
        return m_previousButton;
    }
    inline QPushButton* getNextButton(void)
    {
        return m_nextButton;
    }
Q_SIGNALS:
    void setMute(bool& flag);
private slots:
    void streamOpen(void);
    void showBottonDock(bool flag);
    void locationSizeChanged(bool isFullScreen, QPoint pos, QSize size);
    void updateStatus(int currentTime, int duration = -1);
    void pauseResumeClick(void);
    void muteClick(void);
    void volumeChange(int);
	void prositionChange(int value);
private:
    QWidget     *m_bottomWidget;
    QPushButton *m_pauseResumeButton;
    QPushButton *m_stopButton;
    QPushButton *m_previousButton;
    QPushButton *m_nextButton;
    ProgressQSlider     *m_slider;
    QLabel      *m_timeLabel;
    QPushButton *m_mutedButton;
    QSlider     *m_soundSlider;
    QPushButton *m_fullButton;
    QBoxLayout  *m_controlLayout;
    int          m_duration;
    QtAV::AVPlayer*         m_player;
};

#endif // BOTTOMDOCKWIDGET_H

