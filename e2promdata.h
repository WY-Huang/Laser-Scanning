#ifndef E2PROMDATA_H
#define E2PROMDATA_H

#include "tistdtypes.h"
#include "fileout.h"
#include "float.h"
#include "global.h"
#include <string>
#if _MSC_VER
#include <QDir>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

//主页面参数
#define E2POOM_MEASUREMENTDLG_SAVEBUFF          24
#define E2POOM_MEASUREMENTDLG_SYSPATH_MOTO			"./SAVE/E2P_MEASUREMENT_DLG.bsd"

#define E2POOM_MEASUREMENTDLG_LEASER_DATA_MOD_MIN				0         //显示模式最小值
#define E2POOM_MEASUREMENTDLG_LEASER_DATA_MOD_USE				0         //显示模式默认值   0原图，1轮廓,2轮廓点云,3深度图,4点云
#define E2POOM_MEASUREMENTDLG_LEASER_DATA_MOD_MAX				4         //显示模式最大值
#define E2POOM_MEASUREMENTDLG_DEEPING_DISTANCE_MIN      0         //深度图像采集距离最小值
#define E2POOM_MEASUREMENTDLG_DEEPING_DISTANCE_USE      10        //深度图像采集距离默认值
#define E2POOM_MEASUREMENTDLG_DEEPING_DISTANCE_MAX      FLT_MAX   //深度图像采集距离最大值
#define E2POOM_MEASUREMENTDLG_DEEPING_SPEED_MIN         0         //深度图像采集速度最小值
#define E2POOM_MEASUREMENTDLG_DEEPING_SPEED_USE         1         //深度图像采集速度默认值
#define E2POOM_MEASUREMENTDLG_DEEPING_SPEED_MAX         FLT_MAX   //深度图像采集速度最大值
#define E2POOM_MEASUREMENTDLG_DEEPING_PISDIS_MIN        0         //深度图每像素对应距离最小值
#define E2POOM_MEASUREMENTDLG_DEEPING_PISDIS_USE        0.5       //深度图每像素对应距离默认值
#define E2POOM_MEASUREMENTDLG_DEEPING_PISDIS_MAX        50        //深度图每像素对应距离最大值

//参数设置页面参数
#define E2POOM_PARAMSETINGDLG_SAVEBUFF           12
#define E2POOM_PARAMSETINGDLG_SYSPATH_MOTO			"./SAVE/E2P_PARAMSETING_DLG.bsd"

#define E2POOM_PARAMSETINGDLG_COL_ADD_DISTANCE_MIN      0         //横向插值最小值
#define E2POOM_PARAMSETINGDLG_COL_ADD_DISTANCE_USE      0.5       //横向插值默认值
#define E2POOM_PARAMSETINGDLG_COL_ADD_DISTANCE_MAX      50.0      //横向插值最大值
#define E2POOM_PARAMSETINGDLG_ROW_ADD_DISTANCE_MIN      0         //纵向插值最小值
#define E2POOM_PARAMSETINGDLG_ROW_ADD_DISTANCE_USE      2.0       //纵向插值默认值
#define E2POOM_PARAMSETINGDLG_ROW_ADD_DISTANCE_MAX      50.0      //纵向插值最大值

// qjq 标定参数
#define E2POOM_CAMDLG_SAVEBUFF              64
#define E2POOM_CAMDLG_SYSPATH_MOTO			"./SAVE/E2P_CAMDLG.bsd"

