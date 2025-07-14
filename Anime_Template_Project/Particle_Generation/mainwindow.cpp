#include "mainwindow.h"
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    imageProcessorWidget = new ParticleWidget(this);
    imageProcessorWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(imageProcessorWidget);

    QWidget *controlPanelWidget = new QWidget(this);
    QVBoxLayout *controlPanelLayout = new QVBoxLayout(controlPanelWidget);
    controlPanelLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *controlLayoutTop = new QHBoxLayout();
    controlPanelLayout->addLayout(controlLayoutTop);

    loadImageButton = new QPushButton("加载图片", this);
    controlLayoutTop->addWidget(loadImageButton);
    connect(loadImageButton, &QPushButton::clicked, this, &MainWindow::onLoadImageClicked);

    processButton = new QPushButton("处理并压缩", this);
    controlLayoutTop->addWidget(processButton);
    processButton->setEnabled(false);
    connect(processButton, &QPushButton::clicked, [this]() {
        imageProcessorWidget->processAndResizeImage(heightSlider->value());
    });

    heightSlider = new QSlider(Qt::Horizontal, this);
    heightSlider->setRange(100, 1080);
    heightSlider->setValue(480);
    heightSlider->setSingleStep(10);
    heightSlider->setPageStep(50);
    heightSlider->setTickPosition(QSlider::TicksBelow);
    heightSlider->setTickInterval(100);
    controlLayoutTop->addWidget(heightSlider);

    heightLabel = new QLabel(QString("高度: %1px").arg(heightSlider->value()), this);
    controlLayoutTop->addWidget(heightLabel);
    connect(heightSlider, &QSlider::valueChanged, this, &MainWindow::onHeightSliderValueChanged);


    QHBoxLayout *controlLayoutMiddle = new QHBoxLayout();
    controlPanelLayout->addLayout(controlLayoutMiddle);

    chooseColorButton = new QPushButton("选择点颜色", this);
    controlLayoutMiddle->addWidget(chooseColorButton);
    connect(chooseColorButton, &QPushButton::clicked, this, &MainWindow::onChooseColorClicked);

    dotSizeSlider = new QSlider(Qt::Horizontal, this);
    dotSizeSlider->setRange(1, 10);
    dotSizeSlider->setValue(2);
    dotSizeSlider->setSingleStep(1);
    dotSizeSlider->setTickPosition(QSlider::TicksBelow);
    dotSizeSlider->setTickInterval(1);
    controlLayoutMiddle->addWidget(dotSizeSlider);

    dotSizeLabel = new QLabel(QString("点大小: %1px").arg(dotSizeSlider->value()), this);
    controlLayoutMiddle->addWidget(dotSizeLabel);
    connect(dotSizeSlider, &QSlider::valueChanged, this, &MainWindow::onDotSizeSliderValueChanged);

    QHBoxLayout *controlLayoutBottom = new QHBoxLayout();
    controlPanelLayout->addLayout(controlLayoutBottom);

    repulsionRadiusSlider = new QSlider(Qt::Horizontal, this);
    repulsionRadiusSlider->setRange(10, 200);
    repulsionRadiusSlider->setValue(80);
    repulsionRadiusSlider->setSingleStep(5);
    repulsionRadiusSlider->setTickPosition(QSlider::TicksBelow);
    repulsionRadiusSlider->setTickInterval(20);
    controlLayoutBottom->addWidget(repulsionRadiusSlider);

    repulsionRadiusLabel = new QLabel(QString("排斥半径: %1px").arg(repulsionRadiusSlider->value()), this);
    controlLayoutBottom->addWidget(repulsionRadiusLabel);
    connect(repulsionRadiusSlider, &QSlider::valueChanged, this, &MainWindow::onRepulsionRadiusSliderValueChanged);

    repulsionStrengthSlider = new QSlider(Qt::Horizontal, this);
    repulsionStrengthSlider->setRange(0, 100);
    repulsionStrengthSlider->setValue(20);
    repulsionStrengthSlider->setSingleStep(1);
    repulsionStrengthSlider->setTickPosition(QSlider::TicksBelow);
    repulsionStrengthSlider->setTickInterval(10);
    controlLayoutBottom->addWidget(repulsionStrengthSlider);

    repulsionStrengthLabel = new QLabel(QString("排斥强度: %1").arg(repulsionStrengthSlider->value()), this);
    controlLayoutBottom->addWidget(repulsionStrengthLabel);
    connect(repulsionStrengthSlider, &QSlider::valueChanged, this, &MainWindow::onRepulsionStrengthSliderValueChanged);


    mainLayout->addWidget(controlPanelWidget);
    resize(900, 700);

    imageProcessorWidget->setRepulsionRadius(repulsionRadiusSlider->value());
    imageProcessorWidget->setRepulsionStrength(repulsionStrengthSlider->value());
}

MainWindow::~MainWindow() {}

void MainWindow::onLoadImageClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "选择一张图片",
        lastOpenedDirPath,
        "图像文件 (*.png *.jpg *.jpeg *.bmp *.gif)"
        );

    if (!filePath.isEmpty()) {
        lastOpenedDirPath = QFileInfo(filePath).absolutePath();

        if (imageProcessorWidget->loadImage(filePath.toStdString())) {
            processButton->setEnabled(true);
            imageProcessorWidget->setDotColor(imageProcessorWidget->getDotColor());
            imageProcessorWidget->setDotSize(dotSizeSlider->value());

            imageProcessorWidget->setRepulsionRadius(repulsionRadiusSlider->value());
            imageProcessorWidget->setRepulsionStrength(repulsionStrengthSlider->value());

            imageProcessorWidget->processAndResizeImage(heightSlider->value());
        } else {
            processButton->setEnabled(false);
        }
    }
}


void MainWindow::onHeightSliderValueChanged(int value)
{
    heightLabel->setText(QString("高度: %1px").arg(value));
    if (imageProcessorWidget->hasOriginalImage()) {
        imageProcessorWidget->processAndResizeImage(value);
    }
}

void MainWindow::onChooseColorClicked()
{
    QColor initialColor = imageProcessorWidget->getDotColor();
    QColor color = QColorDialog::getColor(initialColor, this, "选择点颜色");
    if (color.isValid()) {
        imageProcessorWidget->setDotColor(color);
    }
}

void MainWindow::onDotSizeSliderValueChanged(int value)
{
    dotSizeLabel->setText(QString("点大小: %1px").arg(value));
    imageProcessorWidget->setDotSize(value);
}

void MainWindow::onRepulsionRadiusSliderValueChanged(int value)
{
    repulsionRadiusLabel->setText(QString("排斥半径: %1px").arg(value));
    imageProcessorWidget->setRepulsionRadius(value);
}

void MainWindow::onRepulsionStrengthSliderValueChanged(int value)
{
    repulsionStrengthLabel->setText(QString("排斥强度: %1").arg(value));
    imageProcessorWidget->setRepulsionStrength(static_cast<qreal>(value));
}