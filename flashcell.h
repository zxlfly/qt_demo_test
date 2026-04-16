#ifndef FLASHCELL_H
#define FLASHCELL_H

#include <QWidget>
#include <QTimer>

class QLabel;

class FlashCell : public QWidget
{
    Q_OBJECT

public:
    explicit FlashCell(QWidget *parent = nullptr);
    ~FlashCell() override;

    // 设置固定标签文字
    void setLabel(const QString &text);

    // 设置闪烁文字（自动开始闪烁）
    void setFlashText(const QString &text);

    // 停止闪烁（文字消失）
    void stopFlash();

    // 是否正在闪烁
    bool isFlashing() const;

private slots:
    void onToggleVisibility();

private:
    QLabel *m_label;       // 固定标签
    QLabel *m_flashLabel;  // 闪烁文字
    QTimer *m_timer;       // 闪烁定时器
    bool m_visible;        // 当前闪烁文字是否可见
};

#endif // FLASHCELL_H