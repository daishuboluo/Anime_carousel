#include "Bouncing_Ball_Loading.h"
#include <QPainter>
#include <QDebug>
#include <cmath>

Bouncing_Ball_Loading::Bouncing_Ball_Loading(QWidget *parent)
    : QWidget(parent)
{
    this->updateBallPositions(m_ballCount);

    m_timer = new QTimer(this);
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, [=] {
        update();
    });
    m_updateTimer->setTimerType(Qt::PreciseTimer);

    m_updateTimer->start(15);

    connect(m_timer, &QTimer::timeout, this, [=] {
        if (m_balls.size() - 1 < m_timerCount)
        {
            m_timer->stop();
            m_timerCount = 0;
            return;
        }

        qDebug() << "当前球号：" << m_timerCount;

        m_balls[m_timerCount]->startAnimation();
        m_timerCount++;
    });
    m_timer->setTimerType(Qt::PreciseTimer);

    m_timer->start(200);
}

void Bouncing_Ball_Loading::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(239, 239, 239, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    for (auto sphere : m_balls)
    {
        painter.setBrush(QColor(117,117,117,106));

        qreal endshadowY = sphere->fallEndPosition().ry();
        qreal positionY = sphere->position().ry();
        qreal shadowWidth = (positionY / endshadowY) * 30;
        painter.drawEllipse(sphere->fallEndPosition() + QPointF(0.0,25.0), 2 + shadowWidth, 5);

        painter.setBrush(sphere->ballColor());
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(sphere->position(), sphere->widthRadius(), sphere->heightRadius());
    }
}

void Bouncing_Ball_Loading::updateBallPositions(int ballCount)
{
    if (!m_balls.empty())
    {
        for (Sphere *sphere : m_balls)
        {
            delete sphere;
        }
        m_balls.clear();
    }
    if (ballCount <= 0)
    {
        qDebug() << "球的数量小于等于0";
        return;
    }

    int _x = width() / (ballCount + 3);
    int _y = height() / 5;

    for (int i = 1; i < ballCount + 1; ++i)
    {
        Sphere *sphere1 = new Sphere(this);
        sphere1->setStartAndEndPosition(QPointF(_x * (i * 2 -1) , _y), QPointF(_x * (i * 2 -1), height() - _y));
        sphere1->configureAnimation();

        m_balls.push_back(sphere1);
    };
}

void Bouncing_Ball_Loading::setSize(QSize size)
{
    this->resize(size);
    this->updateBallPositions(m_ballCount);
    m_timer->start(200);
}

Sphere::Sphere(QObject *parent) : QObject(parent)
{

}

void Sphere::startAnimation()
{
    m_animationFinished = false;
    animation->start();
}

void Sphere::configureAnimation()
{
    QPointF L_position = m_position;
    QPointF L_fallEndPosition = m_fallEndPosition;

    animation = new QPropertyAnimation(this, "position");
    animation->setDuration(400);
    animation->setStartValue(L_position);
    animation->setEndValue(L_fallEndPosition);
    animation->setEasingCurve(QEasingCurve::Linear);

    connect(animation, &QPropertyAnimation::finished, this, [=] {
        animation2 = new QPropertyAnimation(this, "widthRadius");
        animation2->setDuration(100);
        animation2->setKeyValueAt(0.0,30);
        animation2->setKeyValueAt(0.4,40);
        animation2->setKeyValueAt(0.6,40);
        animation2->setKeyValueAt(1.0,30);
        animation2->setEasingCurve(QEasingCurve::Linear);

        animation3 = new QPropertyAnimation(this, "heightRadius");
        animation3->setDuration(100);
        animation3->setKeyValueAt(0.0,30);
        animation3->setKeyValueAt(0.4,10);
        animation3->setKeyValueAt(0.6,10);
        animation3->setKeyValueAt(1.0,30);
        animation3->setEasingCurve(QEasingCurve::Linear);

        animation2->start();
        animation3->start();

        connect(animation2, &QPropertyAnimation::finished, this, [=] {
            animation4 = new QPropertyAnimation(this, "position");
            animation4->setDuration(400);
            animation4->setStartValue(L_fallEndPosition);
            animation4->setEndValue(L_position);
            animation4->setEasingCurve(QEasingCurve::Linear);
            animation4->start();

            connect(animation4, &QPropertyAnimation::finished, this, [=] {
                startAnimation();
                m_animationFinished = true;
            });
        });
    });
}

void Sphere::setStartAndEndPosition(QPointF startPosition, QPointF endPosition)
{
    if (m_position == startPosition && m_fallEndPosition == endPosition)
        return;
    m_position = startPosition;
    m_fallEndPosition = endPosition;
    emit positionChanged();
    emit fallEndPositionChanged();
}

QPointF Sphere::position() const
{
    return m_position;
}

void Sphere::setPosition(QPointF newPosition)
{
    if (m_position == newPosition)
        return;
    m_position = newPosition;
    emit positionChanged();
}

int Sphere::widthRadius() const
{
    return m_widthRadius;
}

void Sphere::setWidthRadius(int newWidthRadius)
{
    if (m_widthRadius == newWidthRadius)
        return;
    m_widthRadius = newWidthRadius;
    emit widthRadiusChanged();
}

int Sphere::heightRadius() const
{
    return m_heightRadius;
}

void Sphere::setHeightRadius(int newHeightRadius)
{
    if (m_heightRadius == newHeightRadius)
        return;
    m_heightRadius = newHeightRadius;
    emit heightRadiusChanged();
}

QPointF Sphere::fallEndPosition() const
{
    return m_fallEndPosition;
}

void Sphere::setFallEndPosition(QPointF newFallEndPosition)
{
    if (m_fallEndPosition == newFallEndPosition)
        return;
    m_fallEndPosition = newFallEndPosition;
    emit fallEndPositionChanged();
}