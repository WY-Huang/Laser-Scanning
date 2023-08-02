#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_mcs = m_mcs->Get();
    pImage = cv::Mat::zeros(CAMIMAGE_HEIGHT,CAMIMAGE_WIDTH,CV_8UC1);

    paramset = new laser_paramsetingdlg(m_mcs);
//    imgShowLabel = new LabelImageViewer;
//    indexImgShowLabel = ui->stackedWidget->addWidget(imgShowLabel);

    InitSetEdit();  // 界面初始化
    vtk_init();
    initChart();
//  自定义定时器槽 slot_timer_tragetor_clould()
    timer_tragetor_clould=new QTimer(this);
    connect(timer_tragetor_clould, SIGNAL(timeout()), this, SLOT(slot_timer_tragetor_clould()));

    finish_line = false;
    camera_reset_once = true;   // 首次重置相机视角
    camera_reset_always = false;
    finish_cloud = false;
    updateVTKShow = true;
    imgshow_thread = new ImgWindowShowThread(this);
    b_int_show_cvimage_inlab_finish = true;
    b_init_show_pclclould_list_finish=true;
    b_int_show_record_finish=true;
    recordVideo = false;

    connect(imgshow_thread, SIGNAL(Send_show_cvimage_inlab(cv::Mat)), this, SLOT(int_show_cvimage_inlab(cv::Mat)));
    connect(imgshow_thread, SIGNAL(Send_show_pclclould_list(pcl::PointCloud<pcl::PointXYZRGB>::Ptr)), this,
            SLOT(init_show_pclclould_list(pcl::PointCloud<pcl::PointXYZRGB>::Ptr)));
