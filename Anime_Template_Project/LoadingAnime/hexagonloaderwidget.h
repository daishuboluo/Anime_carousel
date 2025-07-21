#ifndef HEXAGONWIDGET_H
#define HEXAGONWIDGET_H

#include <QWidget>
#include <QColor>
#include <QVector>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>
#include <QResizeEvent>
#include <vector>


class HexBrick : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit HexBrick(QColor color = Qt::black, qreal rotation = 0.0, QObject* parent = nullptr);

    QColor color() const { return m_color; }
    void setColor(const QColor& color);

    qreal rotation() const { return m_rotation; }
    void setRotation(qreal rotation) { m_rotation = rotation; }

signals:
    void colorChanged();

private:
    QColor m_color;
    qreal m_rotation;
};

class Gel
{
public:
    Gel(const QPointF& offset, qreal delay = 0.0);

    QPointF offset() const { return m_offset; }
    void setOffset(const QPointF& offset) { m_offset = offset; }

    qreal scale() const { return m_scale; }
    void setScale(qreal scale) { m_scale = scale; }

    std::vector<HexBrick*>& hexBricks() { return m_hexBricks; }
    const std::vector<HexBrick*>& hexBricks() const { return m_hexBricks; }

    qreal animationDelay() const { return m_animationDelay; }

private:
    QPointF m_offset;
    qreal m_scale;
    std::vector<HexBrick*> m_hexBricks;
    qreal m_animationDelay;
};

class HexagonLoaderWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal centerGelScale READ centerGelScale WRITE setCenterGelScale NOTIFY centerGelScaleChanged)
    Q_PROPERTY(qreal r1Scale READ r1Scale WRITE setR1Scale NOTIFY r1ScaleChanged)
    Q_PROPERTY(qreal r2Scale READ r2Scale WRITE setR2Scale NOTIFY r2ScaleChanged)
    Q_PROPERTY(qreal r3Scale READ r3Scale WRITE setR3Scale NOTIFY r3ScaleChanged)

public:
    explicit HexagonLoaderWidget(QWidget *parent = nullptr);
    ~HexagonLoaderWidget();

    qreal centerGelScale() const;
    void setCenterGelScale(qreal scale);

    qreal r1Scale() const;
    void setR1Scale(qreal scale);

    qreal r2Scale() const;
    void setR2Scale(qreal scale);

    qreal r3Scale() const;
    void setR3Scale(qreal scale);

signals:
    void centerGelScaleChanged();
    void r1ScaleChanged();
    void r2ScaleChanged();
    void r3ScaleChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void drawHexagon(QPainter& painter, const QPointF& center, qreal size, const QColor& color, qreal rotation = 0.0);

private:
    void setupGels();
    void setupAnimations();
    QPointF calculateHexagonPoint(qreal size, qreal angleDegrees);

    std::vector<Gel> m_gels;

    QList<QPropertyAnimation*> m_pulseAnimations;
    QList<QPropertyAnimation*> m_fadeAnimations;

    qreal m_centerGelCurrentScale;
    qreal m_r1CurrentScale;
    qreal m_r2CurrentScale;
    qreal m_r3CurrentScale;

    qreal m_currentGlobalScale = 1.0;
};

#endif // HEXAGONWIDGET_H
