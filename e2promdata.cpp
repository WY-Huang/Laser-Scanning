#include "e2promdata.h"

E2promData::E2promData()
{
    #if _MSC_VER
        QString filePath =  "./SAVE";
        QDir dir(filePath);
        if(!dir.exists())
            QDir().mkdir(filePath);
    #else
        std::string dir = "./SAVE";
        if (access(dir.c_str(), 0) == -1)
        {
          mkdir("./SAVE",S_IRWXU);
        }
    #endif

    measurementDlg_leaser_data_mod_min=E2POOM_MEASUREMENTDLG_LEASER_DATA_MOD_MIN;
    measurementDlg_leaser_data_mod_max=E2POOM_MEASUREMENTDLG_LEASER_DATA_MOD_MAX;
    measurementDlg_leaser_data_mod_use=E2POOM_MEASUREMENTDLG_LEASER_DATA_MOD_USE;
    measurementDlg_deepimg_distance_min=E2POOM_MEASUREMENTDLG_DEEPING_DISTANCE_MIN;
    measurementDlg_deepimg_distance_max=E2POOM_MEASUREMENTDLG_DEEPING_DISTANCE_MAX;
    measurementDlg_deepimg_distance_use=E2POOM_MEASUREMENTDLG_DEEPING_DISTANCE_USE;
    measurementDlg_deepimg_speed_min=E2POOM_MEASUREMENTDLG_DEEPING_SPEED_MIN;
    measurementDlg_deepimg_speed_max=E2POOM_MEASUREMENTDLG_DEEPING_SPEED_MAX;
    measurementDlg_deepimg_speed_use=E2POOM_MEASUREMENTDLG_DEEPING_SPEED_USE;
    measurementDlg_deepimg_pisdis_min=E2POOM_MEASUREMENTDLG_DEEPING_PISDIS_MIN;
    measurementDlg_deepimg_pisdis_use=E2POOM_MEASUREMENTDLG_DEEPING_PISDIS_USE;
    measurementDlg_deepimg_pisdis_max=E2POOM_MEASUREMENTDLG_DEEPING_PISDIS_MAX;

    paramsetingDlg_col_add_distance_min=E2POOM_PARAMSETINGDLG_COL_ADD_DISTANCE_MIN;
    paramsetingDlg_col_add_distance_max=E2POOM_PARAMSETINGDLG_COL_ADD_DISTANCE_MAX;
    paramsetingDlg_col_add_distance_use=E2POOM_PARAMSETINGDLG_COL_ADD_DISTANCE_USE;
    paramsetingDlg_row_add_distance_min=E2POOM_PARAMSETINGDLG_ROW_ADD_DISTANCE_MIN;
    paramsetingDlg_row_add_distance_max=E2POOM_PARAMSETINGDLG_ROW_ADD_DISTANCE_MAX;
    paramsetingDlg_row_add_distance_use=E2POOM_PARAMSETINGDLG_ROW_ADD_DISTANCE_USE;

    // qjq
    camdlg_modposX1_min=E2POOM_CAMDLG_MODPOSX1_MIN;
    camdlg_modposX1_max=E2POOM_CAMDLG_MODPOSX1_MAX;
    camdlg_modposX1_use=E2POOM_CAMDLG_MODPOSX1_USE;
    camdlg_modposY1_min=E2POOM_CAMDLG_MODPOSY1_MIN;
    camdlg_modposY1_max=E2POOM_CAMDLG_MODPOSY1_MAX;
    camdlg_modposY1_use=E2POOM_CAMDLG_MODPOSY1_USE;
    camdlg_modposX2_min=E2POOM_CAMDLG_MODPOSX2_MIN;
    camdlg_modposX2_max=E2POOM_CAMDLG_MODPOSX2_MAX;
    camdlg_modposX2_use=E2POOM_CAMDLG_MODPOSX2_USE;
    camdlg_modposY2_min=E2POOM_CAMDLG_MODPOSY2_MIN;
    camdlg_modposY2_max=E2POOM_CAMDLG_MODPOSY2_MAX;
    camdlg_modposY2_use=E2POOM_CAMDLG_MODPOSY2_USE;
    camdlg_modposX3_min=E2POOM_CAMDLG_MODPOSX3_MIN;
    camdlg_modposX3_max=E2POOM_CAMDLG_MODPOSX3_MAX;
    camdlg_modposX3_use=E2POOM_CAMDLG_MODPOSX3_USE;
    camdlg_modposY3_min=E2POOM_CAMDLG_MODPOSY3_MIN;
    camdlg_modposY3_max=E2POOM_CAMDLG_MODPOSY3_MAX;
    camdlg_modposY3_use=E2POOM_CAMDLG_MODPOSY3_USE;
    camdlg_modposX4_min=E2POOM_CAMDLG_MODPOSX4_MIN;
    camdlg_modposX4_max=E2POOM_CAMDLG_MODPOSX4_MAX;
    camdlg_modposX4_use=E2POOM_CAMDLG_MODPOSX4_USE;
    camdlg_modposY4_min=E2POOM_CAMDLG_MODPOSY4_MIN;
    camdlg_modposY4_max=E2POOM_CAMDLG_MODPOSY4_MAX;
    camdlg_modposY4_use=E2POOM_CAMDLG_MODPOSY4_USE;
    camdlg_cvimg_posX1_min=E2POOM_CAMDLG_CVIMG_POSX1_MIN;

    camdlg_cvimg_posX1_max=E2POOM_CAMDLG_CVIMG_POSX1_MAX;
    camdlg_cvimg_posX1_use=E2POOM_CAMDLG_CVIMG_POSX1_USE;
    camdlg_cvimg_posY1_min=E2POOM_CAMDLG_CVIMG_POSY1_MIN;
    camdlg_cvimg_posY1_max=E2POOM_CAMDLG_CVIMG_POSY1_MAX;
    camdlg_cvimg_posY1_use=E2POOM_CAMDLG_CVIMG_POSY1_USE;
    camdlg_cvimg_posX2_min=E2POOM_CAMDLG_CVIMG_POSX2_MIN;
    camdlg_cvimg_posX2_max=E2POOM_CAMDLG_CVIMG_POSX2_MAX;
    camdlg_cvimg_posX2_use=E2POOM_CAMDLG_CVIMG_POSX2_USE;
    camdlg_cvimg_posY2_min=E2POOM_CAMDLG_CVIMG_POSY2_MIN;
    camdlg_cvimg_posY2_max=E2POOM_CAMDLG_CVIMG_POSY2_MAX;
    camdlg_cvimg_posY2_use=E2POOM_CAMDLG_CVIMG_POSY2_USE;
    camdlg_cvimg_posX3_min=E2POOM_CAMDLG_CVIMG_POSX3_MIN;
    camdlg_cvimg_posX3_max=E2POOM_CAMDLG_CVIMG_POSX3_MAX;
    camdlg_cvimg_posX3_use=E2POOM_CAMDLG_CVIMG_POSX3_USE;
    camdlg_cvimg_posY3_min=E2POOM_CAMDLG_CVIMG_POSY3_MIN;
    camdlg_cvimg_posY3_max=E2POOM_CAMDLG_CVIMG_POSY3_MAX;
    camdlg_cvimg_posY3_use=E2POOM_CAMDLG_CVIMG_POSY3_USE;
    camdlg_cvimg_posX4_min=E2POOM_CAMDLG_CVIMG_POSX4_MIN;
    camdlg_cvimg_posX4_max=E2POOM_CAMDLG_CVIMG_POSX4_MAX;
    camdlg_cvimg_posX4_use=E2POOM_CAMDLG_CVIMG_POSX4_USE;
    camdlg_cvimg_posY4_min=E2POOM_CAMDLG_CVIMG_POSY4_MIN;
    camdlg_cvimg_posY4_max=E2POOM_CAMDLG_CVIMG_POSY4_MAX;
    camdlg_cvimg_posY4_use=E2POOM_CAMDLG_CVIMG_POSY4_USE;

    read_para();
}

