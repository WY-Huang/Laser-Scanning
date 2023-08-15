#ifndef SOPTOCAMERATCPIP_H
#define SOPTOCAMERATCPIP_H

#include <QLabel>
#include "global.h"

#if _MSC_VER||WINDOWS_TCP
//#include "PictureBox.h"
#include <opencv2/opencv.hpp>
#include <QThread>
#include "XTcp.h"
#include "opencv2/imgcodecs/legacy/constants_c.h"

#if _MSC_VER
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>

#define RECVBUFFER_MAX      CAMBUILD_IMAGE_HEIGHT*CAMBUILD_IMAGE_WIDTH*3

// 时间戳
class Stamp
{
public:
    int32_t sec;
    u_int32_t nanosec;
};
// 时间+帧名
class Header
{
public:
    Stamp stamp;
    QString frame_id;
};
// 轮廓信息
class Lasertrackoutcloud
{
public:
    float x;
    float y;
    int32_t u;
    int32_t v;
};

class Targetpointoutcloud
{
public:
    float x;
    float y;
    int32_t u;
    int32_t v;
    QString name;
};

class IFAlgorhmitcloud  //轮廓信息
{
public:
    Header header;
    std::vector<Lasertrackoutcloud> lasertrackoutcloud;
    std::vector<Targetpointoutcloud> targetpointoutcloud;
    bool solderjoints;
};

class tcprcvThread;

struct Params
{
  std::vector<double> homography_matrix;
};


class Soptocameratcpip
{
public:
    Soptocameratcpip();
    ~Soptocameratcpip();

    u_int8_t connect_mod;       // 当前连接方式: 0为获取图像，1为获取点云
    void InitConnect(QLabel *lab_show, QString hostName, int port);     // tcp方式获取图像
    void InitConnect_cloud(QString hostName,int port);                      // tcp方式获取点云
    void DisConnect();
    bool b_connect;

    cv::Mat cv_image;

    Params ros_Params;

    void StartRecord(QString filename);
    void StopRecord();

    volatile bool b_int_show_image_inlab;

    volatile bool b_updataimage_finish;

    volatile int callbacknumber;

    XTcp m_client;
    tcprcvThread *rcv_thread;
    bool b_rcv_thread;
    bool b_stop_rcv_thread;

    XTcp m_ftp;

    uchar *rcv_buf;

    bool luzhi;
    cv::VideoWriter writer;

    QString JsonToQstring(QJsonObject jsonObject);

    QJsonObject QstringToJson(QString jsonString);

    IFAlgorhmitcloud IfAlgorhmitcloud;

    volatile bool b_updatacloud_finish;

    void ros_set_homography_matrix(Params ros_Params);

protected:

    QLabel *m_lab_show;

    double Getfps();
};


class tcprcvThread : public QThread
{
    Q_OBJECT

public:
    tcprcvThread(Soptocameratcpip *statci_p);
    void Stop();
protected:
    void run();
private:
    Soptocameratcpip *_p;

};

#endif
#endif // SOPTOCAMERATCPIP_H
