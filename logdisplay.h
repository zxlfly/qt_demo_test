#ifndef LOGDISPLAY_H
#define LOGDISPLAY_H

#include <QObject>
#include <QTextEdit>
constexpr int MAX_LOG_LINES = 2000;
class LogDisplay : public QObject
{
    Q_OBJECT
public:
    explicit LogDisplay(QTextEdit *display, QObject *parent = nullptr);
    ~LogDisplay()override;
signals:
public slots:
    void appendHtml(const QString &html);
private:
    QTextEdit *m_display;
};

#endif // LOGDISPLAY_H
