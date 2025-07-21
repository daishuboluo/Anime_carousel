// regular_loading.h
#ifndef REGULAR_LOADING_H
#define REGULAR_LOADING_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QPropertyAnimation>
#include <QEasingCurve>

class Regular_Loading : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int rotationAngle READ rotationAngle WRITE setRotationAngle NOTIFY rotationAngleChanged)

public:
    explicit Regular_Loading(QWidget *parent = nullptr);

    int rotationAngle() const { return m_rotationAngle; }

public slots:
    void setRotationAngle(int angle);

signals:
    void rotationAngleChanged(int angle);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_rotationAngle;
    QPropertyAnimation *m_animation;
};

#endif // REGULAR_LOADING_H