//    connect(imgshow_thread, SIGNAL(Send_show_record(QString)), this, SLOT(int_show_record(QString)));
//    b_imgshow_thread = true;
//    imgshow_thread->start();

    // 打开图像和点云文件
    connect(ui->openFile, &QAction::triggered, this, [=](){
        {
          if(m_mcs->resultdata.b_deepimg_pushoneline==false)
          {
            imgshow_thread->Lock();
            QString fileName = QFileDialog::getOpenFileName(this, "open Image", "", "Image File(*.bmp *.pcd *.tiff *.BMP *.PCD *.TIFF)");
            QTextCodec* code = QTextCodec::codecForName("gb18030");
            std::string name = code->fromUnicode(fileName).data();
            if(name.size()>0)
            {
                showImgPcd = new showImgPcdDlg;
                showImgPcd->setWindowTitle(fileName);
                showImgPcd->showpoint(name);
                showImgPcd->exec();
                delete showImgPcd;
            }
            imgshow_thread->unLock();
          }
          else
          {
              QString current_date = GetCurTime_M();
              ui->textBrowser->append(current_date + "等采集数据完成后再进行此操作");
          }
        }
    });

    // 保存图像和点云文件
    connect(ui->saveFile, &QAction::triggered, this, [=](){
           u8_save_data=1;

           std::string saveimg = "./USER_DATA";
           if (access(saveimg.c_str(), 0) == -1)
           {
             mkdir("./USER_DATA",S_IRWXU);
           }

           QString current_date = GetCurTime_M();
           ui->textBrowser->append(current_date + "保存成功");
       });

    // 连接按钮控制，lambda表达式
    connect(ui->connectCam, &QAction::triggered, this, [=]()
    {

        if (m_mcs->cam->sop_cam[0].b_connect == false)
        {
            b_imgshow_thread = true;
            imgshow_thread->start();
            img_windowshow(true, ui->imgShow);
            UpdateUi();
        }
        else
        {
            img_windowshow(false, ui->imgShow);
            UpdateUi();
        }
    });

    // 设置相机参数
    connect(ui->applyBtn,&QPushButton::clicked, this, [=](){

        if(m_mcs->resultdata.link_param_state==true)
        {
            int alg0_99_threshold=ui->exposureValue->text().toInt();
            if(alg0_99_threshold<20||alg0_99_threshold>65535)
            {
                QString current_date = GetCurTime_M();
                ui->textBrowser->append(current_date + "设置相机曝光值超出范围");
            }
            else
            {
                uint16_t tab_reg[1];
                tab_reg[0]=alg0_99_threshold;
                int rc=modbus_write_registers(m_mcs->resultdata.ctx_param,ALS103_EXPOSURE_TIME_REG_ADD,1,tab_reg);
                if(rc!=1)
                {
                    QString current_date = GetCurTime_M();
                    ui->textBrowser->append(current_date + "设置曝光参数失败");
                }
                else
                {
                    m_mcs->cam->sop_cam[0].i32_exposure=alg0_99_threshold;
                    m_mcs->cam->sop_cam[0].write_para();

                    QString current_date = GetCurTime_M();
                    ui->textBrowser->append(current_date + "设置曝光参数成功");
                }
            }
        }
        else
        {
            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "请连接相机后再设置曝光值");
        }
    });

    // 显示原图按钮
    connect(ui->showCam, &QAction::triggered, this, [=](){
        if(m_mcs->cam->sop_cam[0].b_connect==true&&m_mcs->e2proomdata.measurementDlg_leaser_data_mod!=0)
        {
          m_mcs->cam->sop_cam[0].DisConnect();
          m_mcs->cam->sop_cam[0].node_mode=0;
          m_mcs->cam->sop_cam[0].InitConnect(ui->imgShow);
        }
        m_mcs->e2proomdata.measurementDlg_leaser_data_mod=0;
        QString current_date = GetCurTime_M();
        ui->textBrowser->append(current_date + "切换为显示原图模式");
        UpdateUi();
        ui->page_3->setVisible(false);
    });

    // 显示中心线按钮
    connect(ui->showCenter, &QAction::triggered, this, [=](){

        if(m_mcs->cam->sop_cam[0].b_connect==true&&m_mcs->e2proomdata.measurementDlg_leaser_data_mod==0)
        {
          m_mcs->cam->sop_cam[0].DisConnect();
          m_mcs->cam->sop_cam[0].node_mode=1;
          m_mcs->cam->sop_cam[0].InitConnect(ui->imgShow);
        }
        m_mcs->e2proomdata.measurementDlg_leaser_data_mod=1;
        QString current_date = GetCurTime_M();
        ui->textBrowser->append(current_date + "切换为显示中心线模式");
        UpdateUi();
        ui->page_3->setVisible(false);
    });

    //显示轮廓
    connect(ui->showTrajectory, &QAction::triggered, this, [=](){
            if(m_mcs->cam->sop_cam[0].b_connect==true&&m_mcs->e2proomdata.measurementDlg_leaser_data_mod==0)
            {
              m_mcs->cam->sop_cam[0].DisConnect();
              m_mcs->cam->sop_cam[0].node_mode=1;
              m_mcs->cam->sop_cam[0].InitConnect(ui->imgShow);
            }
            m_mcs->e2proomdata.measurementDlg_leaser_data_mod=2;

            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "切换为显示轮廓模式");
            ui->stackedWidget->setCurrentIndex(2);
            ui->page_3->setVisible(false);

//            create_axis();
           // m_mcs->resultdata.viewer->removeAllPointClouds();
         //   m_mcs->resultdata.viewer->removeAllShapes();
        });

    // 显示深度图
    connect(ui->showDepth, &QAction::triggered, this, [=](){
            if(m_mcs->cam->sop_cam[0].b_connect==true&&m_mcs->e2proomdata.measurementDlg_leaser_data_mod==0)
            {
              m_mcs->cam->sop_cam[0].DisConnect();
              m_mcs->cam->sop_cam[0].node_mode=1;
              m_mcs->cam->sop_cam[0].InitConnect(ui->imgShow);
            }
            m_mcs->e2proomdata.measurementDlg_leaser_data_mod=3;

            ui->stackedWidget->setCurrentIndex(0);
            ui->page_3->setVisible(true);

            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "切换为显示深度图模式");
//            UpdateUi();
        });

    // 一键采集深度图
    connect(ui->captureDepthBtn, &QPushButton::clicked, this, [=](){
            if(m_mcs->resultdata.b_deepimg_working==false)//深度图空闲
            {
            #if ACQUISITION_MOD==AUTO_MOD
                start_deepimg();
            #else

            #endif
                QString current_date = GetCurTime_M();
                ui->textBrowser->append(current_date + "正在采集数据...");
            }
            else
            {
                stop_deepimg();

                QString current_date = GetCurTime_M();
                ui->textBrowser->append(current_date + "手动停止采集");
            }
        });

    // 显示点云图
    connect(ui->showPointCloud, &QAction::triggered, this, [=](){
           if(m_mcs->cam->sop_cam[0].b_connect==true&&m_mcs->e2proomdata.measurementDlg_leaser_data_mod==0)
           {
             m_mcs->cam->sop_cam[0].DisConnect();
             m_mcs->cam->sop_cam[0].node_mode=1;
             m_mcs->cam->sop_cam[0].InitConnect(ui->imgShow);
           }
           m_mcs->e2proomdata.measurementDlg_leaser_data_mod=4;
           ui->stackedWidget->setCurrentIndex(1);
           ui->page_3->setVisible(true);

           QString current_date = GetCurTime_M();
           ui->textBrowser->append(current_date + "切换为显示点云模式");
//           UpdateUi();
       });

    // 点云自适应缩放
    connect(ui->actionactionCemaraView, &QAction::toggled, this, [=](){ // bool checked
//        float zoomFactor = 1.5;
//        ui->graphicsView->chart()->
//        ui->graphicsView->update();
//        vtkCamera* camera = renderer->GetActiveCamera();
//        camera->SetPosition(-1, 0, 0);
//        camera->SetViewUp (0, 0, -1);
//        camera->SetFocalPoint (0, 0, 0);
//        renderer->ResetCamera();
//        ui->pclShow->GetRenderWindow()->Render();
//        ui->pclShow->update();
//        if(checked)
//        {
//            camera_reset_always = true;
//        }
//        else
//        {
//            camera_reset_always = false;
//        }
    });
    // 数据暂停刷新按钮
    connect(ui->actionPause, &QAction::toggled, this, [=](bool checked){
        if (checked)
        {
            updateVTKShow = false;
        }
        else
        {
            updateVTKShow = true;
        }

    });
    // 点云选两点测距按钮
    connect(ui->actiondisMeasure, &QAction::toggled, this, [=](bool checked){
        doDisMeasure(checked);
    });

    // 点云数据视图设置的工具栏
     connect(ui->actionbackward,&QAction::triggered, this, [=]()
       {
//        vtkNew<vtkCamera> camera;
        vtkCamera* camera = renderer->GetActiveCamera();
        camera->SetPosition(-1, 0, 0);
        camera->SetViewUp (0, 0, -1);
        camera->SetFocalPoint (0, 0, 0);
//        renderer->SetActiveCamera(camera);
        renderer->ResetCamera();
//        renderer->Render();
        ui->pclShow->GetRenderWindow()->Render();
        ui->pclShow->update();
    });
     connect(ui->actiondown,&QAction::triggered, this, [=]()
       {
        vtkCamera* camera = renderer->GetActiveCamera();
        camera->SetPosition(0, 0, -1);
        camera->SetViewUp (0, 0, -1);
        camera->SetFocalPoint (0, 0, 0);
        renderer->ResetCamera();
        ui->pclShow->GetRenderWindow()->Render();
        ui->pclShow->update();
    });
     connect(ui->actionforward,&QAction::triggered, this, [=]()
       {
        vtkCamera* camera = renderer->GetActiveCamera();
        camera->SetPosition(1, 0, 0);
        camera->SetViewUp (0, 0, -1);
        camera->SetFocalPoint (0, 0, 0);
        renderer->ResetCamera();
        ui->pclShow->GetRenderWindow()->Render();
        ui->pclShow->update();
    });
     connect(ui->actionleft,&QAction::triggered, this, [=]()
       {
        vtkCamera* camera = renderer->GetActiveCamera();
        camera->SetPosition(0, 1, 0);
        camera->SetViewUp (0, 0, -1);
        camera->SetFocalPoint (0, 0, 0);
        renderer->ResetCamera();
        ui->pclShow->GetRenderWindow()->Render();
        ui->pclShow->update();
    });
     connect(ui->actionright,&QAction::triggered, this, [=]()
       {
        vtkCamera* camera = renderer->GetActiveCamera();
        camera->SetPosition(0, -1, 0);
        camera->SetViewUp (0, 0, -1);
        camera->SetFocalPoint (0, 0, 0);
        renderer->ResetCamera();
        ui->pclShow->GetRenderWindow()->Render();
        ui->pclShow->update();
    });
     connect(ui->actionup,&QAction::triggered, this, [=]()
       {
        vtkCamera* camera = renderer->GetActiveCamera();
        camera->SetPosition(0, 0, 1);
        camera->SetViewUp (0, 0, -1);
        camera->SetFocalPoint (0, 0, 0);
        renderer->ResetCamera();
        ui->pclShow->GetRenderWindow()->Render();
        ui->pclShow->update();
    });

    //激光头标定
    connect(ui->calibration, &QAction::triggered, this, [=](){
        if(m_mcs->resultdata.link_param_state==true)
        {
           m_mcs->e2proomdata.measurementDlg_leaser_data_mod = 5;
           u_int16_t tab_reg[1];
           tab_reg[0] = 1;
           int rc = modbus_write_registers(m_mcs->resultdata.ctx_param, ALS_SHOW_STEP_REG_ADD, 1, tab_reg);
           if(rc != 1)
           {
            //   if(ui->checkBox->isChecked()==false)
            //      ui->record->append(QString::fromLocal8Bit("写入视图步骤失败"));
           }
           else
           {
               m_mcs->cam->sop_cam[0].DisConnect();
               m_mcs->cam->sop_cam[0].node_mode = 3;
               m_mcs->cam->sop_cam[0].InitConnect1();

               cambuild = new cambuilddlg(m_mcs);
               cambuild->init_dlg_show();
               cambuild->setWindowTitle(QString::fromLocal8Bit("激光平面标定"));
               cambuild->exec();
               cambuild->close_dlg_show();

               delete cambuild;

    //           tab_reg[0]=1;
    //           int rc=modbus_write_registers(m_mcs->resultdata.ctx_param,ALS_SHOW_STEP_REG_ADD,1,tab_reg);
    //           if(rc!=1)
    //           {
    //               if(ui->checkBox->isChecked()==false)
    //                 ui->record->append(QString::fromLocal8Bit("恢复视图步骤失败"));
    //           }
           }
        }
//        else
//        {
//           if(ui->checkBox->isChecked()==false)
//           {
//               QString current_date = GetCurTime_M();
//               ui->textBrowser->append(current_date + "请连接相机后再进行激光头标定");
//           }

//        }
    });

    // 参数设置
    connect(ui->setParam, &QAction::triggered, this, [=](){
        if(m_mcs->resultdata.link_param_state==true)
        {
          paramset->setWindowTitle("参数设置");
          paramset->Initparam(m_mcs);
          paramset->exec();
        }
        else
        {
            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "请先连接传感器再进行参数设置");
        }
    });

    // IP设置
    connect(ui->actionIP, &QAction::triggered, this, [=](){
            bool isbool;
            QString strString = QInputDialog::getText(this, "IP设置", "请输入IP地址:",
                                                      QLineEdit::Normal, ipAddress, &isbool);

            if(isbool && !strString.isEmpty())  // 是否点击了确认及判断字符串是否为空
            {
                ipAddress = strString;
            }

    });

    // 重启按钮
    connect(ui->action_restart, &QAction::triggered, this, &MainWindow::doDockerRestart);

    // 将图像录制为视频
    connect(ui->recordAsVideo, &QAction::triggered, this, &MainWindow::recordAsVideo);

}


