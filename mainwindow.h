#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "logmanager.h"
#include "flashtextmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // 每条日志输出后的回调，在此实现你的业务逻辑
    void onLogProcessed(const LogEntry &entry);
    // 生成测试日志
    void generateTestLog();
    // 测试闪烁文字
    void testFlashText();

private:
    Ui::MainWindow *ui;

    QTimer *m_flashTestTimer;
    int m_flashTestCounter;
    FlashTextManager *m_flashTextManager;
    // 日志相关
    LogDisplay *m_logDisplay1;
    LogDisplay *m_logDisplay2;
    LogManager *m_logA;
    LogManager *m_logB;
    LogManager *m_logC;
};
#endif // MAINWINDOW_H
