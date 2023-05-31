#include "mychartview.h"
#include <QValueAxis>

MyChartView::MyChartView(QWidget* parent)
    : QChartView{ parent }
    , prevPoint_{}
    , leftButtonPressed_{}
    , ctrlPressed_{}
    , alreadySaveRange_{}
    , xRange_{}
    , yRange_{}
    , coordItem_{}
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setMouseTracking(false);
    setCursor(QCursor(Qt::PointingHandCursor));
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
        coordItem_->setPos(80, 18);
        QColor color(Qt::red);
        coordItem_->setBrush(color);
        coordItem_->show();
    }
    const QPoint curPos{ event->pos() };
    const QPointF curVal{ chart()->mapToValue(QPointF(curPos)) };
    coordItem_->setText(QString("X = %1, Y = %2").arg(curVal.x()).arg(curVal.y()));

    if (leftButtonPressed_) {
        const auto offset = curPos - prevPoint_;
        prevPoint_ = curPos;
        if (!alreadySaveRange_) {
            saveAxisRange();
            alreadySaveRange_ = true;
        }
        chart()->scroll(-offset.x(), offset.y());
    }
}

void MyChartView::mouseReleaseEvent(QMouseEvent *event)
{
    leftButtonPressed_ = false;
//    if (event->button() == Qt::RightButton) {
//        if (alreadySaveRange_) {
//            chart()->axisX()->setRange(xRange_[0], xRange_[1]);
//            chart()->axisY()->setRange(yRange_[0], yRange_[1]);
//        }
//    }
}

void MyChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    leftButtonPressed_ = false;
    if (event->button() == Qt::LeftButton) {
        if (alreadySaveRange_) {
            chart()->axisX()->setRange(xRange_[0], xRange_[1]);
            chart()->axisY()->setRange(yRange_[0], yRange_[1]);
        }
    }
}

void MyChartView::wheelEvent(QWheelEvent *event)
{
#if (QT_VERSION <= QT_VERSION_CHECK(6,0,0))
    const auto pos  = QPointF(event->pos());
    const auto isZoomIn = event->delta() > 0;
#else
    const auto pos  = event->position();
    const auto isZoomIn = event->angleDelta().y() > 0;
#endif
    const QPointF curVal = chart()->mapToValue(pos);

    if (!alreadySaveRange_) {
        saveAxisRange();
        alreadySaveRange_ = true;
    }

    auto zoom = [](QValueAxis* axis, double centre, bool zoomIn) {
        constexpr auto scaling{ 1.5 };

        const double down = axis->min();
        const double up = axis->max();

        double downOffset{};
        double upOffset{};
        if (zoomIn) {
            downOffset = (centre - down) / scaling;
            upOffset = (up - centre) / scaling;
        }
        else {
            downOffset = (centre - down) * scaling;
            upOffset = (up - centre) * scaling;
        }

        axis->setRange(centre - downOffset, centre + upOffset);
    };

    if (ctrlPressed_) {
        auto axis = qobject_cast<QValueAxis*>(chart()->axisY());
        zoom(axis, curVal.y(), isZoomIn);
    } else {
        auto axis = qobject_cast<QValueAxis*>(chart()->axisX());
        zoom(axis, curVal.x(), isZoomIn);
    }
}

void MyChartView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        ctrlPressed_ = true;
}

void MyChartView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        ctrlPressed_ = false;
}

void MyChartView::saveAxisRange()
{
    const auto axisX = qobject_cast<QValueAxis*>(chart()->axisX());
    xRange_ = { axisX->min(), axisX->max() };
    const auto axisY = qobject_cast<QValueAxis*>(chart()->axisY());
    yRange_ = { axisY->min(), axisY->max() };
}
