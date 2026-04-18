#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QVariantMap>
// #include <QTextEdit>
#include "logdisplay.h"

// 可调常量
/*constexpr int MAX_LOG_LINES = 1000;  */         // 最大显示行数
constexpr int LOG_OUTPUT_INTERVAL_MS = 1000;  // 日志输出间隔（毫秒）

struct LogEntry {
    QString content;      // 日志内容
    QVariantMap extra;    // 其他业务数据
};

class LogManager : public QObject
{
    Q_OBJECT

public:
    explicit LogManager(QObject *parent = nullptr);
    ~LogManager() override;

    // // 绑定日志显示控件
    // void setDisplay(QTextEdit *display);
    // 设置输出间隔（毫秒）
    void setOutputInterval(int ms);

    // 追加一条日志到队列
    void appendLog(const LogEntry &entry);

    // 批量追加日志到队列
    void appendLogs(const QList<LogEntry> &entries);

signals:
    // 每条日志输出后发出，供外部执行业务逻辑
    void logProcessed(const LogEntry &entry);
    void logReady(const QString &html);
private slots:
    void onTimeout();

private:
    QQueue<LogEntry> m_queue;
    QTimer *m_timer;
    // QTextEdit *m_display;
    int m_outputInterval;
};

#endif // LOGMANAGER_H