E2promData::~E2promData()
{

}

void E2promData::check_para()
{
    if(measurementDlg_leaser_data_mod<measurementDlg_leaser_data_mod_min||measurementDlg_leaser_data_mod>measurementDlg_leaser_data_mod_max)
        measurementDlg_leaser_data_mod=measurementDlg_leaser_data_mod_use;
    if(measurementDlg_deepimg_distance<=measurementDlg_deepimg_distance_min)
        measurementDlg_deepimg_distance=measurementDlg_deepimg_distance_use;
    if(measurementDlg_deepimg_speed<=measurementDlg_deepimg_speed_min)
        measurementDlg_deepimg_speed=measurementDlg_deepimg_speed_use;
    if(measurementDlg_deepimg_pisdis<measurementDlg_deepimg_pisdis_min||measurementDlg_deepimg_pisdis>=measurementDlg_deepimg_pisdis_max)
        measurementDlg_deepimg_pisdis=measurementDlg_deepimg_pisdis_use;
    if(paramsetingDlg_col_add_distance<paramsetingDlg_col_add_distance_min||paramsetingDlg_col_add_distance>paramsetingDlg_col_add_distance_max)
        paramsetingDlg_col_add_distance=paramsetingDlg_col_add_distance_use;
    if(paramsetingDlg_row_add_distance<paramsetingDlg_row_add_distance_min||paramsetingDlg_row_add_distance>paramsetingDlg_row_add_distance_max)
        paramsetingDlg_row_add_distance=paramsetingDlg_row_add_distance_use;

// qjq
    if(camdlg_modposX1<camdlg_modposX1_min||camdlg_modposX1>camdlg_modposX1_max)
       camdlg_modposX1=camdlg_modposX1_use;
   if(camdlg_modposY1<camdlg_modposY1_min||camdlg_modposY1>camdlg_modposY1_max)
       camdlg_modposY1=camdlg_modposY1_use;
   if(camdlg_modposX2<camdlg_modposX2_min||camdlg_modposX2>camdlg_modposX2_max)
       camdlg_modposX2=camdlg_modposX2_use;
   if(camdlg_modposY2<camdlg_modposY2_min||camdlg_modposY2>camdlg_modposY2_max)
       camdlg_modposY2=camdlg_modposY2_use;
   if(camdlg_modposX3<camdlg_modposX3_min||camdlg_modposX3>camdlg_modposX3_max)
       camdlg_modposX3=camdlg_modposX3_use;
   if(camdlg_modposY3<camdlg_modposY3_min||camdlg_modposY3>camdlg_modposY3_max)
       camdlg_modposY3=camdlg_modposY3_use;
   if(camdlg_modposX4<camdlg_modposX4_min||camdlg_modposX4>camdlg_modposX4_max)
       camdlg_modposX4=camdlg_modposX4_use;
   if(camdlg_modposY4<camdlg_modposY4_min||camdlg_modposY4>camdlg_modposY4_max)
       camdlg_modposY4=camdlg_modposY4_use;
   if(camdlg_cvimg_posX1<camdlg_cvimg_posX1_min||camdlg_cvimg_posX1>camdlg_cvimg_posX1_max)
       camdlg_cvimg_posX1=camdlg_cvimg_posX1_use;
   if(camdlg_cvimg_posY1<camdlg_cvimg_posY1_min||camdlg_cvimg_posY1>camdlg_cvimg_posY1_max)
       camdlg_cvimg_posY1=camdlg_cvimg_posY1_use;
   if(camdlg_cvimg_posX2<camdlg_cvimg_posX2_min||camdlg_cvimg_posX2>camdlg_cvimg_posX2_max)
       camdlg_cvimg_posX2=camdlg_cvimg_posX2_use;
   if(camdlg_cvimg_posY2<camdlg_cvimg_posY2_min||camdlg_cvimg_posY2>camdlg_cvimg_posY2_max)
       camdlg_cvimg_posY2=camdlg_cvimg_posY2_use;
   if(camdlg_cvimg_posX3<camdlg_cvimg_posX3_min||camdlg_cvimg_posX3>camdlg_cvimg_posX3_max)
       camdlg_cvimg_posX3=camdlg_cvimg_posX3_use;
   if(camdlg_cvimg_posY3<camdlg_cvimg_posY3_min||camdlg_cvimg_posY3>camdlg_cvimg_posY3_max)
       camdlg_cvimg_posY3=camdlg_cvimg_posY3_use;
   if(camdlg_cvimg_posX4<camdlg_cvimg_posX4_min||camdlg_cvimg_posX4>camdlg_cvimg_posX4_max)
       camdlg_cvimg_posX4=camdlg_cvimg_posX4_use;
   if(camdlg_cvimg_posY4<camdlg_cvimg_posY4_min||camdlg_cvimg_posY4>camdlg_cvimg_posY4_max)
       camdlg_cvimg_posY4=camdlg_cvimg_posY4_use;
}