MainWindow::~MainWindow()
{
    m_mcs->cam->sop_cam[0].DisConnect();
    imgshow_thread->Stop();
    imgshow_thread->quit();
    imgshow_thread->wait();
    if(m_mcs->resultdata.link_result_state==true)
    {
        close_camer_modbus();
        modbus_free(m_mcs->resultdata.ctx_result);
        m_mcs->resultdata.link_result_state=false;

        QString current_date = GetCurTime_M();
        ui->textBrowser->append(current_date + "控制端口关闭");
    }
    if(m_mcs->resultdata.link_param_state==true)
    {
        modbus_close(m_mcs->resultdata.ctx_param);
        m_mcs->resultdata.link_param_state=false;

        QString current_date = GetCurTime_M();
        ui->textBrowser->append(current_date + "参数端口关闭");
    }
    delete timer_tragetor_clould;
    delete paramset;
    delete ui;
}

// 图像显示线程
ImgWindowShowThread::ImgWindowShowThread(MainWindow *statci_p)
{
    _p=statci_p;
    lock=false;
}

void ImgWindowShowThread::Stop()
{
  if(_p->b_imgshow_thread==true)
  {
    _p->stop_b_imgshow_thread=false;
    _p->b_imgshow_thread=false;
    while (_p->stop_b_imgshow_thread==false)
    {
      sleep(0);
    }
  }
}

void ImgWindowShowThread::Lock()
{
  lock=true;
}

void ImgWindowShowThread::unLock()
{
  lock=false;
}

