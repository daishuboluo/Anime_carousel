#include "Square_Pseudo_Bounce_Loading.h"
#include <QDebug>

Square_Pseudo_Bounce_Loading::Square_Pseudo_Bounce_Loading(QWidget *parent)
    : QWidget{parent},
    m_squareSize(0),
    m_initialRotation(45.0),
    m_offsetY(0),
    m_currentRotation(0),
    m_offsetAnimation1(nullptr),
    m_offsetAnimation2(nullptr),
    m_bounceGroup(nullptr),
    m_rotationAnimation(nullptr),
    m_mainAnimation(nullptr)
{
    setAttribute(Qt::WA_TranslucentBackground);
    QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setupAnimations();
}

Square_Pseudo_Bounce_Loading::~Square_Pseudo_Bounce_Loading()
{
    if (m_mainAnimation) {
        m_mainAnimation->stop();
        delete m_mainAnimation;
    }
}

void Square_Pseudo_Bounce_Loading::setOffsetY(qreal offset)
{
    if (m_offsetY == offset)
        return;
    m_offsetY = offset;
    update();
}

void Square_Pseudo_Bounce_Loading::setCurrentRotation(qreal rotation)
{
    if (m_currentRotation == rotation)
        return;
    m_currentRotation = rotation;
    update();
}

void Square_Pseudo_Bounce_Loading::setupAnimations()
{

    m_offsetAnimation1 = new QPropertyAnimation(this, "offsetY");
    m_offsetAnimation1->setStartValue(0.0);
    m_offsetAnimation1->setEndValue(-bounceHeight);
    m_offsetAnimation1->setDuration(350);
    m_offsetAnimation1->setEasingCurve(QEasingCurve::OutQuad);

    m_offsetAnimation2 = new QPropertyAnimation(this, "offsetY");
    m_offsetAnimation2->setStartValue(-bounceHeight);
    m_offsetAnimation2->setEndValue(0.0);
    m_offsetAnimation2->setDuration(350);
    m_offsetAnimation2->setEasingCurve(QEasingCurve::InQuad);

    m_bounceGroup = new QSequentialAnimationGroup(this);
    m_bounceGroup->addAnimation(m_offsetAnimation1);
    m_bounceGroup->addAnimation(m_offsetAnimation2);
    m_bounceGroup->setLoopCount(-1);

    m_rotationAnimation = new QPropertyAnimation(this, "currentRotation");
    m_rotationAnimation->setStartValue(0.0);
    m_rotationAnimation->setEndValue(360.0);
    m_rotationAnimation->setDuration(2800);
    m_rotationAnimation->setLoopCount(-1);
    m_rotationAnimation->setEasingCurve(QEasingCurve::Linear);

    m_mainAnimation = new QParallelAnimationGroup(this);
    m_mainAnimation->addAnimation(m_bounceGroup);
    m_mainAnimation->addAnimation(m_rotationAnimation);
    m_mainAnimation->setLoopCount(-1);

    m_mainAnimation->start();
}

void Square_Pseudo_Bounce_Loading::updateSquareGeometry()
{
    qreal minDim = qMin(width(), height());
    m_squareSize = minDim / 7.0;

    update();
}

void Square_Pseudo_Bounce_Loading::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(96, 151, 226, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());


    QImage blur_image1(this->size(), QImage::Format_ARGB32);
    blur_image1.fill(QColor(0, 0, 0, 0));
    QPainter image_painter1(&blur_image1);
    image_painter1.setRenderHint(QPainter::Antialiasing);
    image_painter1.setPen(Qt::NoPen);
    image_painter1.setBrush(QColor(117, 117, 117, 133));
    image_painter1.drawEllipse(width() / 2.0 -m_squareSize/2,height() / 2.0 + m_squareSize * 0.36,m_squareSize,4);
    image_painter1.end();
    painter.drawImage(QRect(0, 0, width(), height()),blur_image1);


    painter.save();

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);

    qreal centerX = width() / 2.0;
    qreal centerY = height() / 2.0 + m_offsetY;


    QPainterPath path;
    path.addEllipse(width() / 2.0 - height() / 4.3,
                            0 + m_squareSize / 4.0 + -m_offsetY * 0.8,
                            height() / 2.0,
                            height() / 2.0);

    painter.setClipPath(path);


    painter.translate(centerX, centerY);

    painter.rotate(m_initialRotation + m_currentRotation);

    painter.drawRoundedRect(QRectF(-m_squareSize / 2.0, -m_squareSize / 2.0, m_squareSize, m_squareSize),m_squareSize / 10,m_squareSize / 10);

    painter.restore();


}

void Square_Pseudo_Bounce_Loading::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    updateSquareGeometry();
}