void E2promData::read_para()
{
    read_measurementDlg_para();
    read_paramsetingDlg_para();
    // qjq
    read_cambuilddlg_para();
}

void E2promData::read_measurementDlg_para()
{
    Uint8 *buff=NULL;
    CFileOut fo;

    buff=new Uint8[E2POOM_MEASUREMENTDLG_SAVEBUFF];
    if(buff==NULL)
        return;
    if(0 > fo.ReadFile((char*)E2POOM_MEASUREMENTDLG_SYSPATH_MOTO,buff,E2POOM_MEASUREMENTDLG_SAVEBUFF))
    {
        init_measurementDlg_para();
        if(buff!=NULL)
        {
          delete []buff;
          buff=NULL;
        }
    }
    else
    {
      Int8 *i8_p;
      float *f_p;

      i8_p = (Int8*)buff;
      measurementDlg_leaser_data_mod=*i8_p;
      i8_p++;
      f_p = (float*)i8_p;
      measurementDlg_deepimg_distance=*f_p;
      f_p++;
      measurementDlg_deepimg_speed=*f_p;
      f_p++;
      measurementDlg_deepimg_pisdis=*f_p;
      f_p++;
    }
    if(buff!=NULL)
    {
      delete []buff;
      buff=NULL;
    }
}

