#include "ring_loading.h"
#include <QPainter>
#include <QPen>
#include <QResizeEvent>
#include <QtMath>


Ring_Loading::Ring_Loading(QWidget *parent)
    : QWidget(parent)
{
    m_timer = new QTimer(this);

    setRingWidth(10);
    setMargin(5);


    connect(m_timer, &QTimer::timeout, this, [=]{
        Speed += 0.5;
        m_Inner_Arc_Start_Angle -= 16 * Speed;
        if (m_Inner_Arc_Start_Angle >= 360 * 16)
        {
            m_Inner_Arc_Start_Angle = 0;

        }
        update();
    });

    setInner_Arc_End_Angle(40);
}


void Ring_Loading::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());

    m_centerX = side / 2;

    qreal actualMargin = m_margin + m_ringWidth / 2.0;

    QRectF rect( (width() - side) / 2.0, (height() - side) / 2.0, side, side);
    rect.adjust(actualMargin, actualMargin, -actualMargin, -actualMargin);

    if (rect.isEmpty()) {
        return;
    }

    QPen pen;
    pen.setColor(m_ringColor);
    pen.setWidth(m_ringWidth);
    pen.setCapStyle(Qt::RoundCap);

    painter.setPen(pen);


    painter.drawArc(rect, m_startAngle, m_spanAngle);
    painter.drawArc(rect, m_startAngle + 120 * 16 , m_spanAngle);
    painter.drawArc(rect, m_startAngle + 240 * 16, m_spanAngle);


    painter.setPen(m_ringColor);
    painter.setFont(QFont("Arial", side / 20));
    painter.drawText(rect, Qt::AlignCenter, "Loading");


    painter.setFont(QFont("Arial", side / 40));
    QString text = QString::number(qreal(m_Progress_Ring) / 16.0 / 360.0 * 100.0, 'f', 0) + "%";
    painter.drawText(QRectF(rect.x() ,rect.y() + side / 20,rect.width(),rect.height()), Qt::AlignCenter, text);


    QPen innerPen;
    innerPen.setColor(QColor(255, 255, 255, m_Inner_Arc_Alpha));
    innerPen.setWidth(m_ringWidth);
    innerPen.setCapStyle(Qt::RoundCap);
    painter.setPen(innerPen);

    int innerMargin = m_ringWidth * 2.0;

    rect.adjust(innerMargin , innerMargin , - innerMargin, - innerMargin);
    painter.drawArc(rect, m_startAngle + m_Inner_Arc_Start_Angle, m_Inner_Arc_End_Angle);
    painter.drawArc(rect, m_startAngle + m_Inner_Arc_Start_Angle + 120 * 16 , m_Inner_Arc_End_Angle);
    painter.drawArc(rect, m_startAngle + m_Inner_Arc_Start_Angle + 240 * 16, m_Inner_Arc_End_Angle);


    QPen outerPen;
    outerPen.setColor(QColor(255, 255, 255, 255));
    outerPen.setWidth(m_ringWidth * 2);
    outerPen.setCapStyle(Qt::RoundCap);
    painter.setPen(outerPen);


    int outerMargin = m_ringWidth * 3.0;
    rect.adjust(-outerMargin, -outerMargin , outerMargin, outerMargin);

    painter.drawArc(rect, -m_Arc_Position_Value, -m_Progress_Ring);


}

void Ring_Loading::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    update();
    QWidget::resizeEvent(event);
}


void Ring_Loading::setMargin(int margin)
{
    margin = qBound(0, margin, qMin(width(), height()) / 2 - m_ringWidth);
    if (m_margin != margin) {
        m_margin = margin;
        emit marginChanged(m_margin);
        update();
    }
}

void Ring_Loading::setRingColor(const QColor &color)
{
    if (m_ringColor != color) {
        m_ringColor = color;
        emit ringColorChanged(m_ringColor);
        update();
    }
}

void Ring_Loading::setStartAngle(int angle)
{
    int newStart = angle * 16;
    if (m_startAngle != newStart) {
        m_startAngle = newStart;
        emit startAngleChanged(angle);
        update();
    }
}

void Ring_Loading::setEndAngle(int angle)
{
    int newSpan = (angle - (m_startAngle / 16)) * 16;
    if (newSpan < 0) {
        newSpan += 360 * 16;
    }

    if (m_spanAngle != newSpan) {
        m_spanAngle = newSpan;
        emit endAngleChanged(angle);
        update();
    }
}

void Ring_Loading::setRingWidth(int width)
{
    width = qMax(1, width);
    if (m_ringWidth != width) {
        m_ringWidth = width;
        emit ringWidthChanged(m_ringWidth);
        update();
    }
}

