#include "flashcell.h"

#include "flashtextmanager.h"
#include <QHBoxLayout>
#include <QLabel>

FlashCell::FlashCell(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(this))
    , m_flashLabel(new QLabel(this))
    , m_timer(new QTimer(this))
    , m_visible(true)
{
    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("FlashCell { border-right: 1px solid #555555; "
                        "border-bottom: 1px solid #555555;}");
    // 水平布局
    auto *layout = new QHBoxLayout(this);
    layout->setSpacing(5);
    layout->setContentsMargins(5, 2, 5, 2);

    // 固定标签 min-width: 40px; max-width: 40px;
    m_label->setStyleSheet("color: #FFFFFF;background-color:black;min-width: 30px; max-width: 30px;");
    layout->addWidget(m_label);
    // m_label->setFixedWidth(10);

    // 闪烁文字（黄色）
    m_flashLabel->setStyleSheet("color: #FFD700;");
    m_flashLabel->setVisible(false);
    layout->addWidget(m_flashLabel);

    layout->addStretch();

    // 连接定时器
    connect(m_timer, &QTimer::timeout, this, &FlashCell::onToggleVisibility);
}

FlashCell::~FlashCell() = default;

void FlashCell::setLabel(const QString &text)
{
    m_label->setText(text);
}

void FlashCell::setFlashText(const QString &text)
{
    m_flashLabel->setText(text);
    m_flashLabel->setVisible(true);
    m_visible = true;

    // 启动闪烁定时器
    if (!m_timer->isActive()) {
        m_timer->start(FLASH_TOGGLE_INTERVAL_MS);
    }
}

void FlashCell::stopFlash()
{
    m_timer->stop();
    m_flashLabel->setVisible(false);
    m_flashLabel->clear();
}

bool FlashCell::isFlashing() const
{
    return m_timer->isActive();
}

void FlashCell::onToggleVisibility()
{
    m_visible = !m_visible;
    m_flashLabel->setVisible(m_visible);
}