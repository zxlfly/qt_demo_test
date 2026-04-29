#ifndef MISSILEANIMATOR_H
#define MISSILEANIMATOR_H

#include <QObject>
#include <QLabel>
#include <QPixmap>
#include <QPropertyAnimation>

class MissileAnimator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress WRITE setProgress NOTIFY progressChanged)

public:
    explicit MissileAnimator(QLabel *label, QObject *parent = nullptr);

    // 静态渲染图片到 QLabel
    void renderImage(const QString &imagePath);
    // 启动发射动画（匀速向右移动，直到完全移出容器）
    void launch();
    // 直接清除图片，不播放动画
    void clearImage();
    // 设置动画时长（毫秒），默认 2000ms
    void setDuration(int ms);
    int duration() const;

signals:
    void finished();
    void progressChanged();

protected:
    // 监听 QLabel 的 Resize 事件，窗口缩放时重新适配图片
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    qreal progress() const;
    void setProgress(qreal progress);
    // 根据当前 progress 绘制一帧并设置到 QLabel
    void updateFrame();

    QLabel *m_label;
    QPixmap m_originalPixmap;    // 原始加载的图片
    QPixmap m_scaledPixmap;      // 缩放后的图片（高度适配 QLabel）
    qreal m_progress = 0.0;
    int m_duration = 2000;       // 默认动画时长 2000ms
    QPropertyAnimation *m_animation;
};

#endif // MISSILEANIMATOR_H
