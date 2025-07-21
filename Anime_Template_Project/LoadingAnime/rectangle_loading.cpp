#include "Rectangle_Loading.h"
#include <QDebug>
#include <QFont>
#include <QFontMetrics>

Rectangle_Loading::Rectangle_Loading(QWidget *parent)
    : QWidget(parent),
    m_rotationAngle(0),
    m_borderWidth(5),
    m_rotationAnimation(nullptr),
    m_borderAnimation(nullptr),
    m_dotTimer(nullptr),
    m_currentDotCount(0),
    m_drawColor(QColor(255,64,91,255))
{
    setupAnimations();

    m_dotTimer = new QTimer(this);
    connect(m_dotTimer, &QTimer::timeout, this, &Rectangle_Loading::updateLoadingDots);
    m_dotTimer->start(500);
}

Rectangle_Loading::~Rectangle_Loading()
{
    if (m_rotationAnimation) {
        m_rotationAnimation->stop();
    }
    if (m_borderAnimation) {
        m_borderAnimation->stop();
    }
    if (m_dotTimer) {
        m_dotTimer->stop();
    }
}

void Rectangle_Loading::setRotationAngle(qreal angle) {
    if (m_rotationAngle != angle) {
        m_rotationAngle = angle;
        update();
    }
}

void Rectangle_Loading::setBorderWidth(int width) {
    if (m_borderWidth != width) {
        m_borderWidth = width;
        update();
    }
}

void Rectangle_Loading::setupAnimations()
{
    m_rotationAnimation = new QPropertyAnimation(this, "rotationAngle");
    m_rotationAnimation->setStartValue(0);
    m_rotationAnimation->setEndValue(360);
    m_rotationAnimation->setDuration(3000);
    m_rotationAnimation->setLoopCount(-1);
    m_rotationAnimation->setEasingCurve(QEasingCurve::Linear);
    m_rotationAnimation->start();

    m_borderAnimation = new QPropertyAnimation(this, "borderWidth");
    m_borderAnimation->setStartValue(5);
    m_borderAnimation->setEndValue(20);
    m_borderAnimation->setDuration(1500);
    m_borderAnimation->setLoopCount(-1);
    m_borderAnimation->setDirection(QAbstractAnimation::Forward);
    m_borderAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_borderAnimation->setKeyValueAt(0, 5);
    m_borderAnimation->setKeyValueAt(0.5, 20);
    m_borderAnimation->setKeyValueAt(1, 5);
    m_borderAnimation->start();
}

void Rectangle_Loading::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(239, 239, 239, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());
    int minDimension = qMin(width(), height());

    int squareSide = minDimension / 2;

    int minBorderWidth = qMax(2, minDimension / 40);
    int maxBorderWidth = qMax(minBorderWidth + 5, minDimension / 10);

    if (m_borderAnimation->state() == QAbstractAnimation::Running) {
        m_borderAnimation->setKeyValueAt(0, minBorderWidth);
        m_borderAnimation->setKeyValueAt(0.5, maxBorderWidth);
        m_borderAnimation->setKeyValueAt(1, minBorderWidth);
    }
    m_borderWidth = qBound(minBorderWidth, m_borderWidth, maxBorderWidth);

    if (squareSide <= m_borderWidth * 2) {
        squareSide = m_borderWidth * 2 + 1;
    }

    QPen pen;
    pen.setColor(m_drawColor);
    pen.setWidth(m_borderWidth);
    pen.setJoinStyle(Qt::MiterJoin);
    painter.setPen(pen);

    painter.save();
    painter.translate(width() / 2.0, height() / 2.0);
    painter.rotate(m_rotationAngle);
    painter.drawRect(-squareSide / 2.0, -squareSide / 2.0, squareSide, squareSide);
    painter.restore();

    painter.setPen(m_drawColor);
    QFont font;
    font.setPointSize(qMax(10, minDimension / 22));
    font.setBold(true);
    painter.setFont(font);

    QString baseText = "Loading";
    QString dots = "";
    for (int i = 0; i < m_currentDotCount; ++i) {
        dots += ".";
    }
    QString displayText = baseText + dots;

    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(displayText);
    int textHeight = fm.height();

    int x = (width() - textWidth) / 2;
    int y = (height() - textHeight) / 2 + fm.ascent();
    painter.drawText(x, y, displayText);
}

void Rectangle_Loading::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    update();
}

void Rectangle_Loading::updateLoadingDots()
{
    m_currentDotCount = (m_currentDotCount + 1) % 4;
    update();
}