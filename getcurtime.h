#ifndef GETCURTIME_H
#define GETCURTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <QString>

// 保存图像和点云时，用当前系统时间给文件命名
class GetCurTime
{
public:
    GetCurTime();
    ~GetCurTime();

    void get_time_ms(QString* timeOut);
};

#endif // GETCURTIME_H
