#ifndef BOUNCING_BALL_LOADING_H
#define BOUNCING_BALL_LOADING_H

#include <QWidget>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPainter>

#include <QObject>
#include <vector>


class Sphere : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(int widthRadius READ widthRadius WRITE setWidthRadius NOTIFY widthRadiusChanged)
    Q_PROPERTY(int heightRadius READ heightRadius WRITE setHeightRadius NOTIFY heightRadiusChanged)

    Q_PROPERTY(QPointF fallEndPosition READ fallEndPosition WRITE setFallEndPosition NOTIFY fallEndPositionChanged)

public:
    explicit Sphere(QObject *parent = nullptr);

    void startAnimation();
    void configureAnimation();

    void setStartAndEndPosition(QPointF startPosition, QPointF endPosition);

public:
    QPointF position() const;
    void setPosition(QPointF newPosition);
    int widthRadius() const;
    void setWidthRadius(int newWidthRadius);

    int heightRadius() const;
    void setHeightRadius(int newHeightRadius);

    QPointF fallEndPosition() const;
    void setFallEndPosition(QPointF newFallEndPosition);

    QColor ballColor() const { return m_ballColor; }
    void setBallColor(const QColor &color) { m_ballColor = color; }

    bool animationFinished() const { return m_animationFinished; }
    void setAnimationFinished(bool finished) { m_animationFinished = finished; }
signals:
    void positionChanged();
    void widthRadiusChanged();

    void heightRadiusChanged();

    void fallEndPositionChanged();

private:
    QPointF m_position;
    int m_widthRadius{30};
    int m_heightRadius{30};
    QPointF m_fallEndPosition;

    QColor m_ballColor = QColor(255,64,91,255);

    QPropertyAnimation *animation;
    QPropertyAnimation *animation2;
    QPropertyAnimation *animation3;
    QPropertyAnimation *animation4;

    bool m_animationFinished = true;
};

class Bouncing_Ball_Loading : public QWidget
{
    Q_OBJECT
public:
    explicit Bouncing_Ball_Loading(QWidget *parent = nullptr);

    void updateBallPositions(int ballCount);

    void setSize(QSize size);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:

private:
    int m_ballCount = 3;
    std::vector<Sphere*> m_balls;

    QTimer *m_timer;
    int m_timerCount = 0;
    int m_currentTime = 240;

    QTimer *m_updateTimer;
};

#endif // BOUNCING_BALL_LOADING_H