void E2promData::write_measurementDlg_para()
{
    Uint8 *buff=NULL;
    CFileOut fo;

    check_para();
    buff=new Uint8[E2POOM_MEASUREMENTDLG_SAVEBUFF];
    if(buff==NULL)
      return;

    Int8 *i8_p;
    float *f_p;

    i8_p = (Int8*)buff;
    *i8_p=measurementDlg_leaser_data_mod;
    i8_p++;
    f_p = (float*)i8_p;
    *f_p=measurementDlg_deepimg_distance;
    f_p++;
    *f_p=measurementDlg_deepimg_speed;
    f_p++;
    *f_p=measurementDlg_deepimg_pisdis;
    f_p++;


    fo.WriteFile((char*)E2POOM_MEASUREMENTDLG_SYSPATH_MOTO,buff,E2POOM_MEASUREMENTDLG_SAVEBUFF);

    if(buff!=NULL)
    {
      delete []buff;
      buff=NULL;
    }
}

void E2promData::init_measurementDlg_para()
{
    measurementDlg_leaser_data_mod=measurementDlg_leaser_data_mod_use;
    measurementDlg_deepimg_distance=measurementDlg_deepimg_distance_use;
    measurementDlg_deepimg_speed=measurementDlg_deepimg_speed_use;
    measurementDlg_deepimg_pisdis=measurementDlg_deepimg_pisdis_use;
}

void E2promData::read_paramsetingDlg_para()
{
    Uint8 *buff=NULL;
    CFileOut fo;

    buff=new Uint8[E2POOM_PARAMSETINGDLG_SAVEBUFF];
    if(buff==NULL)
        return;
    if(0 > fo.ReadFile((char*)E2POOM_PARAMSETINGDLG_SYSPATH_MOTO,buff,E2POOM_PARAMSETINGDLG_SAVEBUFF))
    {
        init_paramsetingDlg_para();
        if(buff!=NULL)
        {
          delete []buff;
          buff=NULL;
        }
    }
    else
    {
      float *f_p;

      f_p = (float*)buff;
      paramsetingDlg_col_add_distance=*f_p;
      f_p++;
      paramsetingDlg_row_add_distance=*f_p;
      f_p++;
    }
    if(buff!=NULL)
    {
      delete []buff;
      buff=NULL;
    }
}

void E2promData::write_paramsetingDlg_para()
{
    Uint8 *buff=NULL;
    CFileOut fo;

    check_para();
    buff=new Uint8[E2POOM_PARAMSETINGDLG_SAVEBUFF];
    if(buff==NULL)
      return;

    float *f_p;

    f_p = (float*)buff;
    *f_p=paramsetingDlg_col_add_distance;
    f_p++;
    *f_p=paramsetingDlg_row_add_distance;
    f_p++;


    fo.WriteFile((char*)E2POOM_PARAMSETINGDLG_SYSPATH_MOTO,buff,E2POOM_PARAMSETINGDLG_SAVEBUFF);

    if(buff!=NULL)
    {
      delete []buff;
      buff=NULL;
    }
}

void E2promData::init_paramsetingDlg_para()
{
    paramsetingDlg_col_add_distance=paramsetingDlg_col_add_distance_use;
    paramsetingDlg_row_add_distance=paramsetingDlg_row_add_distance_use;
}

