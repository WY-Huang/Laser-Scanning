#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());  // 确保在Qt和VTK之间进行渲染时使用一致的OpenGL渲染属性

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("线激光轮廓测量");
    #if _MSC_VER||WINDOWS_TCP
    #else
        w.m_mcs->cam->sop_cam[0].argc=argc;
        w.m_mcs->cam->sop_cam[0].argv=argv;
    #endif
    w.show();
    return a.exec();
}
