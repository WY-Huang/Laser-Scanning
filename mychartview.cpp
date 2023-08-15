#include "mychartview.h"
#include <QValueAxis>
#include <QScatterSeries>

MyChartView::MyChartView(QWidget* parent)
    : QChartView{ parent }
    , prevPoint_{}
    , leftButtonPressed_{}
    , coordItem_{}
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setMouseTracking(false);
//    setCursor(QCursor(Qt::PointingHandCursor));
    operation_count = 0;
    firstOperate = 0;
}

MyChartView::MyChartView(QChart *chart, QWidget *parent)
    : MyChartView{ parent } // C++11 Delegating constructors
{
    setChart(chart);
}

MyChartView::~MyChartView()
{
}

void MyChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        prevPoint_ = event->pos();
        leftButtonPressed_ = true;
    }
}

void MyChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (!coordItem_) {
        coordItem_ = new QGraphicsSimpleTextItem{ chart() };
        coordItem_->setZValue(5);
        coordItem_->setPos(100, 18);
        QColor color(Qt::red);
        coordItem_->setBrush(color);
        coordItem_->show();
    }
    const QPoint curPos{ event->pos() };
    const QPointF curVal{ chart()->mapToValue(QPointF(curPos)) };
    coordItem_->setText(QString("X = %1, Y = %2").arg(curVal.x()).arg(curVal.y()));

    if (leftButtonPressed_) {
        const auto offset = curPos - prevPoint_;
        offsetAll += offset;
        prevPoint_ = curPos;
        chart()->scroll(-offset.x(), offset.y());

        // 用于判断平移是否比放大先操作
        operation_count += 1;
        if (operation_count == 1)
        {
            firstOperate = 1;
        }

    }
}

void MyChartView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    leftButtonPressed_ = false;
}

void MyChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {

        if (firstOperate == 1)
        {
            chart()->zoomReset();
            chart()->scroll(offsetAll.x(), -offsetAll.y());
        }
        else
        {
            chart()->scroll(offsetAll.x(), -offsetAll.y());
            chart()->zoomReset();
        }

        offsetAll = QPoint(0, 0);
        operation_count = 0;
        firstOperate = 0;
    }
}

void MyChartView::wheelEvent(QWheelEvent *event)
{
#if (QT_VERSION <= QT_VERSION_CHECK(6,0,0))
    const auto isZoomIn = event->delta() > 0;
#else
    const auto isZoomIn = event->angleDelta().y() > 0;
#endif

    constexpr auto scaling{ 1.5 };
    if (isZoomIn)
    {
        chart()->zoom(scaling);
    }
    else
    {
        chart()->zoom(1 / scaling);
    }

    operation_count += 1;

}
