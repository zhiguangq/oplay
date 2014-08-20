#ifndef DRAMALABEL_H
#define DRAMALABEL_H

#include <QLabel>
#include <QWidget>
#include <QString>
class DramaLabel : public QLabel
{
    Q_OBJECT
public:
    DramaLabel(QWidget &parent, int index, QString url = "");
    ~DramaLabel(void);

    QString &getUrl(void);
Q_SIGNALS:
    void dramaLabelClick(int index);
protected:
    void mousePressEvent(QMouseEvent *ev);
private:
    QString     m_url;
    int         m_index;
    QWidget&    m_parentWidget;
};

#endif // DRAMALABEL_H


