#include "label_image_viewer.h"


LabelImageViewer::LabelImageViewer(QWidget* parent) : QLabel(parent)
{
//    setScaledContents(false);
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    setMinimumWidth(800);
    setMinimumHeight(600);
    _winW = width();
    _winH = height();   // window size
    _col = 0;
    _row = 0;
    _scaleFactor = 1.0;
    _mouseAsScaleCt = true;
}

// 窗口缩放事件
void LabelImageViewer::resizeEvent(QResizeEvent * event)
{
    int w = event->size().width();
    int h = event->size().height();
    if (!_srcImg.isNull())
        scaleImage(_scaleFactor);
    _winW = w;
    _winH = h;  // window size
    event->accept();
}

// 窗口显示图像
bool LabelImageViewer::showImage(const QImage& img)
{
    _srcImg = img.copy();
//    setScaledContents(false);
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);//must replace the sizepolicy setting in Qt-designer, otherwise the label will adjust to fit the image's size.
    setStyleSheet("QLabel{background-color:RGB(0,0,0);}");
    int col = _srcImg.rect().width();//source image size
    int row = _srcImg.rect().height();
    if(_col!=col || _row!=row )//size not same as the old img
    {
        _col = col;
        _row = row;
        fitToWindow();
    }
    else//keep scale and offset
    {
        _scaledImg = _srcImg.scaled(_col*_scaleFactor, _row*_scaleFactor);
        QImage croppedImg = _scaledImg.copy(_ctPix.x()-0.5*_winW, _ctPix.y()-0.5*_winH, _winW, _winH);
        setPixmap(QPixmap::fromImage(croppedImg));
    }
    return true;
}

// 图像适应窗口大小
void LabelImageViewer::fitToWindow()
{
    float ra = float(_winW) / _col, rb = float(_winH) / _row;
    _scaleFactor = std::min(ra, rb);//full display long edge, don't stretch short edge, can be shown diff by changing background color.
    _scaledImg = _srcImg.scaled(_col*_scaleFactor, _row*_scaleFactor);
    setPixmap(QPixmap::fromImage(_scaledImg));
   if (ra < rb)
        _ctPix = QPoint(_winW - 0.5*_col*_scaleFactor, 0.5 * _row*_scaleFactor);
    else
        _ctPix = QPoint(0.5*_col*_scaleFactor, _winH - 0.5 * _row*_scaleFactor);
}

// 打开图像文件
void LabelImageViewer::open()
{
    QStringList mimeTypeFilters;
    foreach(const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();

    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
//    dialog.selectMimeTypeFilter("image/jpeg");

    if(dialog.exec() == QDialog::Accepted )
    {
        QString fileName = dialog.selectedFiles().first();
        _srcImg.load(fileName);
        if (_srcImg.isNull()) {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                     tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
            setWindowFilePath(QString());
            setPixmap(QPixmap());
            adjustSize();
        }
        else
        {
            setWindowFilePath(fileName);
            showImage(_srcImg);
        }
    }
}

// 键盘按键事件
void LabelImageViewer::keyPressEvent(QKeyEvent *event)
{
    int mask = event->modifiers();
    if (event->key() == Qt::Key_C)
        fitToWindow();
    else if (event->key() == Qt::Key_O && mask &Qt::ControlModifier)
        open();
    else if (event->key() == Qt::Key_K)
		_mouseAsScaleCt = !_mouseAsScaleCt;
    event->accept();
}

// 滚轮缩放事件
void LabelImageViewer::wheelEvent(QWheelEvent *event)
{
//    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    float newScale = _scaleFactor;
    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        newScale += numSteps.y()*0.1;
    }
    _wheelPos = event->position().toPoint();
    scaleImage(newScale);
    event->accept();
}

// 获取鼠标位置
void LabelImageViewer::mousePressEvent(QMouseEvent * event)
{
    if (_srcImg.isNull())
    {
        event->ignore();
        return;
    }
    _dragStartPos = event->pos();
//    std::cout << _dragStartPos.x() << ", " << _dragStartPos.y() << std::endl;
    event->accept();
}

// 鼠标移动检测
void LabelImageViewer::mouseMoveEvent(QMouseEvent * event)
{
    if (_srcImg.isNull())
    {
        event->ignore();
        return;
    }
    // 获取鼠标按下状态
    Qt::MouseButtons buttons = event->buttons();
    if (buttons & Qt::LeftButton)
    {
        QPoint dragE = event->pos();
        _delta = _dragStartPos - dragE;
        offsetImage();
        _dragStartPos = dragE;
    }
    event->accept();
}
/*
void LabelImageViewer::mouseReleaseEvent(QMouseEvent * event)
{
    if (_srcImg.isNull())
    {
        event->ignore();
        return;
    }
    QPoint dragE = event->pos();
    _delta = _dragStartPos - dragE;
    offsetImage();
    event->accept();
}
*/
// 根据窗口及滚轮变动来缩放图片
void LabelImageViewer::scaleImage(float newScale)
{
    if (_srcImg.isNull())
		return;
    if (newScale > 5 || newScale < 0.33)
        return;
    if (!_mouseAsScaleCt)
	{
        // keep the win-center as scale center
		_ctPix = _ctPix*newScale / _scaleFactor;
		_scaledImg = _srcImg.scaled(_col*newScale, _row*newScale);
		_scaleFactor = newScale;
		QImage croppedImg = _scaledImg.copy(_ctPix.x() - 0.5*_winW, _ctPix.y() - 0.5*_winH, _winW, _winH);
		setPixmap(QPixmap::fromImage(croppedImg));
	}
	else
    {   // keep the mouse position as scale center
        QPoint dt = _wheelPos-QPoint(0.5*_winW,0.5*_winH);//win coords delta
		QPoint pixMsOld = _ctPix + dt;//mouse pix in _scaleFactor
		QPoint pixMsNew = pixMsOld*newScale / _scaleFactor;//mouse pix in newScale
		_ctPix = pixMsNew - dt;
		_scaledImg = _srcImg.scaled(_col*newScale, _row*newScale);
		_scaleFactor = newScale;
		QImage croppedImg = _scaledImg.copy(_ctPix.x() - 0.5*_winW, _ctPix.y() - 0.5*_winH, _winW, _winH);
		setPixmap(QPixmap::fromImage(croppedImg));
	}
}

// 鼠标移动图像
void LabelImageViewer::offsetImage()
{
//    int w = _scaledImg.width();
//    int h = _scaledImg.height();
//    QPoint cp = _ctPix + _delta;
    _ctPix = _ctPix + _delta;
    QImage croppedImg = _scaledImg.copy(_ctPix.x()-0.5*_winW, _ctPix.y()-0.5*_winH, _winW, _winH);
    setPixmap(QPixmap::fromImage(croppedImg));
}


//#include <QApplication>
//int main(int argc,   char* argv[])
//{
//    QApplication app(argc,argv);
//    LabelImageViewer viewer;
//    viewer.show();
//    return app.exec();
//}
