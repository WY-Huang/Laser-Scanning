#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QLabel>
#include <QWidget>
#include <QtWidgets>
#include <QImage>
#include <iostream>

class LabelImageViewer : public QLabel
{
    Q_OBJECT

public:
    LabelImageViewer(QWidget* parent=0);
public slots:
    void open();        // Ctrl+O
    void fitToWindow(); // C
	bool showImage(const QImage& img);
    void switchMode(){ _mouseAsScaleCt = !_mouseAsScaleCt; }    // K
protected:
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
//    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void resizeEvent(QResizeEvent * event);
    virtual void wheelEvent(QWheelEvent * event);
    virtual void keyPressEvent(QKeyEvent * event);
private:
    void scaleImage(float newScale);
    void offsetImage();

    int _winW, _winH;           // window size
    QImage _srcImg, _scaledImg; // entire image
    int _col, _row;             // source image size
    double _scaleFactor;        // scale ratio
	QPoint _dragStartPos;
    QPoint _delta;              // pan distance in pixel, maybe acdepted or ignored.
    QPoint _ctPix;              // windows center's pixel coords in scaled image.
	//fix center or set center to mouse position after scale. 
    bool _mouseAsScaleCt;       // default-true(mouse position as scale center,if false-windows center as scale center), press K to switch
    QPoint _wheelPos;
};

#endif
