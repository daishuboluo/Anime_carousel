#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QColorDialog>

#include "ParticleWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadImageClicked();
    void onHeightSliderValueChanged(int value);
    void onChooseColorClicked();
    void onDotSizeSliderValueChanged(int value);

    void onRepulsionRadiusSliderValueChanged(int value);
    void onRepulsionStrengthSliderValueChanged(int value);

private:
    ParticleWidget *imageProcessorWidget;
    QPushButton *loadImageButton;
    QSlider *heightSlider;
    QLabel *heightLabel;
    QPushButton *processButton;

    QPushButton *chooseColorButton;
    QSlider *dotSizeSlider;
    QLabel *dotSizeLabel;

    QSlider *repulsionRadiusSlider;
    QLabel *repulsionRadiusLabel;
    QSlider *repulsionStrengthSlider;
    QLabel *repulsionStrengthLabel;


    QString lastOpenedDirPath;
};

#endif // MAINWINDOW_H