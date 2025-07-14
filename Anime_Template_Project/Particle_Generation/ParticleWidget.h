#ifndef PARTICLEWIDGET_H
#define PARTICLEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QImage>
#include <QPainter>
#include <QRandomGenerator>
#include <vector>
#include <cmath>
#include <QMouseEvent>
#include <QElapsedTimer>

struct MovingDot {
    QPoint currentPos;
    cv::Point originalImgPos;
    QPoint startPos;
    qreal progress;
    bool reached;

    bool isRepulsed;
    QPoint repulsionStartPos;
    QElapsedTimer repulsionElapsedTimer;
};


class ParticleWidget : public QWidget {
    Q_OBJECT

public:
    explicit ParticleWidget(QWidget *parent = nullptr);
    ~ParticleWidget();

    bool loadImage(const std::string &imagePath);

    void processAndResizeImage(int targetHeight);

    void setDotColor(const QColor& color);
    void setDotSize(int size);

    void setRepulsionRadius(int radius);
    void setRepulsionStrength(qreal strength);
    void setRepositioningDuration(int duration);

    bool hasOriginalImage() const { return !originalImage.empty(); }
    QColor getDotColor() const { return m_dotColor; }

    bool isImageLoaded() const
    {
        return !originalImage.empty();
    }

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void updateAnimation();

private:
    QImage cvMatToQImage(const cv::Mat &mat);
    void startDotAnimation();

    QPoint mapProcessedToWidget(const cv::Point& p) const;

private:
    std::vector<cv::Point> whitePixels;
    cv::Mat processedImage;

    cv::Mat originalImage;
    QImage displayImage;

    QColor m_dotColor;
    int m_dotSize;

    QTimer *m_animationTimer;
    std::vector<MovingDot> m_movingDots;
    int m_animationDuration;
    QElapsedTimer m_elapsedTimer;
    bool m_animationRunning;

    QPoint m_mousePos;
    int m_repulsionRadius;
    qreal m_repulsionStrength;

    int m_repositioningDuration;
};

#endif // PARTICLEWIDGET_H