#ifndef FLASHTEXTMANAGER_H
#define FLASHTEXTMANAGER_H

#include <QWidget>
#include <QGridLayout>

class FlashCell;

// 闪烁切换间隔（毫秒）
constexpr int FLASH_TOGGLE_INTERVAL_MS = 500;

class FlashTextManager : public QWidget
{
    Q_OBJECT

public:
    // 构造时指定行数和列数
    explicit FlashTextManager(int rows, int cols, QWidget *parent = nullptr);
    ~FlashTextManager() override;

    // 设置某个格子的固定标签
    void setLabel(int row, int col, const QString &text);

    // 设置某个格子的闪烁文字（自动开始闪烁）
    void setFlashText(int row, int col, const QString &text);

    // 停止某个格子的闪烁（文字消失）
    void stopFlash(int row, int col);

    // 停止所有闪烁
    void stopAll();

    // 获取行数
    int rows() const;

    // 获取列数
    int cols() const;

private:
    void setupCells();
    int cellIndex(int row, int col) const;
    bool isValidPosition(int row, int col) const;

    QList<FlashCell*> m_cells;
    int m_rows;
    int m_cols;
};

#endif // FLASHTEXTMANAGER_H