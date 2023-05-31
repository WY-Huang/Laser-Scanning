#ifndef MYCHARTVIEW_H
#define MYCHARTVIEW_H

#include <QChartView>
#include <QMouseEvent>
#include <QGraphicsSimpleTextItem>

#if (QT_VERSION <= QT_VERSION_CHECK(6,0,0))
QT_CHARTS_USE_NAMESPACE
#endif

class MyChartView : public QChartView
{
    Q_OBJECT

    using AxisRange = std::array<qreal, 2>;

public:
    explicit MyChartView(QWidget* parent = nullptr);
    MyChartView(QChart *chart, QWidget *parent = nullptr);
    ~MyChartView();
    // Save the coordinate area for resetting
    void saveAxisRange();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QPoint prevPoint_;          // 鼠标左键按下的位置坐标
    bool leftButtonPressed_;    // 鼠标左键是否按下
    bool ctrlPressed_;          // ctrl键是否按下
    bool alreadySaveRange_;     // 是否保存当前轴的范围
    AxisRange xRange_;          // X轴的范围
    AxisRange yRange_;          // Y轴的范围
    QGraphicsSimpleTextItem* coordItem_;
};

#endif // MYCHARTVIEW_H