void ImgWindowShowThread::run()
{
    while(1)
    {
        if(_p->b_imgshow_thread==true)
        {
             if(lock==false)
             {
                // 0原图，1轮廓, 2轮廓点云, 3深度图, 4点云, 5标定
                switch(_p->m_mcs->e2proomdata.measurementDlg_leaser_data_mod)
                {
                    //显示原图（不做处理）
                    case 0:
                    {
                        if(_p->m_mcs->cam->sop_cam[0].b_updataimage_finish==true)
                        {
                          _p->pImage=_p->m_mcs->cam->sop_cam[0].cv_image->clone();
                          if(_p->b_int_show_cvimage_inlab_finish==true)
                          {
                              _p->b_int_show_cvimage_inlab_finish=false;
                              qRegisterMetaType< cv::Mat >("cv::Mat"); //传递自定义类型信号时要添加注册
                              emit Send_show_cvimage_inlab(_p->pImage);
                          }
                          _p->m_mcs->cam->sop_cam[0].b_updataimage_finish=false;
                        }

                        if(_p->u8_save_data==1)//保存结果
                        {
                           QString str=_p->save_imgdata_cvimage(_p->pImage);
                           _p->u8_save_data=0;
                           /*
                           if(_p->b_int_show_record_finish==true)
                           {
                             _p->b_int_show_record_finish=false;
                             qRegisterMetaType< QString >("QString");
                             QString strname="数据保存在:";
                             strname=strname+str;
                             emit Send_show_record(strname);
                           }
                           */
                         }

                    }
                    break;

                    // 显示中心线
                    case 1:
                    {
                       if(_p->m_mcs->cam->sop_cam[0].b_updatacloud_finish==true)
                       {
                          _p->m_mcs->resultdata.cv_imagelinecenter=cv::Mat::zeros(CAMIMAGE_HEIGHT,CAMIMAGE_WIDTH,CV_8UC3);
                          if(_p->m_mcs->cam->sop_cam[0].b_cv_lineEn==true)
                          {
                             _p->cv_line=(*_p->m_mcs->cam->sop_cam[0].cv_line).linepoint;
                             for(int n=0;n<(int)_p->cv_line.size();n++)
                             {
                                if(_p->cv_line[n].z<=_p->m_mcs->resultdata.cv_imagelinecenter.rows-1&&_p->cv_line[n].z>=0)
                                {
                                  int x=n;
                                  int y=_p->cv_line[n].z;
                                  y=_p->m_mcs->resultdata.cv_imagelinecenter.rows-1-y;
                                  _p->m_mcs->resultdata.cv_imagelinecenter.data[y*_p->m_mcs->resultdata.cv_imagelinecenter.cols*3+x*3]=0;
                                  _p->m_mcs->resultdata.cv_imagelinecenter.data[y*_p->m_mcs->resultdata.cv_imagelinecenter.cols*3+x*3+1]=255;
                                  _p->m_mcs->resultdata.cv_imagelinecenter.data[y*_p->m_mcs->resultdata.cv_imagelinecenter.cols*3+x*3+2]=0;
                                }
                             }
                          }

                          if(_p->b_int_show_cvimage_inlab_finish==true)
                          {
                              _p->b_int_show_cvimage_inlab_finish=false;
                              qRegisterMetaType< cv::Mat >("cv::Mat"); //传递自定义类型信号时要添加注册
                              emit Send_show_cvimage_inlab(_p->m_mcs->resultdata.cv_imagelinecenter);
                          }
                          _p->m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                       }

                       if(_p->u8_save_data==1)//保存结果
                       {
                          QString str=_p->save_imgdata_cvimage(_p->m_mcs->resultdata.cv_imagelinecenter);
                          _p->u8_save_data = 0;
                       }

                    }
                    break;

                    //显示轮廓点云
                    case 2:
                    {
                       if(_p->m_mcs->cam->sop_cam[0].b_updatacloud_finish==true)
                       {
                           if(_p->m_mcs->cam->sop_cam[0].b_cv_lineEn==true)
                           {
                              _p->cv_line=(*_p->m_mcs->cam->sop_cam[0].cv_line).linepoint;
                              _p->pclclass.cvpoint3f_to_oneline_pclclould(_p->cv_line,0,&_p->m_mcs->resultdata.ptr_pcl_lineclould);
                              if(_p->b_init_show_pclclould_list_finish==true)
                              {
                                  _p->b_init_show_pclclould_list_finish=false;
                                  _p->finish_line=true;

                                  qRegisterMetaType<pcl::PointCloud<pcl::PointXYZRGB>::Ptr>("pcl::PointCloud<pcl::PointXYZRGB>::Ptr"); //传递自定义类型信号时要添加注册
                                  emit Send_show_pclclould_list(_p->m_mcs->resultdata.ptr_pcl_lineclould);
                              }
                           }
                           _p->m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                       }
                       if(_p->u8_save_data==1)//保存结果
                       {
                          QString str=_p->save_pcldata_pclclould(_p->m_mcs->resultdata.ptr_pcl_lineclould);
                          _p->u8_save_data=0;
                          /*if(_p->b_int_show_record_finish==true)
                          {
                            _p->b_int_show_record_finish=false;
                            qRegisterMetaType< QString >("QString");
                            QString strname="数据保存在:";
                            strname=strname+str;
                            emit Send_show_record(strname);
                          }*/
                       }
                    }
                    break;

                    // 显示深度图
                    case 3:
                    {
                       if(_p->m_mcs->cam->sop_cam[0].b_updatacloud_finish==true)
                       {
                           if(_p->m_mcs->resultdata.b_deepimg_pushoneline==true)
                           {
                               _p->cv_line=((*_p->m_mcs->cam->sop_cam[0].cv_line).linepoint);
                               if(_p->m_mcs->resultdata.b_firstpoint==false)
                               {
                                  _p->m_mcs->resultdata.b_firstpoint=true;
                                  _p->m_mcs->resultdata.f_deepimg_y=0;
                                  _p->m_mcs->resultdata.timeinfo_st=((*_p->m_mcs->cam->sop_cam[0].cv_line).linehead);
                               }
                               else
                               {
                                  int32_t sec2=((*_p->m_mcs->cam->sop_cam[0].cv_line).linehead).stamp.sec;
                                  int32_t sec1=_p->m_mcs->resultdata.timeinfo_st.stamp.sec;
                                  uint32_t nsec2=((*_p->m_mcs->cam->sop_cam[0].cv_line).linehead).stamp.nanosec;
                                  uint32_t nsec1=_p->m_mcs->resultdata.timeinfo_st.stamp.nanosec;
                                  double fsec2=(double)sec2+(double)nsec2/1000000000.0;
                                  double fsec1=(double)sec1+(double)nsec1/1000000000.0;
                                  _p->m_mcs->resultdata.f_deepimg_y=(fsec2-fsec1)*_p->m_mcs->e2proomdata.measurementDlg_deepimg_speed;
                               }
                               _p->pclclass.cvpoint3f_to_oneline_pclclould(_p->cv_line,_p->m_mcs->resultdata.f_deepimg_y,&_p->m_mcs->resultdata.ptr_pcl_lineclould);
                               *(_p->m_mcs->resultdata.ptr_pcl_deepclould)=*(_p->m_mcs->resultdata.ptr_pcl_deepclould)+*(_p->m_mcs->resultdata.ptr_pcl_lineclould);
                           }
                           _p->m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                       }
                       if(_p->m_mcs->resultdata.b_deepimg_showclould_finish==true)
                       {   //采集完成,点云转深度图
                           _p->m_mcs->resultdata.b_deepimg_showclould_finish=false;

                           _p->pclclass.pointCloud2imgI(&_p->m_mcs->resultdata.ptr_pcl_deepclould,&_p->m_mcs->resultdata.cv_deepimg,_p->m_mcs->e2proomdata.measurementDlg_deepimg_pisdis);
                           _p->pclclass.addpoint_image(&_p->m_mcs->resultdata.cv_deepimg,
                                                       (int)(_p->m_mcs->e2proomdata.paramsetingDlg_col_add_distance/_p->m_mcs->e2proomdata.measurementDlg_deepimg_pisdis+0.5),
                                                       (int)(_p->m_mcs->e2proomdata.paramsetingDlg_row_add_distance/_p->m_mcs->e2proomdata.measurementDlg_deepimg_pisdis+0.5));
//                           if(_p->b_int_show_record_finish==true)
//                           {
//                             _p->b_int_show_record_finish=false;
//                             qRegisterMetaType< QString >("QString");
//                             emit Send_show_record("完成数据采集");
//                           }
                       }
                       if(_p->b_int_show_cvimage_inlab_finish==true)
                       {
                           _p->b_int_show_cvimage_inlab_finish=false;
                           _p->pclclass.cv_f32deepimg_to_show8deepimg(_p->m_mcs->resultdata.cv_deepimg,&_p->m_mcs->resultdata.cv_8deepimg_temp);
                           qRegisterMetaType< cv::Mat >("cv::Mat"); //传递自定义类型信号时要添加注册
                           emit Send_show_cvimage_inlab(_p->m_mcs->resultdata.cv_8deepimg_temp);
                       }
                       if(_p->u8_save_data==1)//保存结果
                       {
                           QString str=_p->save_imgdata_cvimage(_p->m_mcs->resultdata.cv_deepimg);
                           _p->u8_save_data=0;
//                           if(_p->b_int_show_record_finish==true)
//                           {
//                             _p->b_int_show_record_finish=false;
//                             qRegisterMetaType< QString >("QString");
//                             QString strname="数据保存在:";
//                             strname=strname+str;
//                             emit Send_show_record(strname);
                        }
                    }
                    break;

                    case 4:   //显示点云
                    {
                         if(_p->m_mcs->cam->sop_cam[0].b_updatacloud_finish==true)
                         {
                             if(_p->m_mcs->resultdata.b_deepimg_pushoneline==true)
                             {
                               if(_p->m_mcs->cam->sop_cam[0].b_cv_lineEn==true)
                               {
                                 _p->cv_line=((*_p->m_mcs->cam->sop_cam[0].cv_line).linepoint);
                                 if(_p->m_mcs->resultdata.b_firstpoint==false)
                                 {
                                    _p->m_mcs->resultdata.b_firstpoint=true;
                                    _p->m_mcs->resultdata.f_deepimg_y=0;
                                    _p->m_mcs->resultdata.timeinfo_st=((*_p->m_mcs->cam->sop_cam[0].cv_line).linehead);
                                 }
                                 else
                                 {
                                    int32_t sec2=((*_p->m_mcs->cam->sop_cam[0].cv_line).linehead).stamp.sec;
                                    int32_t sec1=_p->m_mcs->resultdata.timeinfo_st.stamp.sec;
                                    uint32_t nsec2=((*_p->m_mcs->cam->sop_cam[0].cv_line).linehead).stamp.nanosec;
                                    uint32_t nsec1=_p->m_mcs->resultdata.timeinfo_st.stamp.nanosec;
                                    double fsec2=(double)sec2+(double)nsec2/1000000000.0;
                                    double fsec1=(double)sec1+(double)nsec1/1000000000.0;
                                    _p->m_mcs->resultdata.f_deepimg_y=(fsec2-fsec1)*_p->m_mcs->e2proomdata.measurementDlg_deepimg_speed;
                                 }
                          //     _p->m_mcs->resultdata.f_deepimg_y=(_p->m_mcs->resultdata.deepimg_callbacknum_nownum-1)*COLS_PROPORTION;
                                 _p->pclclass.cvpoint3f_to_oneline_pclclould(_p->cv_line,_p->m_mcs->resultdata.f_deepimg_y,&_p->m_mcs->resultdata.ptr_pcl_lineclould);
                                 *(_p->m_mcs->resultdata.ptr_pcl_deepclould)=*(_p->m_mcs->resultdata.ptr_pcl_deepclould)+*(_p->m_mcs->resultdata.ptr_pcl_lineclould);
                               }
                             }
                             _p->m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                         }
                         if(_p->m_mcs->resultdata.b_deepimg_showclould_finish==true)
                         {   //采集完成,重新刷新下颜色
                             _p->m_mcs->resultdata.b_deepimg_showclould_finish=false;

                             _p->pclclass.updata_color_pclclould(&_p->m_mcs->resultdata.ptr_pcl_deepclould,&_p->m_mcs->resultdata.ptr_pcl_deepclould);
                             if(_p->b_int_show_record_finish==true)
                             {
                               _p->b_int_show_record_finish=false;
                               _p->finish_cloud = true;
                               qRegisterMetaType<pcl::PointCloud<pcl::PointXYZRGB>::Ptr>("pcl::PointCloud<pcl::PointXYZRGB>::Ptr"); //传递自定义类型信号时要添加注册
                               emit Send_show_pclclould_list(_p->m_mcs->resultdata.ptr_pcl_deepclould);
//                               qRegisterMetaType< QString >("QString");
//                               emit Send_show_record("完成数据采集");
                             }
                         }
                         if(_p->b_init_show_pclclould_list_finish==true)
                         {
                             _p->b_init_show_pclclould_list_finish=false;

                             qRegisterMetaType<pcl::PointCloud<pcl::PointXYZRGB>::Ptr>("pcl::PointCloud<pcl::PointXYZRGB>::Ptr"); //传递自定义类型信号时要添加注册
                             emit Send_show_pclclould_list(_p->m_mcs->resultdata.ptr_pcl_deepclould);
                         }
                         if(_p->u8_save_data==1)//保存结果
                         {
                             QString str=_p->save_pcldata_pclclould(_p->m_mcs->resultdata.ptr_pcl_deepclould);
                             _p->u8_save_data=0;
//                             if(_p->b_int_show_record_finish==true)
//                             {
//                               _p->b_int_show_record_finish=false;
//                               qRegisterMetaType< QString >("QString");
//                               QString strname="数据保存在:";
//                               strname=strname+str;
//                               emit Send_show_record(strname);
//                             }
                         }
                    }
                    break;
                    // 一键标定时用
                    case 5:
                    {

                    }
                    break;

                    default:
                    break;
                  }
             }
             sleep(0);
        }
        else
        {
            _p->stop_b_imgshow_thread=true;
            break;
        }
    }
}

