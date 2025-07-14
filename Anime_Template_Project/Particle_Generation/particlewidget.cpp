#include "ParticleWidget.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>
#include <QtMath>
#include <QEvent>
#include <set>
#include <QCursor>

#include <opencv2/opencv.hpp>


ParticleWidget::ParticleWidget(QWidget *parent)
    : QWidget(parent),
    m_dotColor(Qt::red),
    m_dotSize(2),
    m_animationTimer(new QTimer(this)),
    m_animationDuration(1000),
    m_animationRunning(false),
    m_repulsionRadius(80),
    m_repulsionStrength(20.0),
    m_repositioningDuration(333)
{
    setMinimumSize(640, 480);

    connect(m_animationTimer, &QTimer::timeout, this, &ParticleWidget::updateAnimation);

    setMouseTracking(true);
}

ParticleWidget::~ParticleWidget()
{
}

void ParticleWidget::setDotColor(const QColor& color)
{
    m_dotColor = color;
    update();
}

void ParticleWidget::setDotSize(int size)
{
    if (size > 0) {
        m_dotSize = size;
        update();
    }
}

void ParticleWidget::setRepulsionRadius(int radius)
{
    if (radius >= 0)
    {
        m_repulsionRadius = radius;

            update();
    }
}

void ParticleWidget::setRepulsionStrength(qreal strength)
{
    m_repulsionStrength = strength;
    if (m_animationTimer->isActive())
    {
        update();
    }
}

void ParticleWidget::setRepositioningDuration(int duration)
{
    if (duration >= 0) {
        m_repositioningDuration = duration;
    }
}

bool ParticleWidget::loadImage(const std::string& imagePath)
{
    cv::Mat img = cv::imread(imagePath);
    if (img.empty()) {
        qWarning("无法加载图片或图片不存在: %s", imagePath.c_str());
        originalImage = cv::Mat();
        processedImage = cv::Mat();
        displayImage = QImage();
        whitePixels.clear();
        m_movingDots.clear();
        m_animationTimer->stop();
        m_animationRunning = false;
        update();
        return false;
    }
    originalImage = img.clone();
    qDebug() << "图片加载成功:" << imagePath.c_str();
    return true;
}

void ParticleWidget::processAndResizeImage(int targetHeight)
{
    if (originalImage.empty()) {
        qWarning("未加载原始图片，无法进行处理和压缩。");
        processedImage = cv::Mat();
        displayImage = QImage();
        whitePixels.clear();
        update();
        return;
    }

    double aspectRatio = (double)originalImage.cols / originalImage.rows;
    int targetWidth = static_cast<int>(targetHeight * aspectRatio);

    cv::Mat resizedImg;
    cv::resize(originalImage, resizedImg, cv::Size(targetWidth, targetHeight));

    cv::Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

    cvtColor(resizedImg, imgGray, cv::COLOR_BGR2GRAY);
    GaussianBlur(resizedImg, imgBlur, cv::Size(3, 3), 3, 0);
    Canny(imgBlur, imgCanny, 25, 75);

    cv::Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    dilate(imgCanny, imgDil, kernel);
    erode(imgDil, imgErode, kernel);

    cv::threshold(imgErode, imgErode, 127, 255, cv::THRESH_BINARY);

    processedImage = imgErode.clone();

    whitePixels.clear();

    int step = 1;

    if (processedImage.rows > 0) {
        int baseStep = 2;
        step = baseStep;
    }

    for (int y = 0; y < processedImage.rows; y += step) {
        for (int x = 0; x < processedImage.cols; x += step) {
            if (processedImage.at<uchar>(y, x) == 255) {
                whitePixels.push_back(cv::Point(x, y));
            }
        }
    }

    qDebug() << "处理并压缩后的图片尺寸: " << processedImage.cols << "x" << processedImage.rows;
    qDebug() << "找到白色像素数量:" << whitePixels.size();

    displayImage = cvMatToQImage(processedImage);
    startDotAnimation();
    update();
}

