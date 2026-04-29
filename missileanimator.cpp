#include "missileanimator.h"
#include <QPainter>
#include <QResizeEvent>

MissileAnimator::MissileAnimator(QLabel *label, QObject *parent)
    : QObject(parent)
    , m_label(label)
    , m_animation(new QPropertyAnimation(this, "progress"))
{
    // 安装事件过滤器，监听 QLabel 的 Resize 事件
    m_label->installEventFilter(this);

    m_animation->setDuration(m_duration);
    m_animation->setStartValue(0.0);
    m_animation->setEndValue(1.0);
    m_animation->setEasingCurve(QEasingCurve::Linear);

    connect(m_animation, &QPropertyAnimation::finished, this, [this]() {
        // 动画结束：清空图片，恢复空状态
        m_progress = 0.0;
        m_label->setPixmap(QPixmap());
        m_label->update();
        emit finished();
    });
}

void MissileAnimator::renderImage(const QString &imagePath)
{
    // 停止可能正在运行的动画
    if (m_animation->state() == QAbstractAnimation::Running) {
        m_animation->stop();
    }
    m_progress = 0.0;

    // 加载原始图片
    m_originalPixmap.load(imagePath);
    if (m_originalPixmap.isNull()) {
        return;
    }

    // 缩放图片：高度适配 QLabel，保持纵横比
    int labelHeight = m_label->height();
    if (labelHeight <= 0) {
        labelHeight = m_label->minimumSize().height();
    }
    if (labelHeight <= 0) {
        labelHeight = 50; // 兜底值
    }
    m_scaledPixmap = m_originalPixmap.scaledToHeight(labelHeight, Qt::SmoothTransformation);

    // 设置到 QLabel 显示
    m_label->setPixmap(m_scaledPixmap);
}

void MissileAnimator::launch()
{
    // 如果图片未加载，无法启动动画
    if (m_originalPixmap.isNull()) {
        return;
    }

    // 如果动画正在运行，先停止
    if (m_animation->state() == QAbstractAnimation::Running) {
        m_animation->stop();
    }

    // 重新缩放图片以适配当前 QLabel 尺寸
    int labelHeight = m_label->height();
    if (labelHeight <= 0) {
        labelHeight = m_label->minimumSize().height();
    }
    if (labelHeight <= 0) {
        labelHeight = 50;
    }
    m_scaledPixmap = m_originalPixmap.scaledToHeight(labelHeight, Qt::SmoothTransformation);

    // 重置进度并启动动画
    m_progress = 0.0;
    m_animation->setDuration(m_duration);
    m_animation->start();
}

void MissileAnimator::clearImage()
{
    // 停止正在进行的动画
    if (m_animation->state() == QAbstractAnimation::Running) {
        m_animation->stop();
    }

    // 清空图片和状态
    m_progress = 0.0;
    m_label->setPixmap(QPixmap());
    m_label->update();
}

void MissileAnimator::setDuration(int ms)
{
    m_duration = ms;
    m_animation->setDuration(m_duration);
}

int MissileAnimator::duration() const
{
    return m_duration;
}

qreal MissileAnimator::progress() const
{
    return m_progress;
}

void MissileAnimator::setProgress(qreal progress)
{
    if (qFuzzyCompare(m_progress, progress)) {
        return;
    }
    m_progress = progress;
    emit progressChanged();
    updateFrame();
}

void MissileAnimator::updateFrame()
{
    if (m_scaledPixmap.isNull() || !m_label) {
        return;
    }

    // 使用 contentsRect 获取 QLabel 实际可用绘制区域，排除 margin/border
    QRect cr = m_label->contentsRect();
    int labelWidth = cr.width();
    int labelHeight = cr.height();
    int imgWidth = m_scaledPixmap.width();
    int imgHeight = m_scaledPixmap.height();

    // 创建与 QLabel 同尺寸的透明画布
    QPixmap frame(m_label->size());
    frame.fill(Qt::transparent);

    QPainter painter(&frame);
    // 偏移到 contentsRect 起点，排除 margin
    painter.translate(cr.x(), cr.y());

    // 计算 x 偏移：progress=0 时图片在当前位置（左对齐），progress=1 时图片完全移出右边界
    int x = static_cast<int>(m_progress * labelWidth);
    // y 方向居中
    int y = (labelHeight - imgHeight) / 2;

    painter.drawPixmap(x, y, m_scaledPixmap);
    painter.end();

    m_label->setPixmap(frame);
}

bool MissileAnimator::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_label && event->type() == QEvent::Resize) {
        // 窗口缩放时重新适配图片
        if (!m_originalPixmap.isNull()) {
            auto *resizeEvent = static_cast<QResizeEvent *>(event);
            int newHeight = resizeEvent->size().height();

            if (newHeight > 0) {
                m_scaledPixmap = m_originalPixmap.scaledToHeight(newHeight, Qt::SmoothTransformation);

                if (m_animation->state() == QAbstractAnimation::Running) {
                    // 动画进行中：用新缩放的图片刷新当前帧
                    updateFrame();
                } else {
                    // 静态渲染：直接更新 QLabel
                    m_label->setPixmap(m_scaledPixmap);
                }
            }
        }
    }
    return QObject::eventFilter(watched, event);
}
