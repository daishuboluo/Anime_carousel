#include "diffusion_button.h"

Diffusion_button::Diffusion_button(QWidget* parent) : QPushButton{ parent }
{

    this->setCursor(Qt::PointingHandCursor);

    this->resize(130, 42);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 16); // 设置阴影偏移量
    shadow->setBlurRadius(50); // 设置阴影模糊半径
    shadow->setColor(QColor(0, 0, 0, 166)); // 设置阴影颜色
    this->setGraphicsEffect(shadow); // 将阴影效果应用到按钮上

    animation3 = new QPropertyAnimation(this, "radius");
    animation3->setDuration(400);
    animation3->setStartValue(m_radius);
    animation3->setEndValue(this->width());
    animation3->setEasingCurve(QEasingCurve::Linear);
    animation3->setDirection(QAbstractAnimation::Forward);

    animation1 = new QPropertyAnimation(this, "opacity");
    animation1->setDuration(400);
    animation1->setStartValue(m_opacity);
    animation1->setEndValue(0);
    animation1->setEasingCurve(QEasingCurve::Linear);
    animation1->setDirection(QAbstractAnimation::Forward);

    connect(animation3, &QPropertyAnimation::finished, this, &Diffusion_button::reset_animation);
    connect(animation1, &QPropertyAnimation::finished, this, &Diffusion_button::reset_animation);
}

void Diffusion_button::draw_disappearing_circle() // 绘制 扩散圆
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen); // 设置画笔为无，即不绘制边框

    QBrush brush(QColor(135, 206, 235, m_opacity)); // 设置填充颜色为 color
    painter.setBrush(brush);
    painter.drawEllipse(mouse_coordinates, m_radius, m_radius); // 绘制一个以 point 为中心，半径为 radius 的圆
}

void Diffusion_button::execute_animation() // 执行动画
{
    animation3->start();
    animation1->start();
}

void Diffusion_button::reset_animation() // 重置动画
{
    m_radius = 0;
    m_opacity = 255;
}

void Diffusion_button::paintEvent(QPaintEvent *event) // 绘制按钮
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());

    // 裁剪路径
    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), 21, 21);
    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);

    // 绘制纯白矩形
    QBrush Brush(QColor(255,255,255,255));
    painter.setBrush(Brush);
    painter.drawRect(0, 0, width(), height()); 

    // 绘制扩散圆
    this->draw_disappearing_circle(); 
}

void Diffusion_button::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mouse_coordinates = event->pos();
        update();
        execute_animation();
    }
}

int Diffusion_button::radius() const
{
    return m_radius;
}

void Diffusion_button::setRadius(int newRadius)
{
    if (m_radius == newRadius)
        return;
    m_radius = newRadius;
    update();
    emit radiusChanged();
}

int Diffusion_button::opacity() const
{
    return m_opacity;
}

void Diffusion_button::setOpacity(int newOpacity)
{
    if (m_opacity == newOpacity)
        return;
    m_opacity = newOpacity;
    update();
    emit opacityChanged();
}