QPoint ParticleWidget::mapProcessedToWidget(const cv::Point& p) const
{
    if (processedImage.empty() || width() == 0 || height() == 0) {
        return QPoint(0, 0);
    }

    double widgetAspectRatio = static_cast<double>(width()) / height();
    double imageAspectRatio = static_cast<double>(processedImage.cols) / processedImage.rows;

    int drawWidth, drawHeight;
    if (widgetAspectRatio > imageAspectRatio) {
        drawHeight = height();
        drawWidth = static_cast<int>(drawHeight * imageAspectRatio);
    } else {
        drawWidth = width();
        drawHeight = static_cast<int>(drawWidth / imageAspectRatio);
    }

    int startX = (width() - drawWidth) / 2;
    int startY = (height() - drawHeight) / 2;

    double scaleX = static_cast<double>(drawWidth) / processedImage.cols;
    double scaleY = static_cast<double>(drawHeight) / processedImage.rows;

    return QPoint(startX + static_cast<int>(p.x * scaleX),
                    startY + static_cast<int>(p.y * scaleY));
}


void ParticleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);


    if (!displayImage.isNull()) {

        if (m_animationRunning || !m_movingDots.empty())
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(m_dotColor));

            for (const MovingDot& dot : m_movingDots) {
                painter.drawEllipse(dot.currentPos.x() - m_dotSize / 2,
                                    dot.currentPos.y() - m_dotSize / 2,
                                    m_dotSize, m_dotSize);
            }
        }

    }


}

void ParticleWidget::startDotAnimation()
{
    if (whitePixels.empty() || processedImage.empty()) {
        m_movingDots.clear();
        m_animationTimer->stop();
        m_animationRunning = false;
        qDebug() << "无目标像素，粒子清空。";
        return;
    }

    std::set<std::pair<int, int>> targetPixelSet;
    for (const cv::Point& p : whitePixels) {
        targetPixelSet.insert({p.x, p.y});
    }

    std::vector<bool> dotShouldRetain(m_movingDots.size(), false);
    std::set<std::pair<int, int>> matchedTargetPixels;

    for (size_t i = 0; i < m_movingDots.size(); ++i) {
        const auto& dot = m_movingDots[i];
        std::pair<int, int> currentDotOriginalPos = {dot.originalImgPos.x, dot.originalImgPos.y};

        if (targetPixelSet.count(currentDotOriginalPos) && !matchedTargetPixels.count(currentDotOriginalPos)) {
            dotShouldRetain[i] = true;
            matchedTargetPixels.insert(currentDotOriginalPos);
        }
    }

    std::vector<MovingDot> newMovingDots;
    for (size_t i = 0; i < m_movingDots.size(); ++i) {
        if (dotShouldRetain[i]) {
            newMovingDots.push_back(m_movingDots[i]);
            newMovingDots.back().isRepulsed = false;
            newMovingDots.back().repulsionElapsedTimer.invalidate();
            newMovingDots.back().startPos = newMovingDots.back().currentPos;
            newMovingDots.back().progress = 0.0;
            newMovingDots.back().reached = false;
        }
    }

    for (const cv::Point& targetCvPoint : whitePixels) {
        std::pair<int, int> currentTarget = {targetCvPoint.x, targetCvPoint.y};
        if (!matchedTargetPixels.count(currentTarget)) {
            MovingDot dot;
            dot.originalImgPos = targetCvPoint;

            dot.startPos.setX(QRandomGenerator::global()->bounded(width()));
            dot.startPos.setY(QRandomGenerator::global()->bounded(height()));
            dot.currentPos = dot.startPos;
            dot.progress = 0.0;
            dot.reached = false;
            dot.isRepulsed = false;
            dot.repulsionElapsedTimer.invalidate();
            newMovingDots.push_back(dot);
        }
    }

    m_movingDots = newMovingDots;

    if (!m_movingDots.empty())
    {
        m_animationRunning = true;
        m_elapsedTimer.restart();
        if (!m_animationTimer->isActive()) {
            m_animationTimer->start(16);
        }
        qDebug() << "动画调整：当前粒子数量:" << m_movingDots.size();
    } else {
        m_animationTimer->stop();
        m_animationRunning = false;
        qDebug() << "动画停止：无粒子需要移动。";
    }

    update();
}


