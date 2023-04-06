#ifndef CAM_SENSOR_H
#define CAM_SENSOR_H

#include "global.h"
#include "soptopcamera.h"

class my_params;

class Cam_Sensor
{
public:
    static Cam_Sensor* Get();

    // soptop相机
    SoptopCamera sop_cam[CAMTOTALNUM];


protected:
    Cam_Sensor();
    ~Cam_Sensor();
};
#endif // CAM_SENSOR_H
