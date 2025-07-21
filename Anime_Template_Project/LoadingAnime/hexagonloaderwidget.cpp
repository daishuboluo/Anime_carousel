#include "HexagonLoaderWidget.h"
#include <QPainter>
#include <QDebug>
#include <QtMath>
#include <QTimer>
#include <QAbstractAnimation>

HexBrick::HexBrick(QColor color, qreal rotation, QObject* parent)
    : QObject(parent), m_color(color), m_rotation(rotation)
{
}

void HexBrick::setColor(const QColor& color)
{
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
    }
}

Gel::Gel(const QPointF& offset, qreal delay)
    : m_offset(offset), m_scale(1.0), m_animationDelay(delay)
{
    m_hexBricks.push_back(new HexBrick(Qt::black, 0.0, nullptr));
    m_hexBricks.push_back(new HexBrick(Qt::black, 60.0, nullptr));
    m_hexBricks.push_back(new HexBrick(Qt::black, -60.0, nullptr));
}

HexagonLoaderWidget::HexagonLoaderWidget(QWidget *parent)
    : QWidget(parent),
    m_centerGelCurrentScale(1.0),
    m_r1CurrentScale(1.0),
    m_r2CurrentScale(1.0),
    m_r3CurrentScale(1.0),
    m_currentGlobalScale(1.0)
{
    setupGels();
    setupAnimations();
}

HexagonLoaderWidget::~HexagonLoaderWidget()
{
    qDeleteAll(m_pulseAnimations);
    qDeleteAll(m_fadeAnimations);

    for (Gel& gel : m_gels) {
        qDeleteAll(gel.hexBricks());
        gel.hexBricks().clear();
    }
}

qreal HexagonLoaderWidget::centerGelScale() const { return m_centerGelCurrentScale; }
void HexagonLoaderWidget::setCenterGelScale(qreal scale)
{
    if (!qFuzzyCompare(m_centerGelCurrentScale, scale)) {
        m_centerGelCurrentScale = scale;
        if (!m_gels.empty()) {
            m_gels[0].setScale(scale);
        }
        emit centerGelScaleChanged();
        update();
    }
}

qreal HexagonLoaderWidget::r1Scale() const { return m_r1CurrentScale; }
void HexagonLoaderWidget::setR1Scale(qreal scale)
{
    if (!qFuzzyCompare(m_r1CurrentScale, scale)) {
        m_r1CurrentScale = scale;
        for (int i = 1; i <= 6; ++i) {
            m_gels[i].setScale(scale);
        }
        emit r1ScaleChanged();
        update();
    }
}

qreal HexagonLoaderWidget::r2Scale() const { return m_r2CurrentScale; }
void HexagonLoaderWidget::setR2Scale(qreal scale)
{
    if (!qFuzzyCompare(m_r2CurrentScale, scale)) {
        m_r2CurrentScale = scale;
        for (int i = 7; i <= 18; ++i) {
            m_gels[i].setScale(scale);
        }
        emit r2ScaleChanged();
        update();
    }
}

qreal HexagonLoaderWidget::r3Scale() const { return m_r3CurrentScale; }
void HexagonLoaderWidget::setR3Scale(qreal scale)
{
    if (!qFuzzyCompare(m_r3CurrentScale, scale)) {
        m_r3CurrentScale = scale;
        for (int i = 19; i < m_gels.size(); ++i) {
            m_gels[i].setScale(scale);
        }
        emit r3ScaleChanged();
        update();
    }
}