// 自定义图像显示槽
void MainWindow::int_show_cvimage_inlab(cv::Mat cv_image)
{
    // 如果正在录制视频，则将图像写入视频文件
    if (videoWriter.isOpened())
    {
        videoWriter.write(cv_image);
    }

    QImage::Format format = QImage::Format_RGB888;
    switch (cv_image.type())
    {
    case CV_8UC1:
      format = QImage::Format_Indexed8;
      break;
    case CV_8UC3:
      format = QImage::Format_RGB888;
      break;
    case CV_8UC4:
      format = QImage::Format_ARGB32;
      break;
    }
    QImage img = QImage((const uchar*)cv_image.data, cv_image.cols, cv_image.rows,cv_image.cols * cv_image.channels(), format);
    img = img.scaled(ui->imgShow->width(), ui->imgShow->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//图片自适应lab大小
//    ui->imgShow->setPixmap(QPixmap::fromImage(img));
    ui->imgShow->showImage(img);
    b_int_show_cvimage_inlab_finish = true;
}

void MainWindow::showupdata_tabWidget()
{
    if(m_mcs->resultdata.link_param_state==true)
    {
        int real_readnum=1;
        u_int16_t rcvdata[ALS103_REG_TOTALNUM];
        real_readnum=modbus_read_registers(m_mcs->resultdata.ctx_param,ALS103_EXPOSURE_TIME_REG_ADD,ALS103_REG_TOTALNUM,rcvdata);
        if(real_readnum<0)
        {
            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "读取参数失败");
        }
        else
        {
            if(rcvdata[0]>65534)
            {
                m_mcs->cam->sop_cam[0].i32_exposure=65535;
            }
            else if(rcvdata[0]<20)
            {
                m_mcs->cam->sop_cam[0].i32_exposure=20;
            }
            else
            {
                m_mcs->cam->sop_cam[0].i32_exposure=rcvdata[0];
            }
            ui->exposureValue->setText(QString::number(m_mcs->cam->sop_cam[0].i32_exposure));
            /*******************/
            //这里添加其他设置参数显示
            /*******************/

            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "读取参数成功");
        }
    }
}

QString MainWindow::GetCurTime_M()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = " [" + current_date_time.toString("yyyy-MM-dd hh:mm:ss") + "] ";

    return current_date;
}



void MainWindow::img_windowshow(bool b_show, QLabel *lab_show)
{
    if(b_show==true)
    {
    #ifndef ONLY_TEST_CAMER
        if(m_mcs->resultdata.link_result_state==false)
        {
//            QString server_ip=ui->IpAddr->text();
            QString server_ip = ipAddress;
            QString server_port2="1502";
            m_mcs->resultdata.ctx_result = modbus_new_tcp(server_ip.toUtf8(), server_port2.toInt());
            if (modbus_connect(m_mcs->resultdata.ctx_result) == -1)
            {
                QString current_date = GetCurTime_M();
                ui->textBrowser->append(current_date + "控制端口连接失败");
                modbus_free(m_mcs->resultdata.ctx_result);
                return;
            }
            m_mcs->resultdata.link_result_state=true;

            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "控制端口连接成功");
            open_camer_modbus();
        }
        if(m_mcs->resultdata.link_param_state==false)
        {
//            QString server_ip=ui->IpAddr->text();
            QString server_ip = ipAddress;
            QString server_port1="1500";
            m_mcs->resultdata.ctx_param = modbus_new_tcp(server_ip.toUtf8(), server_port1.toInt());
            if (modbus_connect(m_mcs->resultdata.ctx_param) == -1)
            {
                QString current_date = GetCurTime_M();
                ui->textBrowser->append(current_date + "参数端口连接失败");
                modbus_free(m_mcs->resultdata.ctx_param);
                return;
            }
            m_mcs->resultdata.link_param_state=true;

            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "参数端口连接成功");
        }
        //设置task信息
        u_int16_t task=103;
        int rc=modbus_write_registers(m_mcs->resultdata.ctx_result,0x102,1,&task);
        if(rc!=1)
        {
            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "激光器任务模式设置失败");
        }
