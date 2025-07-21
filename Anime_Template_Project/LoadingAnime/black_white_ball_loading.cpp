#include "black_white_ball_loading.h"
#include <QPainter>
#include <QDebug>
#include <QResizeEvent>
#include <QtMath>

Black_White_Ball_Loading::Black_White_Ball_Loading(QWidget *parent)
    : QWidget{parent}
    , m_timer(new QTimer(this))
    , m_ballPairCount(1)
    , m_startPosRatio(0.6)
    , m_endPosRatio(0.8)
    , m_ballRadiusRatio(0.03)
    , m_animationStep(0.02)
    , m_minSide(0)
    , m_groupCount(1)
{
    connect(m_timer, &QTimer::timeout, this, &Black_White_Ball_Loading::updateAnimation);
    m_timer->start(16);

    updateAngles();
    initializeGroupData();
    resize(200, 200);
}

Black_White_Ball_Loading::~Black_White_Ball_Loading()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void Black_White_Ball_Loading::setBallPairCount(int count)
{
    if (count > 0 && count != m_ballPairCount) {
        m_ballPairCount = count;
        updateAngles();
        initializeGroupData();
        update();
    }
}

void Black_White_Ball_Loading::setAnimationRatios(qreal startRatio, qreal endRatio)
{
    if (startRatio >= 0.0 && startRatio < endRatio && endRatio <= 1.0) {
        m_startPosRatio = startRatio;
        m_endPosRatio = endRatio;
        update();
    } else {
        qWarning() << "动画比例无效。起始比例必须小于结束比例，且两者均在0到1之间。";
    }
}

void Black_White_Ball_Loading::setBallRadiusRatio(qreal ratio)
{
    if (ratio > 0.0 && ratio < 0.2) {
        m_ballRadiusRatio = ratio;
        update();
    } else {
        qWarning() << "球体半径比例无效。比例必须大于0且小于0.2。";
    }
}

void Black_White_Ball_Loading::setAnimationSpeed(qreal speed)
{
    if (speed > 0.0 && speed <= 0.1) {
        m_animationStep = speed;
    } else {
        qWarning() << "动画速度无效。速度必须在0到0.1之间。";
    }
}

void Black_White_Ball_Loading::setGroupCount(int count)
{
    if (count > 0 && count != m_groupCount) {
        m_groupCount = count;
        initializeGroupData();
        update();
    } else if (count <= 0) {
        qWarning() << "分组数量必须大于0。";
    }
}

void Black_White_Ball_Loading::initializeGroupData()
{
    m_groupProgresses.clear();
    m_groupForwards.clear();
    m_groupDrawWhiteFirst.clear();

    for (int i = 0; i < m_groupCount; ++i) {
        m_groupProgresses.append(static_cast<qreal>(i) / m_groupCount);
        m_groupForwards.append(true);
        m_groupDrawWhiteFirst.append(true);
    }
}

void Black_White_Ball_Loading::updateAngles()
{
    m_angles.clear();
    if (m_ballPairCount > 0) {
        qreal angleStep = 360.0 / m_ballPairCount;
        for (int i = 0; i < m_ballPairCount; ++i) {
            m_angles.append(qDegreesToRadians(i * angleStep));
        }
    }
}

void Black_White_Ball_Loading::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    m_center = rect().center();
    m_minSide = qMin(width(), height());

    update();
}

QPointF Black_White_Ball_Loading::getPointOnLine(qreal progress, qreal distance, qreal angle)
{
    qreal actualLineLength = m_minSide * (m_endPosRatio - m_startPosRatio);
    qreal startDistance = m_minSide * m_startPosRatio;

    qreal currentDistance = startDistance + (progress * actualLineLength);

    qreal dx = currentDistance * qCos(angle);
    qreal dy = currentDistance * qSin(angle);

    return QPointF(m_center.x() + dx, m_center.y() + dy);
}

void Black_White_Ball_Loading::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor("#34495E"));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());
    qreal ballRadius = m_minSide * m_ballRadiusRatio;

    for (int i = 0; i < m_ballPairCount; ++i) {
        qreal currentAngle = m_angles.at(i);

        int groupIndex = i % m_groupCount;
        qreal currentGroupProgress = m_groupProgresses.at(groupIndex);
        bool currentGroupDrawWhiteFirst = m_groupDrawWhiteFirst.at(groupIndex);


        QPointF whiteBallPos = getPointOnLine(currentGroupProgress, 0, currentAngle);
        QPointF blackBallPos = getPointOnLine(1.0 - currentGroupProgress, 0, currentAngle);

        if (currentGroupDrawWhiteFirst) {
            painter.setBrush(Qt::white);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(whiteBallPos, ballRadius, ballRadius);

            painter.setBrush(Qt::black);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(blackBallPos, ballRadius, ballRadius);
        } else {
            painter.setBrush(Qt::black);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(blackBallPos, ballRadius, ballRadius);

            painter.setBrush(Qt::white);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(whiteBallPos, ballRadius, ballRadius);
        }
    }
}

void Black_White_Ball_Loading::updateAnimation()
{
    for (int i = 0; i < m_groupCount; ++i) {
        if (m_groupForwards[i]) {
            m_groupProgresses[i] += m_animationStep;
            if (m_groupProgresses[i] >= 1.0)
            {
                m_groupProgresses[i] = 1.0;
                m_groupForwards[i] = false;
            }
            m_groupDrawWhiteFirst[i] = true;
        }
        else
        {
            m_groupProgresses[i] -= m_animationStep;
            if (m_groupProgresses[i] <= 0.0)
            {
                m_groupProgresses[i] = 0.0;
                m_groupForwards[i] = true;
            }
            m_groupDrawWhiteFirst[i] = false;
        }
    }
    update();
}