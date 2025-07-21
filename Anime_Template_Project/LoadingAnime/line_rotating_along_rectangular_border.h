#ifndef LINE_ROTATING_ALONG_RECTANGULAR_BORDER_H
#define LINE_ROTATING_ALONG_RECTANGULAR_BORDER_H

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <QPropertyAnimation>

class Line_Rotating_Along_Rectangular_Border : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(qreal rotationAngle READ getRotationAngle WRITE setRotationAngle NOTIFY rotationAngleChanged)

public:
    explicit Line_Rotating_Along_Rectangular_Border(QWidget *parent = nullptr);

    qreal getRotationAngle() const;
    void setRotationAngle(qreal angle);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QRect m_squareRect;
    int m_borderThickness;
    qreal m_rotationAngle;
    QPropertyAnimation *m_animation;

    void calculateSquareGeometry();

signals:
    void rotationAngleChanged(qreal angle);
};

#endif // LINE_ROTATING_ALONG_RECTANGULAR_BORDER_H