void HexagonLoaderWidget::setupGels()
{
    m_gels.push_back(Gel(QPointF(0, 0), 0.0));

    m_gels.push_back(Gel(QPointF(-32, 0), 0.2));
    m_gels.push_back(Gel(QPointF(-16, -28), 0.2));
    m_gels.push_back(Gel(QPointF(16, -28), 0.2));
    m_gels.push_back(Gel(QPointF(32, 0), 0.2));
    m_gels.push_back(Gel(QPointF(-16, 28), 0.2));
    m_gels.push_back(Gel(QPointF(16, 28), 0.2));

    
    m_gels.push_back(Gel(QPointF(-48, -28), 0.4));
    m_gels.push_back(Gel(QPointF(48, -28), 0.4));
    m_gels.push_back(Gel(QPointF(0, 56), 0.4));
    m_gels.push_back(Gel(QPointF(-48, 28), 0.4));
    m_gels.push_back(Gel(QPointF(48, 28), 0.4));
    m_gels.push_back(Gel(QPointF(0, -56), 0.4));
    m_gels.push_back(Gel(QPointF(-32, -56), 0.4));
    m_gels.push_back(Gel(QPointF(32, -56), 0.4));
    m_gels.push_back(Gel(QPointF(-32, 56), 0.4));
    m_gels.push_back(Gel(QPointF(32, 56), 0.4));
    m_gels.push_back(Gel(QPointF(-64, 0), 0.4));
    m_gels.push_back(Gel(QPointF(64, 0), 0.4));

    m_gels.push_back(Gel(QPointF(-48, -84), 0.6));
    m_gels.push_back(Gel(QPointF(48, -84), 0.6));
    m_gels.push_back(Gel(QPointF(16, -84), 0.6));
    m_gels.push_back(Gel(QPointF(-16, -84), 0.6));
    m_gels.push_back(Gel(QPointF(-48, 84), 0.6)); 
    m_gels.push_back(Gel(QPointF(-48, 84), 0.6));
    m_gels.push_back(Gel(QPointF(48, 84), 0.6));
    m_gels.push_back(Gel(QPointF(16, 84), 0.6));
    m_gels.push_back(Gel(QPointF(-16, 84), 0.6));
    m_gels.push_back(Gel(QPointF(-80, -28), 0.6));
    m_gels.push_back(Gel(QPointF(-80, 28), 0.6));
    m_gels.push_back(Gel(QPointF(64, 56), 0.6));
    m_gels.push_back(Gel(QPointF(-64, -56), 0.6));
    m_gels.push_back(Gel(QPointF(-96, 0), 0.6));
    m_gels.push_back(Gel(QPointF(80, -28), 0.6));
    m_gels.push_back(Gel(QPointF(80, 28), 0.6));
    m_gels.push_back(Gel(QPointF(-64, 56), 0.6));
    m_gels.push_back(Gel(QPointF(64, -56), 0.6));
    m_gels.push_back(Gel(QPointF(96, 0), 0.6));
}

