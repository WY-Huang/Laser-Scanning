#ifndef CAM_SENSOR_H
#define CAM_SENSOR_H

#include "global.h"

#if _MSC_VER||WINDOWS_TCP
#include "soptocameratcpip.h"
#else
#include "soptopcamera.h"
#endif

class my_params;

class Cam_Sensor
{
public:
    static Cam_Sensor* Get();

    // soptop相机
    #if _MSC_VER||WINDOWS_TCP
        Soptocameratcpip sop_cam[CAMTOTALNUM];
    #else
        SoptopCamera sop_cam[CAMTOTALNUM];
    #endif


protected:
    Cam_Sensor();
    ~Cam_Sensor();
};
#endif // CAM_SENSOR_H
