#ifndef RING_LOADING_H
#define RING_LOADING_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QPropertyAnimation>
#include <QTimer>

class Ring_Loading : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int margin READ getMargin WRITE setMargin NOTIFY marginChanged)

    Q_PROPERTY(QColor ringColor READ getRingColor WRITE setRingColor NOTIFY ringColorChanged)

    Q_PROPERTY(int startAngle READ getStartAngle WRITE setStartAngle NOTIFY startAngleChanged)

    Q_PROPERTY(int endAngle READ getEndAngle WRITE setEndAngle NOTIFY endAngleChanged)

    Q_PROPERTY(int ringWidth READ getRingWidth WRITE setRingWidth NOTIFY ringWidthChanged)

    Q_PROPERTY(int Progress_Ring READ name WRITE setname NOTIFY nameChanged FINAL)


    Q_PROPERTY(int Inner_Arc_Start_Angle READ Inner_Arc_Start_Angle WRITE setInner_Arc_Start_Angle NOTIFY Inner_Arc_Start_AngleChanged FINAL)
    Q_PROPERTY(int Inner_Arc_End_Angle READ Inner_Arc_End_Angle WRITE setInner_Arc_End_Angle NOTIFY Inner_Arc_End_AngleChanged FINAL)
    Q_PROPERTY(int Inner_Arc_Alpha READ Inner_Arc_Alpha WRITE setInner_Arc_Alpha NOTIFY Inner_Arc_AlphaChanged FINAL)
    Q_PROPERTY(int Arc_Position_Value READ Arc_Position_Value WRITE setArc_Position_Value FINAL)


public:
    explicit Ring_Loading(QWidget *parent = nullptr);

    int getMargin() const { return m_margin; }
    QColor getRingColor() const { return m_ringColor; }
    int getStartAngle() const { return m_startAngle / 16; }
    int getEndAngle() const { return (m_startAngle + m_spanAngle) / 16; }

    void setMargin(int margin);
    void setRingColor(const QColor &color);
    void setStartAngle(int angle);
    void setEndAngle(int angle);

    int getRingWidth() const { return m_ringWidth; }
    void setRingWidth(int width);


    int name() const;
    void setname(int newProgress_Ring);

    void initCenterX() { m_centerX = qMin(width(), height()) / 2; }


    int Inner_Arc_Start_Angle() const;
    void setInner_Arc_Start_Angle(int newInner_Arc_Start_Angle);

    int Inner_Arc_End_Angle() const;
    void setInner_Arc_End_Angle(int newInner_Arc_End_Angle);

    int Inner_Arc_Alpha() const;
    void setInner_Arc_Alpha(int newInner_Arc_Alpha);

    int Arc_Position_Value() const;
    void setArc_Position_Value(int newArc_Position_Value);

public slots:

    void startAnimation();
    void startAnimation2();


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QColor m_ringColor;
    int m_startAngle;
    int m_spanAngle;
    int m_ringWidth;
    int m_margin;

    int m_Progress_Ring{0};
    int m_Arc_Position_Value{0};

    int m_centerX{0};

    int m_Inner_Arc_Start_Angle{0};
    int m_Inner_Arc_End_Angle{0};
    int m_Inner_Arc_Alpha{0};

    QTimer *m_timer;
    qreal Speed{0};
signals:
    void marginChanged(int margin);
    void ringColorChanged(const QColor &color);
    void startAngleChanged(int angle);
    void endAngleChanged(int angle);
    void ringWidthChanged(int width);
    void nameChanged();


    void Inner_Arc_Start_AngleChanged();
    void Inner_Arc_End_AngleChanged();
    void Inner_Arc_AlphaChanged();
};

#endif // RING_LOADING_H
