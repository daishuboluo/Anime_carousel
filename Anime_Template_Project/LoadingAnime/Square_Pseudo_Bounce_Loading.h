#ifndef SQUARE_PSEUDO_BOUNCE_LOADING_H
#define SQUARE_PSEUDO_BOUNCE_LOADING_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QPainter>
#include <QTimer>
#include <QPainterPath>

class Square_Pseudo_Bounce_Loading : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(qreal offsetY READ offsetY WRITE setOffsetY)
    Q_PROPERTY(qreal currentRotation READ currentRotation WRITE setCurrentRotation)

public:
    explicit Square_Pseudo_Bounce_Loading(QWidget *parent = nullptr);
    ~Square_Pseudo_Bounce_Loading();

    qreal offsetY() const { return m_offsetY; }
    qreal currentRotation() const { return m_currentRotation; }

    void setOffsetY(qreal offset);
    void setCurrentRotation(qreal rotation);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    qreal m_squareSize;
    qreal m_initialRotation;
    qreal m_offsetY;
    qreal m_currentRotation;
    qreal bounceHeight{20};
    QPropertyAnimation *m_offsetAnimation1;
    QPropertyAnimation *m_offsetAnimation2;
    QSequentialAnimationGroup *m_bounceGroup;
    QPropertyAnimation *m_rotationAnimation;
    QParallelAnimationGroup *m_mainAnimation;

    void setupAnimations();
    void updateSquareGeometry();
};

#endif // SQUARE_PSEUDO_BOUNCE_LOADING_H
