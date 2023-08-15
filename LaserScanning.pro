QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    PictureBox.cpp \
    XTcp.cpp \
    cam_sensor.cpp \
    cambuilddlg.cpp \
    e2promdata.cpp \
    fileout.cpp \
    getcurtime.cpp \
    label_image_viewer.cpp \
    laser_paramsetingdlg.cpp \
    main.cpp \
    mainwindow.cpp \
    my_params.cpp \
    mychartview.cpp \
    mypclfunction.cpp \
    resultdata.cpp \
    showimgpcddlg.cpp \
    soptocameratcpip.cpp \
    soptopcamera.cpp

HEADERS += \
    PictureBox.h \
    XTcp.h \
    cam_sensor.h \
    cambuilddlg.h \
    e2promdata.h \
    fileout.h \
    getcurtime.h \
    global.h \
    label_image_viewer.h \
    laser_paramsetingdlg.h \
    mainwindow.h \
    my_params.h \
    mychartview.h \
    mypclfunction.h \
    resultdata.h \
    showimgpcddlg.h \
    soptocameratcpip.h \
    soptopcamera.h \
    tistdtypes.h

FORMS += \
    cambuilddlg.ui \
    laser_paramsetingdlg.ui \
    mainwindow.ui \
    showimgpcddlg.ui

#opencv库的添加
INCLUDEPATH += /usr/local/include/opencv4
LIBS += /usr/local/lib/libopencv_*

#ROS库添加
INCLUDEPATH +=/opt/ros/galactic/include
LIBS += /opt/ros/galactic/lib/lib*.so
LIBS += /opt/ros/galactic/lib/x86_64-linux-gnu/lib*.so

#modbustcp库的添加
INCLUDEPATH += /home/wanyel/Downloads/modbus/libmodbus/install/include
LIBS += /home/wanyel/Downloads/modbus/libmodbus/install/lib/libmodbus.so*

#ros2接口
INCLUDEPATH += /home/wanyel/TPSoftware/tutorial_interfaces/install/tutorial_interfaces/include
LIBS += /home/wanyel/TPSoftware/tutorial_interfaces/install/tutorial_interfaces/lib/libtutorial_interfaces*.so

#Eigen库
INCLUDEPATH += /usr/include/eigen3

#VTK库添加
INCLUDEPATH +=/usr/local/include/vtk-8.2
LIBS += /usr/local/lib/libvtk*

#Boost
INCLUDEPATH += /usr/include/boost
LIBS += /usr/lib/x86_64-linux-gnu/libboost_*

#PCL库添加
INCLUDEPATH +=/usr/local/include/pcl-1.12
LIBS += /usr/local/lib/libpcl_*

#libssh库添加
INCLUDEPATH += /usr/include/libssh
LIBS += /usr/lib/x86_64-linux-gnu/libssh*

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ico.qrc
