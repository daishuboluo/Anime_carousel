#ifndef RECTANGLE_LOADING_H
#define RECTANGLE_LOADING_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QPainter>
#include <QTimer>

class Rectangle_Loading : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal rotationAngle READ rotationAngle WRITE setRotationAngle)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)

public:
    explicit Rectangle_Loading(QWidget *parent = nullptr);
    ~Rectangle_Loading();

    qreal rotationAngle() const { return m_rotationAngle; }
    void setRotationAngle(qreal angle);

    int borderWidth() const { return m_borderWidth; }
    void setBorderWidth(int width);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLoadingDots();

private:
    qreal m_rotationAngle;
    int m_borderWidth;
    QPropertyAnimation *m_rotationAnimation;
    QPropertyAnimation *m_borderAnimation;

    QTimer *m_dotTimer;
    int m_currentDotCount;

    QColor m_drawColor;

    void setupAnimations();
};

#endif // RECTANGLE_LOADING_H