#define E2POOM_CAMDLG_MODPOSX1_MIN          0
#define E2POOM_CAMDLG_MODPOSX1_MAX          100000
#define E2POOM_CAMDLG_MODPOSX1_USE          0
#define E2POOM_CAMDLG_MODPOSY1_MIN          0
#define E2POOM_CAMDLG_MODPOSY1_MAX          100000
#define E2POOM_CAMDLG_MODPOSY1_USE          8000
#define E2POOM_CAMDLG_MODPOSX2_MIN          0
#define E2POOM_CAMDLG_MODPOSX2_MAX          100000
#define E2POOM_CAMDLG_MODPOSX2_USE          2750
#define E2POOM_CAMDLG_MODPOSY2_MIN          0
#define E2POOM_CAMDLG_MODPOSY2_MAX          100000
#define E2POOM_CAMDLG_MODPOSY2_USE          0
#define E2POOM_CAMDLG_MODPOSX3_MIN          0
#define E2POOM_CAMDLG_MODPOSX3_MAX          100000
#define E2POOM_CAMDLG_MODPOSX3_USE          5750
#define E2POOM_CAMDLG_MODPOSY3_MIN          0
#define E2POOM_CAMDLG_MODPOSY3_MAX          100000
#define E2POOM_CAMDLG_MODPOSY3_USE          0
#define E2POOM_CAMDLG_MODPOSX4_MIN          0
#define E2POOM_CAMDLG_MODPOSX4_MAX          100000
#define E2POOM_CAMDLG_MODPOSX4_USE          8500
#define E2POOM_CAMDLG_MODPOSY4_MIN          0
#define E2POOM_CAMDLG_MODPOSY4_MAX          100000
#define E2POOM_CAMDLG_MODPOSY4_USE          8000

#define E2POOM_CAMDLG_CVIMG_POSX1_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX1_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX1_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY1_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY1_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY1_USE       0
#define E2POOM_CAMDLG_CVIMG_POSX2_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX2_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX2_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY2_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY2_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY2_USE       0
#define E2POOM_CAMDLG_CVIMG_POSX3_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX3_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX3_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY3_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY3_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY3_USE       0
#define E2POOM_CAMDLG_CVIMG_POSX4_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX4_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX4_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY4_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY4_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY4_USE       0

class E2promData
{
public:
    E2promData();
    ~E2promData();

/****************************/
    //主页面参数
    Int8 measurementDlg_leaser_data_mod;  //显示模式，0原图，1轮廓,2轮廓点云,3深度图,4点云
    float measurementDlg_deepimg_distance;    //深度图像采集距离
    float measurementDlg_deepimg_speed;       //深度图像采集速度
    float measurementDlg_deepimg_pisdis;      //深度图每像素对应距离

    void read_measurementDlg_para();				//读取主页面参数
    void write_measurementDlg_para();				//保存主页面参数
    void init_measurementDlg_para();				//初始化主页面参数
    //默认参数
    Int8 measurementDlg_leaser_data_mod_min;    //显示模式最小值
    Int8 measurementDlg_leaser_data_mod_max;    //显示模式最大值
    Int8 measurementDlg_leaser_data_mod_use;    //显示模式默认值
    float measurementDlg_deepimg_distance_min;   //深度图像采集距离最小值
    float measurementDlg_deepimg_distance_max;   //深度图像采集距离最大值
    float measurementDlg_deepimg_distance_use;   //深度图像采集距离默认值
    float measurementDlg_deepimg_speed_min;       //深度图像采集速度最小值
    float measurementDlg_deepimg_speed_max;       //深度图像采集速度最大值
    float measurementDlg_deepimg_speed_use;       //深度图像采集速度默认值
    float measurementDlg_deepimg_pisdis_min;      //深度图每像素对应距离最小值
    float measurementDlg_deepimg_pisdis_max;      //深度图每像素对应距离最大值
    float measurementDlg_deepimg_pisdis_use;      //深度图每像素对应距离默认值

/****************************/
    //参数设置页面
    float paramsetingDlg_col_add_distance;    //横向最小插值距离
    float paramsetingDlg_row_add_distance;    //纵向最小插值距离

    void read_paramsetingDlg_para();				//读取参数设置页面参数
    void write_paramsetingDlg_para();				//保存参数设置页面参数
    void init_paramsetingDlg_para();				//初始化参数设置页面参数

