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

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QPoint prevPoint_;          // 第一次鼠标左键按下的位置坐标
    QPoint offsetAll;           // 鼠标按下状态总的移动量
    int operation_count;        // 操作计数
    int firstOperate;           // 鼠标左键是否按下
    bool leftButtonPressed_;    // 鼠标左键是否按下
    QGraphicsSimpleTextItem* coordItem_;
};

#endif // MYCHARTVIEW_H
