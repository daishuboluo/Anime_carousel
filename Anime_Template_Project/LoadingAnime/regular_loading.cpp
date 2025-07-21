// regular_loading.cpp
#include "regular_loading.h"
#include <QDebug>

Regular_Loading::Regular_Loading(QWidget *parent)
    : QWidget{parent}
    , m_rotationAngle(0)
{
    setMinimumSize(100, 100);

    m_animation = new QPropertyAnimation(this, "rotationAngle");
    m_animation->setStartValue(0);
    m_animation->setEndValue(360);
    m_animation->setDuration(2000);
    m_animation->setLoopCount(-1);
    m_animation->setEasingCurve(QEasingCurve::Linear);

    connect(this, &Regular_Loading::rotationAngleChanged, this, QOverload<>::of(&QWidget::update));

    m_animation->start();
}

void Regular_Loading::setRotationAngle(int angle)
{
    if (m_rotationAngle != angle) {
        m_rotationAngle = angle % 360;
        emit rotationAngleChanged(m_rotationAngle);
    }
}

void Regular_Loading::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(239, 239, 239, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());
    int side = qMin(width(), height());
    int circleDiameter = side * 0.8;
    int x = (width() - circleDiameter) / 2;
    int y = (height() - circleDiameter) / 2;
    QRectF circleRect(x, y, circleDiameter, circleDiameter);

    qreal borderWidth = qMax(2.0, side / 20.0);

    QPen circlePen(QColor(252,204,214));
    circlePen.setWidthF(borderWidth);
    circlePen.setCapStyle(Qt::RoundCap);
    painter.setPen(circlePen);
    painter.drawEllipse(circleRect);

    QPen arcPen(QColor(250,66,101));
    arcPen.setWidthF(borderWidth);
    arcPen.setCapStyle(Qt::RoundCap);
    painter.setPen(arcPen);

    int startAngle = -m_rotationAngle * 16;
    int spanAngle = 40 * 16;

    painter.drawArc(circleRect, startAngle, spanAngle);
}