void Ring_Loading::startAnimation()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "startAngle");
    animation->setDuration(2000);
    animation->setStartValue(0);
    animation->setEndValue(360);

    QPropertyAnimation *animation2 = new QPropertyAnimation(this, "endAngle");
    animation2->setDuration(2000);
    animation2->setStartValue(0);
    animation2->setEndValue(489);

    QPropertyAnimation *animation3 = new QPropertyAnimation(this, "margin");
    animation3->setDuration(2000);
    animation3->setStartValue(m_centerX);
    animation3->setEndValue(50);

    QPropertyAnimation *animation4 = new QPropertyAnimation(this, "ringColor");
    animation4->setDuration(1500);
    animation4->setStartValue(QColor(255,255,255,0));
    animation4->setEndValue(QColor(255,255,255,255));


    animation->start(QAbstractAnimation::DeleteWhenStopped);
    animation2->start(QAbstractAnimation::DeleteWhenStopped);
    animation3->start(QAbstractAnimation::DeleteWhenStopped);
    animation4->start(QAbstractAnimation::DeleteWhenStopped);

    connect(animation2, &QPropertyAnimation::finished, this, &Ring_Loading::startAnimation2);
    connect(animation4, &QPropertyAnimation::finished, this,[=]{
        QPropertyAnimation *animation5 = new QPropertyAnimation(this, "ringColor");
        animation5->setDuration(200);
        animation5->setStartValue(QColor(255,255,255,0));
        animation5->setEndValue(QColor(255,255,255,255));
        animation5->start(QAbstractAnimation::DeleteWhenStopped);

        connect(animation5, &QPropertyAnimation::stateChanged, this, [=]{
            QPropertyAnimation *animation6 = new QPropertyAnimation(this, "Inner_Arc_Alpha");
            animation6->setDuration(300);
            animation6->setStartValue(0);
            animation6->setEndValue(255);
            animation6->start(QAbstractAnimation::DeleteWhenStopped);

        });
    });
}

void Ring_Loading::startAnimation2()
{

    QPropertyAnimation *animation = new QPropertyAnimation(this, "Progress_Ring");
    animation->setDuration(2000);
    animation->setStartValue(0);
    animation->setEndValue(360);

    QPropertyAnimation *animation2 = new QPropertyAnimation(this, "Arc_Position_Value");
    animation2->setDuration(2000);
    animation2->setStartValue(0);
    animation2->setEndValue(180);


    connect(animation, &QPropertyAnimation::stateChanged, this,[=](QAbstractAnimation::State newState){
        if (newState == QAbstractAnimation::Running)
        {
            m_timer->start(15);
        }
    });
    connect(animation, &QPropertyAnimation::finished, this,[=]{
        m_timer->stop();
        m_Inner_Arc_Alpha = 0;
        m_Progress_Ring = 0;


        update();
        startAnimation();
    });


    animation->start(QAbstractAnimation::DeleteWhenStopped);
    animation2->start(QAbstractAnimation::DeleteWhenStopped);
}


int Ring_Loading::name() const
{
    return m_Progress_Ring / 16;
}

void Ring_Loading::setname(int newProgress_Ring)
{
    if (m_Progress_Ring == newProgress_Ring * 16)
        return;
    m_Progress_Ring = newProgress_Ring * 16;
    update();
    emit nameChanged();
}

int Ring_Loading::Inner_Arc_Start_Angle() const
{
    return m_Inner_Arc_Start_Angle / 16;
}

void Ring_Loading::setInner_Arc_Start_Angle(int newInner_Arc_Start_Angle)
{
    if (m_Inner_Arc_Start_Angle == newInner_Arc_Start_Angle * 16)
        return;
    m_Inner_Arc_Start_Angle = newInner_Arc_Start_Angle * 16;
    emit Inner_Arc_Start_AngleChanged();
}

int Ring_Loading::Inner_Arc_End_Angle() const
{
    return m_Inner_Arc_End_Angle / 16;
}

void Ring_Loading::setInner_Arc_End_Angle(int newInner_Arc_End_Angle)
{
    if (m_Inner_Arc_End_Angle == newInner_Arc_End_Angle * 16)
        return;
    m_Inner_Arc_End_Angle = newInner_Arc_End_Angle * 16;
    update();
    emit Inner_Arc_End_AngleChanged();
}


int Ring_Loading::Inner_Arc_Alpha() const
{
    return m_Inner_Arc_Alpha;
}

void Ring_Loading::setInner_Arc_Alpha(int newInner_Arc_Alpha)
{
    if (m_Inner_Arc_Alpha == newInner_Arc_Alpha)
        return;
    m_Inner_Arc_Alpha = newInner_Arc_Alpha;
    update();
    emit Inner_Arc_AlphaChanged();
}

int Ring_Loading::Arc_Position_Value() const
{
    return m_Arc_Position_Value / 16;
}

void Ring_Loading::setArc_Position_Value(int newArc_Position_Value)
{
    if (m_Arc_Position_Value == newArc_Position_Value * 16)
        return;
    m_Arc_Position_Value = newArc_Position_Value * 16;
    update();
}