//        else
//        {
//            ui->record->append("激光器任务模式设置成功");
//        }
        showupdata_tabWidget();
        if(m_mcs->e2proomdata.measurementDlg_leaser_data_mod==0)
        {
          m_mcs->cam->sop_cam[0].node_mode=0;
        }
        else
        {
          m_mcs->cam->sop_cam[0].node_mode=1;
        }
   #endif
        m_mcs->cam->sop_cam[0].InitConnect(lab_show);
    }
    else
    {
        m_mcs->cam->sop_cam[0].DisConnect();
    #ifndef ONLY_TEST_CAMER
        if(m_mcs->resultdata.link_result_state==true)
        {
            close_camer_modbus();
            modbus_free(m_mcs->resultdata.ctx_result);
            m_mcs->resultdata.link_result_state=false;

            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "控制端口关闭");
        }
        if(m_mcs->resultdata.link_param_state==true)
        {
            modbus_close(m_mcs->resultdata.ctx_param);
            m_mcs->resultdata.link_param_state=false;

            QString current_date = GetCurTime_M();
            ui->textBrowser->append(current_date + "参数端口关闭");
        }
    #endif
    }
}

void MainWindow::initChart()
{
    series = new QScatterSeries();                              // 创建一个散点数据集对象
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);  // 设置绘制的散点的样式为圆
    series->setMarkerSize(1);                                   // 设置绘制的点的大小
    series->setColor(QColor(0,255,0));                          // 设置散点颜色
    series->setUseOpenGL(true);                                 // 设置使用openGL加速
//    connect(series, &QScatterSeries::hovered, ui->graphicsView, [=](const QPointF &point, bool state){
//        if (state)
//        {
//            std::cout << "oo" << std::endl;
//            ui->graphicsView->coordItem_->setText(QString("X = %1, Y = %2").arg(point.x()).arg(point.y()));
//        }
//    });

//    ui->graphicsView->setRenderHint(QPainter::Antialiasing);                              // 设置抗锯齿
    //    ui->graphicsView->chart()->setTitle("散点图标题");                                      // 设置图表标题
    ui->graphicsView->chart()->setDropShadowEnabled(false);                               // 设置不启用背景阴影效果
    //    QBrush brush(Qt::darkGray);                                                                // 创建蓝色画刷
    //    ui->graphicsView->chart()->setBackgroundBrush(brush);
    //    ui->graphicsView->chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);  // 在图例中显示点的形状样式
    ui->graphicsView->chart()->legend()->hide();
    ui->graphicsView->chart()->setTheme(QChart::ChartThemeDark);                  // 设置表的样式
//    ui->graphicsView->chart()->addSeries(series);       // 将创建的series添加经chart中
//    ui->graphicsView->chart()->createDefaultAxes();     // 新添加series后，调用这个函数根据添加的series自动生成对于类型的XY轴，会删除已有的轴再生成
//    ui->graphicsView->setRubberBand(QChartView::RectangleRubberBand); // 设置选择区域的模式
//    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);   // 平移
    ui->graphicsView->setInteractive(true); // 启用交互模式


    axisX = new QValueAxis();
    axisY = new QValueAxis();
    ui->graphicsView->chart()->addAxis(axisX, Qt::AlignBottom); // 将坐标轴添加到图表
    ui->graphicsView->chart()->addAxis(axisY, Qt::AlignLeft);

    axisX->setRange(0, 100);                                    // 设置初始坐标轴范围
    axisX->setTitleText("X_axis (mm)");
    axisY->setRange(-10, 10);
    axisY->setTitleText("Y_axis (mm)");
    ui->graphicsView->chart()->addSeries(series);
    series->attachAxis(axisX);                                  // 将系列关联到坐标轴
    series->attachAxis(axisY);

}

void MainWindow::init_show_pclclould_list(pcl::PointCloud<pcl::PointXYZRGB>::Ptr pclclould)
{
    if(pclclould->size()==0)
    {
        b_init_show_pclclould_list_finish = true;
        return;
    }

    if(finish_line && updateVTKShow)
    {
        finish_line = false;
        b_int_show_record_finish = true;

//        series->clear();
        ui->graphicsView->chart()->removeSeries(series);
        QVector<QPointF> MyPointf;      // 绘制散点所需要的数据
        pcl::PointXYZRGB minPt, maxPt;
        pcl::getMinMax3D(*pclclould, minPt, maxPt);
        for (std::size_t i = 0; i < pclclould->points.size(); ++i)
        {
            MyPointf << QPointF(pclclould->points[i].y - minPt.y, maxPt.z - pclclould->points[i].z);
//            series->append(pclclould->points[i].y, pclclould->points[i].z);
        }
        series->replace(MyPointf);
//        ui->graphicsView->chart()->addSeries(series);
//        ui->graphicsView->chart()->createDefaultAxes();
        // 动态调整坐标轴范围
        axisX->setRange(0, maxPt.y-minPt.y);
        axisY->setRange(0, maxPt.z-minPt.z);
        ui->graphicsView->chart()->addSeries(series);
    }
/*
    if(finish_line && updateVTKShow)
        {
            // 清空数据
            points->Reset();
            cells->Reset();
            polydata->Initialize();
            vtkIdType idtype;

            scalars->SetNumberOfValues(pclclould->size());
            for (std::size_t i = 0; i < pclclould->points.size(); ++i)
            {
                idtype = points->InsertNextPoint(pclclould->points[i].x,
                                                 pclclould->points[i].y,
                                                 pclclould->points[i].z);
                cells->InsertNextCell(1, &idtype);
                scalars->SetValue(i, static_cast<float>(pclclould->points[i].z) );

            }

            polydata->SetPoints(points);
            polydata->SetVerts(cells);
            polydata->GetPointData()->SetScalars(scalars);

            mapper->SetInputData(polydata);   // err
            mapper->ScalarVisibilityOn();
            //mapper->SetScalarModeToUsePointData();
            mapper->SetScalarRange(points->GetBounds()[4], points->GetBounds()[5]);
            mapper->SetColorModeToMapScalars();
            mapper->SetLookupTable(lut);

            actor->SetMapper(mapper);
            actor->GetProperty()->SetInterpolationToFlat();
            cubeAxesActor->SetBounds(points->GetBounds());

            if (camera_reset_once || camera_reset_always)
            {
                camera_reset_once = false;
                renderer->ResetCamera();
            }
            ui->pclShow->GetRenderWindow()->Render();   // err 3

            finish_line = false;
            b_int_show_record_finish = true;

        }
*/
    // 扫描完成的点云
    if(finish_cloud==true)
        {
            finish_cloud = false;
            b_int_show_record_finish = true;
            vtkIdType idtype;
            points = vtkSmartPointer<vtkPoints>::New();
            cells = vtkSmartPointer<vtkCellArray>::New();
            polydata = vtkSmartPointer<vtkPolyData>::New();

            scalars->SetNumberOfValues(pclclould->size());
            for (std::size_t i = 0; i < pclclould->points.size (); ++i)
            {
                idtype = points->InsertNextPoint(pclclould->points[i].x,
                                                 pclclould->points[i].y,
                                                 pclclould->points[i].z);
                cells->InsertNextCell(1, &idtype);
                scalars->SetValue(i, static_cast<float>(pclclould->points[i].z) );

            }

            polydata->SetPoints(points);
            polydata->SetVerts(cells);
            polydata->GetPointData()->SetScalars(scalars);

            mapper->SetInputData(polydata);
            mapper->ScalarVisibilityOn();
            //mapper->SetScalarModeToUsePointData();
            mapper->SetScalarRange(points->GetBounds()[4], points->GetBounds()[5]);
            mapper->SetColorModeToMapScalars();
            mapper->SetLookupTable(lut);

            actor->SetMapper(mapper);
            actor->GetProperty()->SetInterpolationToFlat();
            cubeAxesActor->SetBounds(points->GetBounds());

            renderer->ResetCamera();
            ui->pclShow->GetRenderWindow()->Render();
//            ui->pclShow->update();

        }

    b_init_show_pclclould_list_finish=true;
}

