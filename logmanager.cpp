#include "logmanager.h"

#include <QDateTime>
#include <QTextCursor>

LogManager::LogManager(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_outputInterval(LOG_OUTPUT_INTERVAL_MS)
{
    connect(m_timer, &QTimer::timeout, this, &LogManager::onTimeout);
}

LogManager::~LogManager()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

// void LogManager::setDisplay(QTextEdit *display)
// {
//     m_display = display;
//     if (m_display) {
//         m_display->setReadOnly(true);
//     }
// }

void LogManager::setOutputInterval(int ms)
{
    m_outputInterval = ms;
    if (m_timer->isActive()) {
        m_timer->setInterval(m_outputInterval);
    }
}

void LogManager::appendLog(const LogEntry &entry)
{
    m_queue.enqueue(entry);
    if (!m_timer->isActive()) {
        m_timer->start(m_outputInterval);
    }
}

void LogManager::appendLogs(const QList<LogEntry> &entries)
{
    for (const auto &entry : entries) {
        m_queue.enqueue(entry);
    }
    if (!m_timer->isActive() && !m_queue.isEmpty()) {
        m_timer->start(m_outputInterval);
    }
}

void LogManager::onTimeout()
{
    if (m_queue.isEmpty()) {
        m_timer->stop();
        return;
    }

    LogEntry entry = m_queue.dequeue();

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // 使用 HTML 富文本设置颜色：时间白色（等宽字体固定宽度），内容黄色
    QString html = QString("<span style=\"color: #FFFFFF; font-family: 'Courier New', monospace;\">%1</span>  <span style=\"color: #FFD700;\">%2</span>")
                       .arg(timestamp, entry.content);

    // m_display->append(html);

    // // 限制最大行数
    // if (m_display->document()->blockCount() > MAX_LOG_LINES) {
    //     QTextCursor cursor(m_display->document());
    //     cursor.movePosition(QTextCursor::Start);
    //     cursor.select(QTextCursor::BlockUnderCursor);
    //     cursor.removeSelectedText();
    // }

    // // 自动滚动到底部
    // m_display->moveCursor(QTextCursor::End);
    emit logReady(html);
    emit logProcessed(entry);
}
