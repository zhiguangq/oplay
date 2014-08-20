#include "DramaLabel.h"

DramaLabel::DramaLabel(QWidget &parent, int index, QString url)
    : m_parentWidget(parent)
    , m_url(url)
    , m_index(index)
{
    if(!url.isEmpty())
    {
        this->setText(QString("µÚ%1¼¯").arg(index));    
        this->setStyleSheet("QLabel { color: white;}");
    }
    connect(this, SIGNAL(dramaLabelClick(int)), &m_parentWidget, SLOT(dramaLabelClick(int)));
}


DramaLabel::~DramaLabel(void)
{
}

QString &DramaLabel::getUrl(void)
{
    return m_url;
}

void DramaLabel::mousePressEvent(QMouseEvent *ev)
{
    emit dramaLabelClick(m_index);
}
