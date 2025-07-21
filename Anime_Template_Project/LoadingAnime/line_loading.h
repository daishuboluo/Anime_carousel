#ifndef LINE_LOADING_H
#define LINE_LOADING_H

#include <QWidget>
#include <vector>
#include <QPainter>
#include <QPaintEvent>
#include <QObject>
#include <QTimer>
#include <QPropertyAnimation>
#include <QTimer>
#include <QElapsedTimer>
#include <QPainterPath>


class M_Line : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal angle READ angle WRITE setAngle)

public:
    explicit M_Line(QObject *parent = nullptr);

    void startAnimation();
    void resetAnimation();

    qreal angle() const;
    void setAngle(qreal newAngle);

    bool isFinished() const;
private:
    qreal m_angle{0.0};
    bool m_isFinished{true};

    QPropertyAnimation *m_animation;
};


class Line_Loading : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int Gradient_Position READ Gradient_Position WRITE setGradient_Position)

public:
    explicit Line_Loading(QWidget *parent = nullptr);
    ~Line_Loading();

    int Gradient_Position() const;
    void setGradient_Position(int newGradient_Position);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onTimeout();

private:
    std::vector<M_Line*> m_lines;
    void generateLines();

    QTimer *m_timer;
    QTimer *m_timer2;
    int m_count{0};
    int m_count2{0};
    bool m_direction{true};
    bool m_isFinished{false};

    QElapsedTimer m_elapsedTimer;

    qint64 m_overTime{3500};
    QPropertyAnimation *animation;


    int m_Gradient_Position;

signals:
};

#endif // LINE_LOADING_H
