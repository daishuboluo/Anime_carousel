// line_loading.cpp
#include "line_loading.h"
#include <QPainter>
#include <QDebug>
#include <QtMath>
#include <QScreen>
#include <QGuiApplication>

M_Line::M_Line(QObject *parent)
    : QObject(parent)
{
    m_animation = new QPropertyAnimation(this, "angle");
    m_animation->setDuration(1100);
    m_animation->setStartValue(0.0);
    m_animation->setEndValue(90.0);
    m_animation->setEasingCurve(QEasingCurve::Linear);

    connect(m_animation, &QPropertyAnimation::finished, this,[=]{
        m_isFinished = true;
    });


}

qreal M_Line::angle() const
{
    return m_angle;
}

void M_Line::setAngle(qreal newAngle)
{
    if (m_angle != newAngle) {
        m_angle = newAngle;
    }
}

void M_Line::startAnimation()
{
    m_isFinished = false;
    m_animation->setDirection(QAbstractAnimation::Forward);
    m_animation->start();
}



void M_Line::resetAnimation()
{
    m_isFinished = false;
    m_animation->setDirection(QAbstractAnimation::Backward);
    m_animation->start();
}

bool M_Line::isFinished() const
{
    return m_isFinished;
}


Line_Loading::Line_Loading(QWidget *parent)
    : QWidget{parent}
{
    generateLines();
    animation = new QPropertyAnimation(this, "Gradient_Position");
    animation->setDuration(5000);
    animation->setStartValue(0);
    animation->setEndValue(360);
    animation->setLoopCount(-1);
    animation->setEasingCurve(QEasingCurve::Linear);
    
    animation->start();
    m_timer = new QTimer(this);
    m_timer2 = new QTimer(this);

    connect(m_timer, &QTimer::timeout, this, &Line_Loading::onTimeout);
    connect(m_timer2, &QTimer::timeout, this, [=]{
        if (m_elapsedTimer.elapsed() > m_overTime )
        {
            if (!m_lines[0]->isFinished()) 
            {
                qDebug() << "最前面的动画没有执行完";
                return;
            }
            if (!m_lines[m_lines.size()-1]->isFinished())
            {
                qDebug() << "最后面的动画没有执行完";
                return;
            }


            qDebug() << "执行了计时器";
            m_elapsedTimer.invalidate();
            m_timer->start(80);
        }

        this->update();

    });
    m_timer->start(80);
    m_timer2->start(16);
}

Line_Loading::~Line_Loading()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void Line_Loading::generateLines()
{
    for (int i = 0; i < 20; ++i)
    {
        M_Line *line = new M_Line(this);
        m_lines.push_back(line);
    }
}

void Line_Loading::onTimeout()
{
    if (m_count >= m_lines.size())
    {
        m_count = 0;
        m_count2 = m_lines.size() - 1;
        m_timer->stop();
        m_direction = !m_direction;
        m_elapsedTimer.start();
        return;
    }
    if (m_count2 < 0)
    {
        m_count2 = m_lines.size() - 1;
        m_direction = !m_direction;
        m_elapsedTimer.start();
        m_timer->stop();

        return;
    }

    if (m_direction)
    {
        m_lines[m_count]->startAnimation();
        m_count++;
    }
    else
    {
        m_lines[m_count2]->resetAnimation();
        m_count2--;
    }


}

void Line_Loading::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(239, 239, 239, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    const qreal LINE_WIDTH_RATIO_TO_HEIGHT = 0.2;
    const int total_lines = m_lines.size();
    if (total_lines == 0)
    {
        return;
    }

    QConicalGradient gradient(width() / 2, height(), -m_Gradient_Position);
    gradient.setColorAt(0, QColor( 255, 167, 69, 255));
    gradient.setColorAt(0.125, QColor(254, 134, 159, 255));
    gradient.setColorAt(0.25, QColor(239, 122, 200, 255));
    gradient.setColorAt(0.375, QColor(160, 131, 237, 255));
    gradient.setColorAt(0.5, QColor(67, 174, 255, 255));
    gradient.setColorAt(0.625, QColor(160, 131, 237, 255));
    gradient.setColorAt(0.75, QColor(239, 122, 200, 255));
    gradient.setColorAt(0.875, QColor(254, 134, 159, 255));
    gradient.setColorAt(1, QColor(255, 167, 69, 255));

    const qreal ideal_line_height_by_height = height() * 0.7;
    const qreal target_total_width_at_90_deg = width() * 0.8;

    qreal max_line_height_by_width;
    if (total_lines > 1) {
        max_line_height_by_width = target_total_width_at_90_deg / (total_lines - 1.0 + LINE_WIDTH_RATIO_TO_HEIGHT);
    } else {
        max_line_height_by_width = target_total_width_at_90_deg;
    }

    const qreal line_height = qMin(ideal_line_height_by_height, max_line_height_by_width);
    qreal line_width = line_height * LINE_WIDTH_RATIO_TO_HEIGHT;

    if (line_width < 1.0) {
        line_width = 1.0;
    }

    const qreal widget_center_y = height() / 2.0;
    const qreal fixed_bottom_y = widget_center_y + (line_height / 2.0);
    const qreal fixed_horizontal_pitch = line_height;
    const qreal total_bottom_points_span = (total_lines - 1) * fixed_horizontal_pitch + line_width;
    const qreal start_pivot_x = (width() - total_bottom_points_span) / 2.0;

    QPainterPath combinedClipPath;

    for (int i = 0; i < total_lines; ++i)
    {
        qreal lineAngle = m_lines[i]->angle();
        qreal current_pivot_x = start_pivot_x + i * fixed_horizontal_pitch;

        QTransform transform;
        transform.translate(current_pivot_x, fixed_bottom_y);
        transform.rotate(lineAngle);

        QRectF rectInLocalCoords(-line_width / 2.0, -line_height, line_width, line_height /*+ 1.0*/);

        QPainterPath singleLinePath;
        singleLinePath.addRect(rectInLocalCoords);

        QPainterPath transformedSingleLinePath = transform.map(singleLinePath);

        combinedClipPath.addPath(transformedSingleLinePath);
    }

    painter.setClipPath(combinedClipPath);


    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());


}

int Line_Loading::Gradient_Position() const
{
    return m_Gradient_Position;
}

void Line_Loading::setGradient_Position(int newGradient_Position)
{
    m_Gradient_Position = newGradient_Position;
}