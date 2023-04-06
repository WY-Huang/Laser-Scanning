#ifndef SOPTOPCAMERA_H
#define SOPTOPCAMERA_H

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <sensor_msgs/msg/image.hpp>
#include "std_msgs/msg/header.hpp"
#include <cv_bridge/cv_bridge.h>

#include "tutorial_interfaces/msg/if_algorhmitcloud.hpp"
#include "tutorial_interfaces/msg/if_algorhmitmsg.hpp"

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QThread>
#include <QLabel>
#include "fileout.h"
#include <QTimerEvent>
#include "global.h"
// qjq
#include "opencv2/core/mat.hpp"
#include <QTimer>
#include <QProcess>
#include "PictureBox.h"

#define SOPTOPCAM_SAVEBUFF		32
#define SOPTOPCAM_SYSPATH_MOTO	"./SAVE/SOPTOPCAM.bsd"

#define SOPTOPCAM_EXPOSURE_MAX    65535
#define SOPTOPCAM_EXPOSURE_USE    10000
#define SOPTOPCAM_EXPOSURE_MIN    20

using std::placeholders::_1;
using std::placeholders::_2;

class StartCameraThread; // 两个类相互引用，前置声明避免未定义错误

// qjq
struct Params
{
  std::vector<double> homography_matrix;
};

// ros2激光线信息
class Ros2lineinfo
{
public:
    std_msgs::msg::Header linehead;
    std::vector<cv::Point3f> linepoint;
};

// soptop相机设置
class SoptopCamera : public QObject
{
public:
    SoptopCamera();
    ~SoptopCamera();

    int argc;
    char **argv;

    void InitConnect(QLabel *lab_show);   //连接相机
    // qjq
    void InitConnect1();//连接相机
    Params ros_Params;
    PictureBox *m_lab_show1;

    cv_bridge::CvImagePtr cv_ptr;
    volatile bool b_int_show_image_inlab;
    bool callback_error; //图像卡住
//    bool luzhi;

    cv::VideoWriter writer;

    std::shared_ptr<rclcpp::AsyncParametersClient> _param_camera;
    std::shared_ptr<rclcpp::AsyncParametersClient> _param_gpio;
    std::shared_ptr<rclcpp::AsyncParametersClient> _param_camera_get;
    std::shared_ptr<rclcpp::AsyncParametersClient> _param_homography_matrix;
    std::shared_ptr<rclcpp::AsyncParametersClient> _param_homography_matrix_get;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr _pub_config;

    void ros_set_homography_matrix(Params ros_Params);
    void ros_config_set(std::string msg);


    void DisConnect();    //断开相机
    bool b_connect;       //是否连接相机
    bool stop_b_connect;  //是否成功断开相机

    int node_mode;      //0:订阅原图,1:订阅点云

    int i32_exposure;         //曝光值
    int i32_exposure_max;     //曝光最大值
    int i32_exposure_min;     //曝光最小值
    int i32_exposure_use;     //曝光默认值
    void updata_parameter();  //刷新相机参数

    cv::Mat *cv_image;    //相机图像
    QLabel *m_lab_show;   //显示控件位置

    Ros2lineinfo *cv_line;     //相机轮廓
    bool b_cv_lineEn;     //相机轮廓有效位

    void int_show_image_inlab();//刷新图像

    void write_para();     //保存相机参数
    void init_para();       //默认参赛

    volatile bool b_updataimage_finish; //获取最相机图像完成
    volatile bool b_updatacloud_finish; //获取点云图像完成

    volatile bool b_stopthred;

    volatile int callbacknumber;

protected:
    StartCameraThread *StartCamera_thread;// 定义相机线程类的指针

    virtual void timerEvent(QTimerEvent *event);
    int timerid1;

    volatile int oldcallbacknumber;

    void read_para();      //读取相机参数
    void check_para();     //核对相机参数
};

// 开启相机线程
class StartCameraThread : public QThread
{
public:
    StartCameraThread(SoptopCamera *statci_p);
protected:
    void run();
private:
    SoptopCamera *_p;
};

//qjq
typedef void (*pUpdateMainThreadQImage)(QImage image);

class Cambuild : public rclcpp::Node
{
public:
    Cambuild(SoptopCamera *statci_p);
    ~Cambuild();

private:
    SoptopCamera *_p;
public:
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;

    void callbackGlobalParam(std::shared_future<std::vector<rclcpp::Parameter>> future);
    void callbackMatrixParam(std::shared_future<std::vector<rclcpp::Parameter>> future);

//#ifdef DEBUG_MYINTERFACES
//    rclcpp::Subscription<tutorial_interfaces::msg::IfAlgorhmitmsg>::SharedPtr subscription_;
//#else
      rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription1_;
//#endif

//#ifdef DEBUG_MYINTERFACES
    //void topic_callback(const tutorial_interfaces::msg::IfAlgorhmitmsg msg)  const;
//#else
    void cambuild_callback(const sensor_msgs::msg::Image msg)  const;
//#endif

};

// 原图订阅节点
class Camshow : public rclcpp::Node
{
public:
    Camshow(SoptopCamera *statci_p);
    ~Camshow();

private:
    SoptopCamera *_p;
public:
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    void topic_callback(const sensor_msgs::msg::Image msg)  const;
};


// 点云订阅节点
class Cloudshow : public rclcpp::Node
{
public:
    Cloudshow(SoptopCamera *statci_p);
    ~Cloudshow();

private:
    SoptopCamera *_p;
public:
    rclcpp::Subscription<tutorial_interfaces::msg::IfAlgorhmitcloud>::SharedPtr subscricloud_;
    void cloud_callback(const tutorial_interfaces::msg::IfAlgorhmitcloud msg)  const;
};

#endif // SOPTOPCAMERA_H
