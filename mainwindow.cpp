#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "logmanager.h"
#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_logManager(new LogManager(this))
    , m_testTimer(new QTimer(this))
    , m_testCounter(0)
    , m_flashTestTimer(new QTimer(this))
    , m_flashTestCounter(0)
    , m_flashTextManager(new FlashTextManager(2, 2, this))
{
    ui->setupUi(this);

    // 绑定日志显示控件
    m_logManager->setDisplay(ui->logTextEdit);

    // 连接信号槽：每条日志输出后触发 onLogProcessed
    connect(m_logManager, &LogManager::logProcessed,
            this, &MainWindow::onLogProcessed);

    // 启动测试日志生成器：每 500ms 生成一条测试日志
    connect(m_testTimer, &QTimer::timeout, this, &MainWindow::generateTestLog);
    m_testTimer->start(500);

    // 将 FlashTextManager 添加到 UI 容器的布局中
    ui->verticalLayout_3->addWidget(m_flashTextManager);

    // 设置闪烁区域标签（2行2列）
    m_flashTextManager->setLabel(0, 0, "a:");
    m_flashTextManager->setLabel(0, 1, "b:");
    m_flashTextManager->setLabel(1, 0, "c:");
    m_flashTextManager->setLabel(1, 1, "d:");

    // 启动闪烁测试：每 2 秒切换一次闪烁状态
    connect(m_flashTestTimer, &QTimer::timeout, this, &MainWindow::testFlashText);
    m_flashTestTimer->start(2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLogProcessed(const LogEntry &entry)
{
    // 简版回调：控制台输出时间戳
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    qDebug() << "[" << timestamp << "] 日志已处理:" << entry.content;

    // TODO: 在此实现你的业务逻辑
    // entry.content - 日志内容
    // entry.extra   - 其他业务数据
}

void MainWindow::generateTestLog()
{
    static const QStringList testMessages = {
        "系统启动完成",
        "正在加载配置文件",
        "连接数据库成功",
        "检测到新设备接入",
        "正在同步数据",
        "任务队列已清空",
        "收到用户指令",
        "开始执行批处理任务",
        "缓存已刷新",
        "网络连接正常",
        "系统启动完成",
        "正在加载配置文件",
        "连接数据库成功",
        "检测到新设备接入",
        "正在同步数据",
        "任务队列已清空",
        "收到用户指令",
        "开始执行批处理任务",
        "缓存已刷新",
        "网络连接正常"
    };

    LogEntry entry;
    entry.content = testMessages[m_testCounter % testMessages.size()];
    entry.extra["type"] = "info";
    entry.extra["seq"] = m_testCounter;

    m_logManager->appendLog(entry);
    m_testCounter++;

    // 生成 20 条后停止测试
    if (m_testCounter >= 20) {
        m_testTimer->stop();
        LogEntry done;
        done.content = "测试日志生成完毕";
        done.extra["type"] = "success";
        m_logManager->appendLog(done);
    }
}

void MainWindow::testFlashText()
{
    static const QStringList flashMessages = {
        "警告：温度过高",
        "注意：内存不足",
        "提示：需要重启"
    };

    // 轮流在不同格子显示闪烁文字（2行2列）
    int row = (m_flashTestCounter / 2) % 2;
    int col = m_flashTestCounter % 2;
    QString text = flashMessages[m_flashTestCounter % flashMessages.size()];

    if (m_flashTestCounter % 4 < 2) {
        // 开始闪烁
        m_flashTextManager->setFlashText(row, col, text);
        qDebug() << "开始闪烁 - 行" << row << "列" << col << ":" << text;
    } else {
        // 停止闪烁
        m_flashTextManager->stopFlash(row, col);
        qDebug() << "停止闪烁 - 行" << row << "列" << col;
    }

    m_flashTestCounter++;

    // 测试 12 次后停止
    if (m_flashTestCounter >= 12) {
        m_flashTestTimer->stop();
        m_flashTextManager->stopAll();
        qDebug() << "闪烁测试结束";
    }
}