// qjq
void E2promData::read_cambuilddlg_para(){

    Uint8 *buff=NULL;
        CFileOut fo;

        buff=new Uint8[E2POOM_CAMDLG_SAVEBUFF];
        if(buff==NULL)
            return;
        if(0 > fo.ReadFile((char*)E2POOM_CAMDLG_SYSPATH_MOTO,buff,E2POOM_CAMDLG_SAVEBUFF))
        {
            init_camdlg_para();
            if(buff!=NULL)
            {
              delete []buff;
              buff=NULL;
            }
        }
        else
        {
          Int32 *i32_p;

          i32_p = (Int32*)buff;
          camdlg_modposX1=*i32_p;
          i32_p++;
          camdlg_modposY1=*i32_p;
          i32_p++;
          camdlg_modposX2=*i32_p;
          i32_p++;
          camdlg_modposY2=*i32_p;
          i32_p++;
          camdlg_modposX3=*i32_p;
          i32_p++;
          camdlg_modposY3=*i32_p;
          i32_p++;
          camdlg_modposX4=*i32_p;
          i32_p++;
          camdlg_modposY4=*i32_p;
          i32_p++;
          camdlg_cvimg_posX1=*i32_p;
          i32_p++;
          camdlg_cvimg_posY1=*i32_p;
          i32_p++;
          camdlg_cvimg_posX2=*i32_p;
          i32_p++;
          camdlg_cvimg_posY2=*i32_p;
          i32_p++;
          camdlg_cvimg_posX3=*i32_p;
          i32_p++;
          camdlg_cvimg_posY3=*i32_p;
          i32_p++;
          camdlg_cvimg_posX4=*i32_p;
          i32_p++;
          camdlg_cvimg_posY4=*i32_p;
          i32_p++;
        }
        if(buff!=NULL)
        {
          delete []buff;
          buff=NULL;
        }


        check_para();
}


void E2promData::write_camdlg_para()
{
    Uint8 *buff=NULL;
    CFileOut fo;

    check_para();
    buff=new Uint8[E2POOM_CAMDLG_SAVEBUFF];
    if(buff==NULL)
      return;

    Int32 *i32_p;

    i32_p = (Int32*)buff;
    *i32_p=camdlg_modposX1;
    i32_p++;
    *i32_p=camdlg_modposY1;
    i32_p++;
    *i32_p=camdlg_modposX2;
    i32_p++;
    *i32_p=camdlg_modposY2;
    i32_p++;
    *i32_p=camdlg_modposX3;
    i32_p++;
    *i32_p=camdlg_modposY3;
    i32_p++;
    *i32_p=camdlg_modposX4;
    i32_p++;
    *i32_p=camdlg_modposY4;
    i32_p++;
    *i32_p=camdlg_cvimg_posX1;
    i32_p++;
    *i32_p=camdlg_cvimg_posY1;
    i32_p++;
    *i32_p=camdlg_cvimg_posX2;
    i32_p++;
    *i32_p=camdlg_cvimg_posY2;
    i32_p++;
    *i32_p=camdlg_cvimg_posX3;
    i32_p++;
    *i32_p=camdlg_cvimg_posY3;
    i32_p++;
    *i32_p=camdlg_cvimg_posX4;
    i32_p++;
    *i32_p=camdlg_cvimg_posY4;
    i32_p++;

    fo.WriteFile((char*)E2POOM_CAMDLG_SYSPATH_MOTO,buff,E2POOM_CAMDLG_SAVEBUFF);

    if(buff!=NULL)
    {
      delete []buff;
      buff=NULL;
    }
}

void E2promData::init_camdlg_para()
{
    camdlg_modposX1=camdlg_modposX1_use;
    camdlg_modposY1=camdlg_modposY1_use;
    camdlg_modposX2=camdlg_modposX2_use;
    camdlg_modposY2=camdlg_modposY2_use;
    camdlg_modposX3=camdlg_modposX3_use;
    camdlg_modposY3=camdlg_modposY3_use;
    camdlg_modposX4=camdlg_modposX4_use;
    camdlg_modposY4=camdlg_modposY4_use;
    camdlg_cvimg_posX1=camdlg_cvimg_posX1_use;
    camdlg_cvimg_posY1=camdlg_cvimg_posY1_use;
    camdlg_cvimg_posX2=camdlg_cvimg_posX2_use;
    camdlg_cvimg_posY2=camdlg_cvimg_posY2_use;
    camdlg_cvimg_posX3=camdlg_cvimg_posX3_use;
    camdlg_cvimg_posY3=camdlg_cvimg_posY3_use;
    camdlg_cvimg_posX4=camdlg_cvimg_posX4_use;
    camdlg_cvimg_posY4=camdlg_cvimg_posY4_use;
}
