#include "logdisplay.h"


LogDisplay::LogDisplay(QTextEdit *display,QObject *parent)
    :QObject{parent}
    ,m_display(display)
{
    if (m_display) {
        m_display->setReadOnly(true);
        if(MAX_LOG_LINES != -1){
            m_display->document()->setMaximumBlockCount(MAX_LOG_LINES);
        }
    }
}
LogDisplay::~LogDisplay() = default;
void LogDisplay::appendHtml(const QString &html)
{
    if (m_display) {
        m_display->append(html);
        m_display->moveCursor(QTextCursor::End);
    }
}