// VTK初始化
void MainWindow::vtk_init()
{

    slotConnector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    this->Connections = slotConnector;

    colors = vtkSmartPointer<vtkNamedColors>::New();
    cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
    points = vtkSmartPointer<vtkPoints>::New();
    cells = vtkSmartPointer<vtkCellArray>::New();
    polydata = vtkSmartPointer<vtkPolyData>::New();
    scalars = vtkSmartPointer<vtkFloatArray>::New();
    lut = vtkSmartPointer<vtkLookupTable>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor = vtkSmartPointer<vtkActor>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
    scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBarWidget = vtkSmartPointer<vtkScalarBarWidget>::New();

    style = vtkSmartPointer<InteractionStyle::MouseInteractorStylePP>::New();
    iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    axes_actor = vtkSmartPointer<vtkAxesActor>::New();
    axes_actorWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
//    textActorFPS = vtkSmartPointer<vtkTextActor>::New();

    ui->pclShow->SetRenderWindow(renderWindow); // 设置渲染窗口
    style->SetDefaultRenderer(renderer);        // 将 renderer 设置为默认的渲染器

    renderer->GradientBackgroundOn();       // 背景设置
    renderer->SetBackground(colors->GetColor3d("dark").GetData());
//    renderer->SetBackground2(colors->GetColor3d("Gray").GetData());
//    renderer->ResetCamera();

    ui->pclShow->GetRenderWindow()->AddRenderer(renderer);  // 添加renderer到渲染窗口
    //iren=ui->pclShow->GetInteractor();
    iren = ui->pclShow->GetRenderWindow()->GetInteractor(); // 交互
    iren->SetInteractorStyle(style);

    // 坐标轴及网格显示控制
    cubeAxesActor->SetScreenSize(10);
    cubeAxesActor->DrawZGridlinesOn();
    cubeAxesActor->DrawXGridlinesOn();
    cubeAxesActor->DrawYGridlinesOn();
    cubeAxesActor->SetDrawXInnerGridlines(false);
    cubeAxesActor->SetDrawYInnerGridlines(false);
    cubeAxesActor->SetDrawZInnerGridlines(false);
    cubeAxesActor->SetFlyMode(2);
    cubeAxesActor->SetGridLineLocation(2);
    cubeAxesActor->XAxisMinorTickVisibilityOff();
    cubeAxesActor->YAxisMinorTickVisibilityOff();
    cubeAxesActor->ZAxisMinorTickVisibilityOff();
    cubeAxesActor->SetCamera(renderer->GetActiveCamera());

    lut->SetHueRange(0.666, 0);
    lut->Build();
    scalarBar->SetTitle("Unit (mm)\n");
    scalarBar->SetNumberOfLabels(5);
    scalarBar->SetLookupTable(lut);
    scalarBar->GetLabelTextProperty()->SetFontSize(12);
//    vtkTextProperty* textProp = scalarBar->GetLabelTextProperty();  // 获取ColorBarActor的TextProperty
//    textProp->SetFontSize(12);                                      // 更改TextProperty的字体大小
    scalarBar->SetWidth(0.08);                                      // 更改ColorBarActor的高度和宽度
    scalarBar->SetHeight(0.6);
    scalarBar->GetPositionCoordinate()->SetValue(0.9, 0.2);         // 设置ColorBarActor的位置
    scalarBar->SetLabelFormat("%.3f");

    renderer->AddActor(actor);
    renderer->AddActor(cubeAxesActor);
    renderer->AddActor2D(scalarBar);
//    renderer->AddActor(textActorFPS);
    //

    vtkCamera* camera = renderer->GetActiveCamera();
    camera->SetPosition(1, 0, 0);
    camera->SetViewUp (0, 0, -1);
    camera->SetFocalPoint (0, 0, 0);
    renderer->ResetCamera();

    axes_actor->SetPosition(0, 0, 0);
    axes_actor->SetTotalLength(2, 2, 2);
    axes_actor->SetShaftType(0);
    axes_actor->SetCylinderRadius(0.02);
    axes_actorWidget->SetOrientationMarker(axes_actor);
    axes_actorWidget->SetInteractor(iren);
    axes_actorWidget->On();
    axes_actorWidget->InteractiveOn();

    ui->pclShow->update();
}

// 窗口初始化参数设置，IP地址、曝光值、显示深度页面参数、调试窗口
void MainWindow::InitSetEdit()
{

//    ui->IpAddr->setText("192.168.1.2");
    ipAddress = "192.168.1.2";
    ui->exposureValue->setText(QString::number(m_mcs->cam->sop_cam[0].i32_exposure));

    ui->sampleDis->setText(QString::number(m_mcs->e2proomdata.measurementDlg_deepimg_distance));
    ui->sampleVel->setText(QString::number(m_mcs->e2proomdata.measurementDlg_deepimg_speed));

    ui->stackedWidget->setCurrentIndex(0);

    // 按钮可用性初始化
    ui->applyBtn->setEnabled(false);
    ui->captureDepthBtn->setEnabled(false);
    ui->toolBar_right->setEnabled(false);
    ui->showCam->setEnabled(false);
    ui->showCenter->setEnabled(false);
    ui->showTrajectory->setEnabled(false);
    ui->showPointCloud->setEnabled(false);
    ui->showDepth->setEnabled(false);
    ui->saveFile->setEnabled(false);
    ui->setParam->setEnabled(false);
    ui->calibration->setEnabled(false);
    ui->page_3->hide();

    QMainWindow::setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    ui->action_restart->setVisible(false);
    ui->actionactionCemaraView->setVisible(false);
}

// modbus
void MainWindow::open_camer_modbus()
{
    if(m_mcs->resultdata.link_result_state==true)
    {
        uint16_t tab_reg[1];
        tab_reg[0]=0xff;
        modbus_write_registers(m_mcs->resultdata.ctx_result,0x101,1,tab_reg);

    }
}

void MainWindow::close_camer_modbus()
{
    if(m_mcs->resultdata.link_result_state==true)
    {
        uint16_t tab_reg[1];
        tab_reg[0]=0;
        modbus_write_registers(m_mcs->resultdata.ctx_result,0x101,1,tab_reg);

    }
}