    float paramsetingDlg_col_add_distance_min;    //横向最小插值距离最小值
    float paramsetingDlg_col_add_distance_max;    //横向最小插值距离最大值
    float paramsetingDlg_col_add_distance_use;    //横向最小插值距离默认值
    float paramsetingDlg_row_add_distance_min;    //纵向最小插值距离最小值
    float paramsetingDlg_row_add_distance_max;    //纵向最小插值距离最大值
    float paramsetingDlg_row_add_distance_use;    //纵向最小插值距离默认值

// qjq
    /**********************************/             //标定的参数及函数
    Int32 camdlg_modposX1;
    Int32 camdlg_modposY1;
    Int32 camdlg_modposX2;
    Int32 camdlg_modposY2;
    Int32 camdlg_modposX3;
    Int32 camdlg_modposY3;
    Int32 camdlg_modposX4;
    Int32 camdlg_modposY4;

    Int32 camdlg_cvimg_posX1;
    Int32 camdlg_cvimg_posY1;
    Int32 camdlg_cvimg_posX2;
    Int32 camdlg_cvimg_posY2;
    Int32 camdlg_cvimg_posX3;
    Int32 camdlg_cvimg_posY3;
    Int32 camdlg_cvimg_posX4;
    Int32 camdlg_cvimg_posY4;

    void write_camdlg_para();
    void init_camdlg_para();
    void read_cambuilddlg_para();

    Int32 camdlg_modposX1_min;
    Int32 camdlg_modposX1_max;
    Int32 camdlg_modposX1_use;
    Int32 camdlg_modposY1_min;
    Int32 camdlg_modposY1_max;
    Int32 camdlg_modposY1_use;
    Int32 camdlg_modposX2_min;
    Int32 camdlg_modposX2_max;
    Int32 camdlg_modposX2_use;
    Int32 camdlg_modposY2_min;
    Int32 camdlg_modposY2_max;
    Int32 camdlg_modposY2_use;
    Int32 camdlg_modposX3_min;
    Int32 camdlg_modposX3_max;
    Int32 camdlg_modposX3_use;
    Int32 camdlg_modposY3_min;
    Int32 camdlg_modposY3_max;
    Int32 camdlg_modposY3_use;
    Int32 camdlg_modposX4_min;
    Int32 camdlg_modposX4_max;
    Int32 camdlg_modposX4_use;
    Int32 camdlg_modposY4_min;
    Int32 camdlg_modposY4_max;
    Int32 camdlg_modposY4_use;

    Int32 camdlg_cvimg_posX1_min;
    Int32 camdlg_cvimg_posX1_max;
    Int32 camdlg_cvimg_posX1_use;
    Int32 camdlg_cvimg_posY1_min;
    Int32 camdlg_cvimg_posY1_max;
    Int32 camdlg_cvimg_posY1_use;
    Int32 camdlg_cvimg_posX2_min;
    Int32 camdlg_cvimg_posX2_max;
    Int32 camdlg_cvimg_posX2_use;
    Int32 camdlg_cvimg_posY2_min;
    Int32 camdlg_cvimg_posY2_max;
    Int32 camdlg_cvimg_posY2_use;
    Int32 camdlg_cvimg_posX3_min;
    Int32 camdlg_cvimg_posX3_max;
    Int32 camdlg_cvimg_posX3_use;
    Int32 camdlg_cvimg_posY3_min;
    Int32 camdlg_cvimg_posY3_max;
    Int32 camdlg_cvimg_posY3_use;
    Int32 camdlg_cvimg_posX4_min;
    Int32 camdlg_cvimg_posX4_max;
    Int32 camdlg_cvimg_posX4_use;
    Int32 camdlg_cvimg_posY4_min;
    Int32 camdlg_cvimg_posY4_max;
    Int32 camdlg_cvimg_posY4_use;
    /***************************/
    void write();

private:
    void read_para();				//读取
    void check_para();			//检查参数
};

#endif // E2PROMDATA_H