void HexagonLoaderWidget::setupAnimations()
{
    QPropertyAnimation* centerPulse = new QPropertyAnimation(this, "centerGelScale", this);
    centerPulse->setDuration(2000);
    centerPulse->setLoopCount(-1);
    centerPulse->setKeyValueAt(0, 1.0);
    centerPulse->setKeyValueAt(0.5, 0.01);
    centerPulse->setKeyValueAt(1, 1.0);
    m_pulseAnimations.append(centerPulse);

    QPropertyAnimation* r1Pulse = new QPropertyAnimation(this, "r1Scale", this);
    r1Pulse->setDuration(2000);
    r1Pulse->setLoopCount(-1);
    r1Pulse->setKeyValueAt(0, 1.0);
    r1Pulse->setKeyValueAt(0.5, 0.01);
    r1Pulse->setKeyValueAt(1, 1.0);
    m_pulseAnimations.append(r1Pulse);

    QPropertyAnimation* r2Pulse = new QPropertyAnimation(this, "r2Scale", this);
    r2Pulse->setDuration(2000);
    r2Pulse->setLoopCount(-1);
    r2Pulse->setKeyValueAt(0, 1.0);
    r2Pulse->setKeyValueAt(0.5, 0.01);
    r2Pulse->setKeyValueAt(1, 1.0);
    m_pulseAnimations.append(r2Pulse);

    QPropertyAnimation* r3Pulse = new QPropertyAnimation(this, "r3Scale", this);
    r3Pulse->setDuration(2000);
    r3Pulse->setLoopCount(-1);
    r3Pulse->setKeyValueAt(0, 1.0);
    r3Pulse->setKeyValueAt(0.5, 0.01);
    r3Pulse->setKeyValueAt(1, 1.0);
    m_pulseAnimations.append(r3Pulse);

    for (Gel& gel : m_gels) {
        for (HexBrick* brick : gel.hexBricks()) {
            QPropertyAnimation* brickFade = new QPropertyAnimation(brick, "color", this);
            brickFade->setDuration(2000);
            brickFade->setLoopCount(-1);
            brickFade->setKeyValueAt(0, QColor("#252525"));
            brickFade->setKeyValueAt(0.5, QColor("#000000"));
            brickFade->setKeyValueAt(1, QColor("#353535"));
            m_fadeAnimations.append(brickFade);
        }
    }

    QTimer::singleShot(0, this, [centerPulse]() { centerPulse->start(); });
    QTimer::singleShot(200, this, [r1Pulse]() { r1Pulse->start(); });
    QTimer::singleShot(400, this, [r2Pulse]() { r2Pulse->start(); });
    QTimer::singleShot(600, this, [r3Pulse]() { r3Pulse->start(); });

    int fadeAnimIndex = 0;
    for (const Gel& gel : m_gels) {
        for (int j = 0; j < gel.hexBricks().size(); ++j) {
            if (fadeAnimIndex < m_fadeAnimations.size()) {
                QPropertyAnimation* brickFade = m_fadeAnimations[fadeAnimIndex];
                QTimer::singleShot(static_cast<int>(gel.animationDelay() * 1000), this, [brickFade]() { brickFade->start(); });
                fadeAnimIndex++;
            }
        }
    }
}

void HexagonLoaderWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor("#282A50"));

    painter.translate(width() / 2, height() / 2);

    painter.scale(m_currentGlobalScale, m_currentGlobalScale);

    const qreal HEX_BRICK_WIDTH = 30.0;
    const qreal HEX_BRICK_HEIGHT = 17.0;

    for (const Gel& gel : m_gels) {
        painter.save();
        painter.translate(gel.offset());
        painter.scale(gel.scale(), gel.scale());

        for (const HexBrick* brick : gel.hexBricks()) {
            drawHexagon(painter, QPointF(0, 0), HEX_BRICK_WIDTH / 2.0, brick->color(), brick->rotation());
        }
        painter.restore();
    }
}

void HexagonLoaderWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    int newWidth = width();
    int newHeight = height();

    const qreal originalDesignDimension = 200.0;
    qreal scaleFactor = qMin(newWidth, newHeight) / originalDesignDimension;

    m_currentGlobalScale = qMax(0.5, qMin(scaleFactor, 2.0));

    update();
    QWidget::resizeEvent(event);
}

QPointF HexagonLoaderWidget::calculateHexagonPoint(qreal size, qreal angleDegrees) {
    qreal angleRad = qDegreesToRadians(angleDegrees);
    return QPointF(size * qCos(angleRad), size * qSin(angleRad));
}

void HexagonLoaderWidget::drawHexagon(QPainter& painter, const QPointF& center, qreal size, const QColor& color, qreal rotation)
{
    qreal rectWidth = 30.0;
    qreal rectHeight = 17.0;

    painter.save();

    painter.translate(center);
    painter.rotate(rotation);

    QPointF brickCenterOffset(0, -1.5);
    painter.translate(brickCenterOffset);

    painter.setBrush(color);
    painter.setPen(Qt::NoPen);

    painter.drawRect(-rectWidth / 2, -rectHeight / 2, rectWidth, rectHeight);

    painter.restore();
}