void MainWindow::UpdateUi()
{
    ui->stackedWidget->setCurrentIndex(0);
    // 连接、断开按钮的控制，应用、一键采集按钮控制
    if(m_mcs->cam->sop_cam[0].b_connect==false)
    {
        ui->connectCam->setText("连接相机");
        ui->applyBtn->setEnabled(false);
        ui->captureDepthBtn->setEnabled(false);
        ui->toolBar_right->setEnabled(false);
        ui->showCam->setEnabled(false);
        ui->showCenter->setEnabled(false);
        ui->showTrajectory->setEnabled(false);
        ui->showPointCloud->setEnabled(false);
        ui->showDepth->setEnabled(false);
        ui->saveFile->setEnabled(false);
        ui->setParam->setEnabled(false);
        ui->calibration->setEnabled(false);
        ui->imgShow->clear();
    }
    else
    {
        ui->connectCam->setText("断开相机");
        ui->applyBtn->setEnabled(true);
        ui->captureDepthBtn->setEnabled(true);
        ui->toolBar_right->setEnabled(true);
        ui->showCam->setEnabled(true);
        ui->showCenter->setEnabled(true);
        ui->showTrajectory->setEnabled(true);
        ui->showPointCloud->setEnabled(true);
        ui->showDepth->setEnabled(true);
        ui->saveFile->setEnabled(true);
        ui->setParam->setEnabled(true);
        ui->calibration->setEnabled(true);


    }
}

QString MainWindow::save_imgdata_cvimage(cv::Mat cv_image)
{
    QString dir="./USER_DATA/";
    QString time;
    GetCurTime to;
    to.get_time_ms(&time);
    QString format;
    switch(cv_image.type())
    {
    case CV_8UC1:
        format=".bmp";
    break;
    case CV_8UC3:
        format=".bmp";
    break;
    case CV_32FC1:
        format=".tiff";
    break;
    case CV_32FC3:
        format=".tiff";
    break;
    case CV_64FC1:
        format=".tiff";
    break;
    case CV_64FC3:
        format=".tiff";
    break;
    }
    dir=dir+time+format;
    cv::imwrite(dir.toStdString(), cv_image);
    return dir;
}

QString MainWindow::save_pcldata_pclclould(pcl::PointCloud<pcl::PointXYZRGB>::Ptr pclclould)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr saveclould(new pcl::PointCloud<pcl::PointXYZ>);
    QString dir="./USER_DATA/";
    QString time;
    GetCurTime to;
    to.get_time_ms(&time);
    QString format=".pcd";
    dir=dir+time+format;
    pcl::copyPointCloud(*pclclould,*saveclould);//点云转换
    if (saveclould->size() > 0)
    {
        pcl::io::savePCDFile(dir.toStdString(), *saveclould);
    }
    return dir;
}

void MainWindow::start_deepimg()
{
    float callback_timer;//定时器间隔
    m_mcs->e2proomdata.measurementDlg_deepimg_distance = ui->sampleDis->text().toFloat();
    m_mcs->e2proomdata.measurementDlg_deepimg_speed = ui->sampleVel->text().toFloat();

//    m_mcs->e2proomdata.measurementDlg_deepimg_pisdis=ui->deepimg_Edit_3->text().toFloat();
    float usetime = m_mcs->e2proomdata.measurementDlg_deepimg_distance/m_mcs->e2proomdata.measurementDlg_deepimg_speed;//获得采集时间(秒)
    m_mcs->e2proomdata.write_measurementDlg_para();

    callback_timer=usetime;//获得采集间隔(秒/次)
    m_mcs->resultdata.deepimg_timer=callback_timer*1000.0+0.5;
    m_mcs->resultdata.b_deepimg_working=true;
    m_mcs->resultdata.b_deepimg_showclould_finish=false;
    m_mcs->resultdata.f_deepimg_y=0;
    m_mcs->resultdata.b_firstpoint=false;
    m_mcs->resultdata.ptr_pcl_deepclould->clear();
//  m_mcs->resultdata.cv_deepimg=cv::Mat::zeros(m_mcs->resultdata.deepimg_callbacknum,m_mcs->resultdata.cv_deepimg.cols,CV_32FC1);
    timer_tragetor_clould->start(m_mcs->resultdata.deepimg_timer);
    m_mcs->resultdata.b_deepimg_pushoneline=true;

    ui->captureDepthBtn->setText("手动停止采集");
}

void MainWindow::stop_deepimg()
{
    timer_tragetor_clould->stop();
    m_mcs->resultdata.b_deepimg_working=false;
    m_mcs->resultdata.b_deepimg_showclould_finish=true;
    m_mcs->resultdata.b_deepimg_pushoneline=false;
    ui->captureDepthBtn->setText("一键采集");
}

// 自定义定时器槽函数，一键采集按钮控制
void MainWindow::slot_timer_tragetor_clould()
{
    timer_tragetor_clould->stop();
    m_mcs->resultdata.b_deepimg_working=false;
    m_mcs->resultdata.b_deepimg_showclould_finish=true;
    m_mcs->resultdata.b_deepimg_pushoneline=false;
    ui->captureDepthBtn->setText("一键采集");

    QString current_date = GetCurTime_M();
    ui->textBrowser->append(current_date + "数据采集完成");
}

// 控制距离测量按钮是否执行
void MainWindow::doDisMeasure(bool checked)
{
   if (checked)
   {
       updateVTKShow = false;
       style->recordPoint = true;
       style->clearDis = false;
   }
   else
   {
       updateVTKShow = true;
       style->recordPoint = false;
       style->clearDis = true;
       style->twoPoints.clear();
   }
}

void MainWindow::doDockerRestart()
{
    ssh_session session;
    int rc;
    // 初始化SSH会话
    session = ssh_new();
    if (session == NULL) {
        std::cout << "Failed to create SSH session" << std::endl;
        return ;
    }
    // 设置SSH连接选项
    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.1.2");
    ssh_options_set(session, SSH_OPTIONS_USER, "pi");
    // 连接到下位机
    rc = ssh_connect(session);
    if (rc != SSH_OK) {
        std::cout << "Failed to connect to the host" << std::endl;
        ssh_free(session);
        return ;
    }
    // 认证
    rc = ssh_userauth_password(session, NULL, "123456");
    if (rc != SSH_AUTH_SUCCESS) {
        std::cout << "Failed to authenticate" << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        return ;
    }
    // 执行Docker命令来重启指定的容器
    ssh_channel channel;
    channel = ssh_channel_new(session);
    if (channel == NULL) {
        std::cout << "Failed to create SSH channel" << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        return ;
    }
    // 开启会话通道
    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        std::cout << "Failed to open SSH channel" << std::endl;
        ssh_channel_free(channel);
        ssh_disconnect(session);
        ssh_free(session);
        return ;
    }
    // 执行重启容器命令（无法执行需要sudo权限）
    std::string command = "echo \"123456\" | sudo -S docker-compose restart";
//    std::cout << command.c_str() << std::endl;
    rc = ssh_channel_request_exec(channel, command.c_str());
    if (rc != SSH_OK) {
        std::cout << "Failed to execute command: " << command << std::endl;
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        ssh_disconnect(session);
        ssh_free(session);
        return ;
    }
    // 关闭连接和会话
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(session);
    ssh_free(session);
}

void MainWindow::recordAsVideo()
{
    if (!recordVideo)
    {
        recordVideo = true;
        ui->recordAsVideo->setText("停止录制");

        QString dir="./USER_DATA/";
        QString time;
        GetCurTime to;
        to.get_time_ms(&time);
        dir = dir + time + ".avi";
        videoWriter.open(dir.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(1536, 1024));

    }
    else
    {
        recordVideo = false;
        ui->recordAsVideo->setText("开始录制");
        videoWriter.release();
    }

}
