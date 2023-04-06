#ifndef MY_PARAMS_H
#define MY_PARAMS_H
#include <opencv2/opencv.hpp>
#include "cam_sensor.h"
#include "soptopcamera.h"
#include "e2promdata.h"
#include "resultdata.h"

class Cam_Sensor;

class My_params
{
public:
    static My_params *Get();
    Cam_Sensor *cam;    //相机

    E2promData e2proomdata;	//存储的数据

    ResultData resultdata;		//实时结果数据

protected:
    My_params();
    ~My_params();
};

#endif // MY_PARAMS_H
