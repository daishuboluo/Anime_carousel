#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QResizeEvent>

#include "ring_loading.h"
#include "bouncing_ball_loading.h"
#include "Rectangle_Loading.h"
#include "line_rotating_along_rectangular_border.h"
#include "Square_Pseudo_Bounce_Loading.h"
#include "regular_loading.h"
#include "line_loading.h"
#include "black_white_ball_loading.h"
#include "HexagonLoaderWidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ring_Loading *m_ringLoading;
    Bouncing_Ball_Loading *m_bouncingBallLoading;
    Rectangle_Loading *m_rectangleLoading;
    Line_Rotating_Along_Rectangular_Border *m_lineRotating;
    Square_Pseudo_Bounce_Loading *m_infinitySymbolLoading;
    Regular_Loading *m_regularLoading;
    Line_Loading *m_lineLoading;
    Black_White_Ball_Loading *loadingWidget;
    HexagonLoaderWidget *loader;
};
#endif // WIDGET_H