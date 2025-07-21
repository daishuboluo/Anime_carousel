#ifndef BLACK_WHITE_BALL_LOADING_H
#define BLACK_WHITE_BALL_LOADING_H

#include <QWidget>
#include <QTimer>
#include <QPointF>
#include <QVector>

class Black_White_Ball_Loading : public QWidget
{
    Q_OBJECT

public:
    explicit Black_White_Ball_Loading(QWidget *parent = nullptr);
    ~Black_White_Ball_Loading();

    void setBallPairCount(int count);
    void setAnimationRatios(qreal startRatio, qreal endRatio);
    void setBallRadiusRatio(qreal ratio);
    void setAnimationSpeed(qreal speed);
    void setGroupCount(int count);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateAnimation();

private:
    QTimer *m_timer;

    int m_ballPairCount;
    qreal m_startPosRatio;
    qreal m_endPosRatio;
    qreal m_ballRadiusRatio;
    qreal m_animationStep;

    QPointF m_center;
    qreal m_minSide;

    QVector<qreal> m_angles;

    int m_groupCount;
    QVector<qreal> m_groupProgresses;
    QVector<bool> m_groupForwards;
    QVector<bool> m_groupDrawWhiteFirst;

    QPointF getPointOnLine(qreal progress, qreal distance, qreal angle);
    void updateAngles();
    void initializeGroupData();
};

#endif // BLACK_WHITE_BALL_LOADING_H