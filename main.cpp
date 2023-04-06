#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication a(argc, argv);
    MainWindow w;
    w.m_mcs->cam->sop_cam[0].argc=argc;
    w.m_mcs->cam->sop_cam[0].argv=argv;
    w.setWindowTitle("线激光轮廓测量");
    w.show();
    return a.exec();
}
