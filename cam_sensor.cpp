#include "cam_sensor.h"

Cam_Sensor* Cam_Sensor::Get()
{
    static Cam_Sensor fun;// ???
    return &fun;
}

Cam_Sensor::Cam_Sensor()
{

}

Cam_Sensor::~Cam_Sensor()
{

}
