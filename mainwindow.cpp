#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "logmanager.h"
#include "logdisplay.h"
#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_logDisplay1 = new LogDisplay(ui->logTextEdit, this);
    m_logDisplay2 = new LogDisplay(ui->logTextEdit_2,this);
    // 绑定日志显示控件
    m_logA = new LogManager(this);
    connect(m_logA, &LogManager::logReady,m_logDisplay1,&LogDisplay::appendHtml);

    m_logB = new LogManager(this);
    connect(m_logB, &LogManager::logReady,m_logDisplay1,&LogDisplay::appendHtml);

    m_logC = new LogManager(this);
    connect(m_logC, &LogManager::logReady,m_logDisplay2,&LogDisplay::appendHtml);

    // 连接信号槽：每条日志输出后触发 onLogProcessed
    connect(m_logA, &LogManager::logProcessed,
            this, &MainWindow::onLogProcessed);
    connect(m_logB, &LogManager::logProcessed,
            this, &MainWindow::onLogProcessed);
    connect(m_logC, &LogManager::logProcessed,
            this, &MainWindow::onLogProcessed);
    generateTestLog();
    // 将 FlashTextManager 添加到 UI 容器的布局中

    m_flashTestTimer = new QTimer(this);
    m_flashTestCounter = 0;
    // 每个独立的闪烁区域一个 FlashTextManager 实例，有多少个就实例化多少个就行
    m_flashTextManager = new FlashTextManager(2, 2, this);
    ui->verticalLayout_3->addWidget(m_flashTextManager);
    // 设置闪烁区域标签（2行2列）
    m_flashTextManager->setLabel(0, 0, "a:");
    m_flashTextManager->setLabel(0, 1, "b右:");
    m_flashTextManager->setLabel(1, 0, "c:");
    m_flashTextManager->setLabel(1, 1, "d:");
    // 启动闪烁测试：每 2 秒切换一次闪烁状态
    connect(m_flashTestTimer, &QTimer::timeout, this, &MainWindow::testFlashText);
    m_flashTestTimer->start(2000);

    // 导弹动画：绑定 UI 中已有的 QLabel
    m_missileAnimator = new MissileAnimator(ui->label, this);
    connect(m_missileAnimator, &MissileAnimator::finished,
            this, &MainWindow::onMissileFinished);

    // 先渲染图片，3 秒后自动启动发射动画
    m_missileAnimator->renderImage(":/images/d.png");
    m_missileTestTimer = new QTimer(this);
    m_missileTestTimer->setSingleShot(true);
    connect(m_missileTestTimer, &QTimer::timeout, this, &MainWindow::testMissileLaunch);
    m_missileTestTimer->start(3000);
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
    // 构造函数里，替代定时器的方式
    static const QStringList testMessagesA = {
        "LOGA1", "LOGA2", "LOGA3", "LOGA4", "LOGA5",
        "LOGA6", "LOGA7", "LOGA8", "LOGA9", "LOGA10",
    };
    static const QStringList testMessagesB = {
        "logb1", "logb2", "logb3", "logb4", "logb5",
        "logb6", "logb7", "logb8", "logb9", "logb10",
    };
    static const QStringList testMessagesC = {
        "C1", "C2", "C3", "C4", "C5",
        "C6", "C7", "C8", "C9", "C10",
    };
    for (int i = 0; i < 20; ++i) {
        LogEntry entryA;
        entryA.content = testMessagesA[i % testMessagesA.size()];
        entryA.extra["type"] = "info";
        m_logA->appendLog(entryA);

        LogEntry entryB;
        entryB.content = testMessagesB[i % testMessagesB.size()];
        entryB.extra["type"] = "info";
        m_logB->appendLog(entryB);

        LogEntry entryC;
        entryC.content = testMessagesC[i % testMessagesC.size()];
        entryC.extra["type"] = "info";
        m_logC->appendLog(entryC);
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

void MainWindow::testMissileLaunch()
{
    qDebug() << "启动导弹发射动画";
    m_missileAnimator->launch();
}

void MainWindow::onMissileFinished()
{
    qDebug() << "导弹动画结束";

    // 动画结束后，3 秒后重新渲染图片并再次发射，循环演示
    QTimer::singleShot(3000, this, [this]() {
        m_missileAnimator->renderImage(":/images/d.png");
        QTimer::singleShot(2000, this, [this]() {
            m_missileAnimator->launch();
        });
    });
}