void ParticleWidget::updateAnimation()
{
    qint64 mainAnimationElapsed = m_elapsedTimer.elapsed();

    bool allDotsReachedTarget = true;

    for (MovingDot& dot : m_movingDots) {
        QPoint targetPos = mapProcessedToWidget(dot.originalImgPos);
        QPoint idealPos;

        if (!dot.reached)
        {
            dot.progress = qMin(1.0, static_cast<qreal>(mainAnimationElapsed) / m_animationDuration);
            idealPos.setX(static_cast<int>(dot.startPos.x() + (targetPos.x() - dot.startPos.x()) * dot.progress));
            idealPos.setY(static_cast<int>(dot.startPos.y() + (targetPos.y() - dot.startPos.y()) * dot.progress));

            if (dot.progress >= 1.0)
            {
                idealPos = targetPos;
                dot.reached = true;
            }
            else
            {
                allDotsReachedTarget = false;
            }
        }
        else
        {
            idealPos = targetPos;
        }


        QPoint currentEffectivePos = dot.currentPos;

        bool inRepulsionRange = false;
        if (rect().contains(m_mousePos))
        {
            qreal dx_repel = currentEffectivePos.x() - m_mousePos.x();
            qreal dy_repel = currentEffectivePos.y() - m_mousePos.y();
            qreal distance_repel = std::sqrt(dx_repel*dx_repel + dy_repel*dy_repel);

            if (distance_repel < m_repulsionRadius && distance_repel > 0.1) {
                inRepulsionRange = true;
                dot.isRepulsed = true;
                dot.repulsionElapsedTimer.invalidate();

                qreal repulsionFactor = (m_repulsionRadius - distance_repel) / m_repulsionRadius;
                qreal strength = m_repulsionStrength * repulsionFactor;

                qreal normalizedDx = dx_repel / distance_repel;
                qreal normalizedDy = dy_repel / distance_repel;

                QPoint displacedPos;
                displacedPos.setX(static_cast<int>(idealPos.x() + normalizedDx * strength));
                displacedPos.setY(static_cast<int>(idealPos.y() + normalizedDy * strength));

                dot.currentPos.setX(qBound(0, displacedPos.x(), width()));
                dot.currentPos.setY(qBound(0, displacedPos.y(), height()));

            }
        }

        if (!inRepulsionRange) {
            if (dot.isRepulsed) {
                dot.repulsionStartPos = dot.currentPos;
                dot.repulsionElapsedTimer.restart();
                dot.isRepulsed = false;
            }

            if (dot.repulsionElapsedTimer.isValid()) {
                qint64 repelElapsed = dot.repulsionElapsedTimer.elapsed();
                qreal repelProgress = qMin(1.0, static_cast<qreal>(repelElapsed) / m_repositioningDuration);

                dot.currentPos.setX(static_cast<int>(dot.repulsionStartPos.x() + (idealPos.x() - dot.repulsionStartPos.x()) * repelProgress));
                dot.currentPos.setY(static_cast<int>(dot.repulsionStartPos.y() + (idealPos.y() - dot.repulsionStartPos.y()) * repelProgress));

                if (repelProgress >= 1.0) {
                    dot.repulsionElapsedTimer.invalidate();
                    dot.currentPos = idealPos;
                }
                allDotsReachedTarget = false;
            } else {
                dot.currentPos = idealPos;
            }
        }
    }




    update();
}


void ParticleWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();

    QWidget::mouseMoveEvent(event);
}


QImage ParticleWidget::cvMatToQImage(const cv::Mat &mat)
{
    switch (mat.type()) {
    case CV_8UC4:
    {
        return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_ARGB32);
    }
    case CV_8UC3:
    {
        QImage image(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8UC1:
    {
        return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8);
    }
    default:
        qWarning("无法将CV::Mat转换为QImage: 未知或不支持的格式");
        return QImage();
    }
}

void ParticleWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    startDotAnimation();
    update();
}