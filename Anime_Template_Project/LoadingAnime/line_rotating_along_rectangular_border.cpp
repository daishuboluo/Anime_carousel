// Line_Rotating_Along_Rectangular_Border.cpp
#include "line_rotating_along_rectangular_border.h"
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QPainterPath>

Line_Rotating_Along_Rectangular_Border::Line_Rotating_Along_Rectangular_Border(QWidget *parent)
    : QWidget{parent},
    m_rotationAngle(0.0)
{
    setAttribute(Qt::WA_TranslucentBackground);

    m_animation = new QPropertyAnimation(this, "rotationAngle");
    m_animation->setDuration(2000);
    m_animation->setStartValue(0.0);
    m_animation->setEndValue(360.0);
    m_animation->setLoopCount(-1);
    m_animation->setEasingCurve(QEasingCurve::InOutSine);

    m_animation->start();
}

qreal Line_Rotating_Along_Rectangular_Border::getRotationAngle() const
{
    return m_rotationAngle;
}

void Line_Rotating_Along_Rectangular_Border::setRotationAngle(qreal angle)
{
    if (m_rotationAngle == angle)
        return;
    m_rotationAngle = angle;
    emit rotationAngleChanged(m_rotationAngle);
    update();
}

void Line_Rotating_Along_Rectangular_Border::calculateSquareGeometry()
{
    int windowWidth = width();
    int windowHeight = height();

    int minSide = qMin(windowWidth, windowHeight);

    int squareSide = minSide / 2;

    int x = (windowWidth - squareSide) / 2;
    int y = (windowHeight - squareSide) / 2;

    m_squareRect = QRect(x, y, squareSide, squareSide);

    m_borderThickness = qMax(1, minSide / 50);
}

void Line_Rotating_Along_Rectangular_Border::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(239, 239, 239, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());
    QPainterPath outerPath;
    outerPath.addRect(m_squareRect);

    QRect innerRect = m_squareRect.adjusted(m_borderThickness, m_borderThickness,
                                             -m_borderThickness, -m_borderThickness);
    QPainterPath innerPath;
    innerPath.addRect(innerRect);

    QPainterPath borderClipPath = outerPath.subtracted(innerPath);

    painter.setClipPath(borderClipPath);

    painter.save();

    qreal rotatingRectLength = width();
    qreal rotatingRectWidth = m_squareRect.width() / 2.0;

    painter.translate(m_squareRect.center());

    painter.rotate(m_rotationAngle);

    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);

    QRectF rotatingRect(-rotatingRectLength / 2.0, -rotatingRectWidth / 2.0,
                        rotatingRectLength, rotatingRectWidth);
    painter.drawRect(rotatingRect);

    painter.restore();
}

void Line_Rotating_Along_Rectangular_Border::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    calculateSquareGeometry();
    update();
}