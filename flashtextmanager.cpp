#include "flashtextmanager.h"

#include "flashcell.h"

FlashTextManager::FlashTextManager(int rows, int cols, QWidget *parent)
    : QWidget(parent)
    , m_rows(rows)
    , m_cols(cols)
{
    setupCells();
}

FlashTextManager::~FlashTextManager() = default;

void FlashTextManager::setLabel(int row, int col, const QString &text)
{
    if (!isValidPosition(row, col)) {
        return;
    }
    m_cells[cellIndex(row, col)]->setLabel(text);
}

void FlashTextManager::setFlashText(int row, int col, const QString &text)
{
    if (!isValidPosition(row, col)) {
        return;
    }
    m_cells[cellIndex(row, col)]->setFlashText(text);
}

void FlashTextManager::stopFlash(int row, int col)
{
    if (!isValidPosition(row, col)) {
        return;
    }
    m_cells[cellIndex(row, col)]->stopFlash();
}

void FlashTextManager::stopAll()
{
    for (auto *cell : m_cells) {
        cell->stopFlash();
    }
}

int FlashTextManager::rows() const
{
    return m_rows;
}

int FlashTextManager::cols() const
{
    return m_cols;
}

void FlashTextManager::setupCells()
{
    auto *layout = new QGridLayout(this);
    layout->setSpacing(5);
    layout->setContentsMargins(5, 5, 5, 5);

    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            auto *cell = new FlashCell(this);
            m_cells.append(cell);
            layout->addWidget(cell, r, c);
        }
    }
}

int FlashTextManager::cellIndex(int row, int col) const
{
    return row * m_cols + col;
}

bool FlashTextManager::isValidPosition(int row, int col) const
{
    return row >= 0 && row < m_rows && col >= 0 && col < m_cols;
}