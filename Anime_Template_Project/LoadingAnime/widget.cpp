#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(1200,860);

    m_ringLoading = new Ring_Loading(this);
    m_ringLoading->resize(this->size() / 2 );
    m_ringLoading->initCenterX();
    m_ringLoading->startAnimation();

    m_bouncingBallLoading = new Bouncing_Ball_Loading(this);

    m_rectangleLoading = new Rectangle_Loading(this);

    m_lineRotating = new Line_Rotating_Along_Rectangular_Border(this);

    m_infinitySymbolLoading = new Square_Pseudo_Bounce_Loading(this);

    m_regularLoading = new Regular_Loading(this);

    m_lineLoading = new Line_Loading(this);

    loadingWidget = new Black_White_Ball_Loading(this);
    loadingWidget->setBallPairCount(36);
    loadingWidget->setAnimationRatios(0.3, 0.4);
    loadingWidget->setBallRadiusRatio(0.025);
    loadingWidget->setAnimationSpeed(0.025);
    loadingWidget->setGroupCount(4);

    loader = new HexagonLoaderWidget(this);
}

Widget::~Widget() {}

void Widget::resizeEvent(QResizeEvent *event)
{
    m_ringLoading->resize(event->size() / 3);
    m_ringLoading->move(0, 0);

    m_bouncingBallLoading->move(event->size().width() / 3, 0);
    m_bouncingBallLoading->setSize(event->size() / 3 );

    m_rectangleLoading->move(event->size().width() / 3 * 2, 0);
    m_rectangleLoading->resize(event->size() / 3 );

    m_lineRotating->move(0, event->size().height() / 3);
    m_lineRotating->resize(event->size() / 3 );

    m_infinitySymbolLoading->move(event->size().width() / 3, event->size().height() / 3);
    m_infinitySymbolLoading->resize(event->size() / 3 );

    m_regularLoading->move(event->size().width() / 3 * 2, event->size().height() / 3);
    m_regularLoading->resize(event->size() / 3 );

    m_lineLoading->move(0, event->size().height() / 3 * 2);
    m_lineLoading->resize(event->size() / 3 );

    loadingWidget->move(event->size().width() / 3, event->size().height() / 3 * 2);
    loadingWidget->resize(event->size() / 3 );

    loader->move(event->size().width() / 3 * 2, event->size().height() / 3 * 2);
    loader->resize(event->size() / 3 );

    QWidget::resizeEvent(event);
}