#include "showimgpcddlg.h"
#include "ui_showimgpcddlg.h"


showImgPcdDlg::showImgPcdDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showImgPcdDlg)
{
    ui->setupUi(this);
    // 设置窗体最大化和最小化
    this->setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);

    imgLabel = new LabelImageViewer;
    indexLabel = ui->stackedWidget->addWidget(imgLabel);
//    std::cout << indexLabel << std::endl;

    vtk_init();
//    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
//    ui->widgetPcd->SetRenderWindow(renderWindow);

//    renderer = vtkSmartPointer<vtkRenderer>::New();
//    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
//    renderer->GradientBackgroundOn();   // 用于启用渐变背景
//    renderer->SetBackground2(colors->GetColor3d("Black").GetData());
//    renderer->SetBackground(colors->GetColor3d("Gray").GetData()); // Black, DarkSlateBlue，LightSkyBlue，Gray
//    renderer->ResetCamera();
//    ui->widgetPcd->GetRenderWindow()->AddRenderer(renderer);

    pclclould.reset(new pcl::PointCloud<pcl::PointXYZ>);

}

showImgPcdDlg::~showImgPcdDlg()
{

//    renderer->Delete();
//    renderWindow->Delete();
//    this->Connections->Disconnect();
    delete imgLabel;
    delete ui;

}


void showImgPcdDlg::showpoint(std::string filename)
{

    QString msg=filename.c_str();

    QStringList msgList = msg.split(".");
    if(msgList[msgList.size()-1]=="BMP"||msgList[msgList.size()-1]=="bmp")
    {
//        ui->stackedWidget->setCurrentIndex(0);
        ui->stackedWidget->setCurrentIndex(indexLabel);
        cv::Mat m_srcImage = cv::imread(filename);
        if (m_srcImage.empty()==0)
        {
            cvtColor(m_srcImage, m_srcImage, cv::COLOR_BGR2RGB);    // BGR转化为RGB
            QImage::Format format = QImage::Format_RGB888;
            switch (m_srcImage.type())
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
            QImage img = QImage((const uchar*)m_srcImage.data, m_srcImage.cols, m_srcImage.rows,
            m_srcImage.cols * m_srcImage.channels(), format);
//            img = img.scaled(ui->labelImg->width(), ui->labelImg->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);// 图片自适应lab大小
//            ui->labelImg->setPixmap(QPixmap::fromImage(img));
//            ui->labelImg->setScaledContents(true);
            img = img.scaled(imgLabel->width(), imgLabel->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            imgLabel->showImage(img);
        }
    }
    else if(msgList[msgList.size()-1]=="TIFF"||msgList[msgList.size()-1]=="tiff")
    {
//        ui->stackedWidget->setCurrentIndex(0);
        ui->stackedWidget->setCurrentIndex(indexLabel);
        cv::Mat m_fsrcImage = cv::imread(filename,cv::IMREAD_UNCHANGED);
        cv::Mat m_srcImage;
        if(m_fsrcImage.type()!=CV_32FC1)
            return;
        pclclass.cv_f32deepimg_to_show8deepimg(m_fsrcImage,&m_srcImage);
        if (m_srcImage.empty()==0)
        {
            cvtColor(m_srcImage, m_srcImage, cv::COLOR_BGR2RGB);    // BGR转化为RGB
            QImage::Format format = QImage::Format_RGB888;
            switch (m_srcImage.type())
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
            QImage img = QImage((const uchar*)m_srcImage.data, m_srcImage.cols, m_srcImage.rows,
            m_srcImage.cols * m_srcImage.channels(), format);
//            img = img.scaled(ui->labelImg->width(), ui->labelImg->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);// 图片自适应lab大小
//            ui->labelImg->setPixmap(QPixmap::fromImage(img));
//            ui->labelImg->setScaledContents(true);
            imgLabel->showImage(img);
        }
    }
    else if(msgList[msgList.size()-1]=="PCD"||
            msgList[msgList.size()-1]=="pcd")
    {
        // 显示打开的点云
        ui->stackedWidget->setCurrentIndex(1);
        pcl::io::loadPCDFile<pcl::PointXYZ>(filename, *pclclould);
//        vtk_init();
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

//            actor->Modified();
//            cubeAxesActor->Modified();
//            scalarBar->Modified();
        renderer->ResetCamera();
        ui->widgetPcd->GetRenderWindow()->Render();
        ui->widgetPcd->update();
/*
        //  vtk_init();

        vtkSmartPointer<vtkEventQtSlotConnect> slotConnector = vtkSmartPointer<vtkEventQtSlotConnect>::New();   // 管理信号与槽
        this->Connections = slotConnector;
        vtkSmartPointer<vtkCubeAxesActor> cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();     // 坐标轴
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();                          // 点集
        vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();                     // 空间单元
        vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();                    // 多边形网格数据，由点和单元组成
        vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();                   // 颜色映射表
        vtkSmartPointer<vtkPolyDataMapper> mapper=vtkSmartPointer<vtkPolyDataMapper>::New();            // 数据渲染
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();                             // 用于显示几何图形
        vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();       // 渲染colorbar
//        vtkSmartPointer<vtkScalarBarWidget> scalarBarWidget=vtkSmartPointer<vtkScalarBarWidget>::New();   // 交互模式调整colorbar属性
        vtkSmartPointer<MouseInteractorStylePP> style = vtkSmartPointer<MouseInteractorStylePP>::New();
        vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        vtkSmartPointer<vtkAxesActor> axes_actor = vtkSmartPointer<vtkAxesActor>::New();
        vtkSmartPointer<vtkOrientationMarkerWidget> axes_actorWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();

        ui->widgetPcd->update();
        ui->stackedWidget->setCurrentIndex(1);
        vtkActorCollection* actorCollection = renderer->GetActors();
        vtkActor2DCollection* actorCollection2D=renderer->GetActors2D();
        int num = actorCollection->GetNumberOfItems();
        int num1=actorCollection2D->GetNumberOfItems();
//        cout<<num<<endl;
//        cout<<num1<<endl;
        // 这个函数比较重要，否则getNextActor将没法得到正确的actor
        actorCollection->InitTraversal();
        actorCollection2D->InitTraversal();
        for (int i=0;i<num;++i)
        {
            vtkActor* actor = actorCollection->GetNextActor();
            renderer->RemoveActor(actor);
            //处理code
        }
        for (int i=0;i<num1;++i)
        {
            vtkActor2D* actor2D = actorCollection2D->GetNextActor2D();
            renderer->RemoveActor2D(actor2D);
            //处理code
        }
        pcl::io::loadPCDFile<pcl::PointXYZ>(filename, *pclclould);

        vtkIdType idtype;
        scalars->SetNumberOfValues(pclclould->size());
        for (std::size_t i = 0; i < pclclould->points.size (); ++i)
        {
            idtype = points->InsertNextPoint(pclclould->points[i].x,
                                             pclclould->points[i].y,
                                             pclclould->points[i].z);
            cells->InsertNextCell(1, &idtype);
            scalars->SetValue(i, static_cast<float>(pclclould->points[i].z) );
            // scalars->SetValue(i, static_cast<float>(cloud->points[i].z) / max_p.z);
            // i++;
        }
//        lut->SetHueRange(0.0, 0.3); // 设置颜色映射范围为红到绿
//        lut->SetHueRange(0.667, 0.0); // 设置颜色映射范围为红到蓝
//        lut->SetSaturationRange(1.0, 1.0); // 设置颜色映射的饱和度为100%
        lut->Build();
        polydata->SetPoints(points);
        polydata->SetVerts(cells);
        polydata->GetPointData()->SetScalars(scalars);

        mapper->SetInputData(polydata);
        mapper->ScalarVisibilityOn();
//        mapper->SetScalarModeToUsePointData();
        mapper->SetScalarRange(points->GetBounds()[4],points->GetBounds()[5]);
//        qDebug()<<(double)points->GetBounds()[4]<<points->GetBounds()[5];
        mapper->SetColorModeToMapScalars();
        mapper->SetLookupTable(lut);
        actor->SetMapper(mapper);
        actor->GetProperty()->SetInterpolationToFlat();
        cubeAxesActor->SetBounds(points->GetBounds());

        cubeAxesActor->SetScreenSize(10);

        cubeAxesActor->DrawZGridlinesOff();
        cubeAxesActor->DrawXGridlinesOn();
        cubeAxesActor->DrawYGridlinesOn();

        cubeAxesActor->SetDrawXInnerGridlines(false);
        cubeAxesActor->SetDrawYInnerGridlines(false);
        cubeAxesActor->SetDrawZInnerGridlines(false);

        cubeAxesActor->SetGridLineLocation(2);
        cubeAxesActor->XAxisMinorTickVisibilityOff();
        cubeAxesActor->YAxisMinorTickVisibilityOff();
        cubeAxesActor->ZAxisMinorTickVisibilityOff();
        cubeAxesActor->SetCamera(renderer->GetActiveCamera());

        scalarBar->SetLookupTable(lut);
        scalarBar->SetNumberOfLabels(5);

        vtkTextProperty* textProp = scalarBar->GetLabelTextProperty();  // 获取ColorBarActor的TextProperty
        textProp->SetFontSize(12);      // 更改TextProperty的字体大小
        scalarBar->SetWidth(0.08);      // 设置ColorBarActor宽度为0.1
        scalarBar->SetHeight(0.6);      // 设置ColorBarActor高度为0.8
        scalarBar->GetPositionCoordinate()->SetValue(0.9, 0.2); // 设置ColorBarActor的位置
        scalarBar->SetLabelFormat("%.3f");

//        cubeAxesActor->Modified();
//        renderer->AddActor(cylinderActor);
        renderer->AddActor(cubeAxesActor);
        renderer->AddActor(actor);
        renderer->AddActor2D(scalarBar);
        renderer->ResetCamera();
//        style->Data=polydata;
        ui->widgetPcd->GetRenderWindow()->Render();

       // ui->widgetPcd->GetRenderWindow()->Finalize();
        ui->widgetPcd->update();
*/
    }
}

void showImgPcdDlg::vtk_init()
{
    slotConnector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    this->Connections = slotConnector;

    colors = vtkSmartPointer<vtkNamedColors>::New();
    cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
//    points = vtkSmartPointer<vtkPoints>::New();
    cells = vtkSmartPointer<vtkCellArray>::New();
    polydata = vtkSmartPointer<vtkPolyData>::New();
    scalars = vtkSmartPointer<vtkFloatArray>::New();
    lut = vtkSmartPointer<vtkLookupTable>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor = vtkSmartPointer<vtkActor>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
    scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBarWidget = vtkSmartPointer<vtkScalarBarWidget>::New();

    style = vtkSmartPointer<MouseInteractorStylePP>::New();
    iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    axes_actor = vtkSmartPointer<vtkAxesActor>::New();
    axes_actorWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

    ui->widgetPcd->SetRenderWindow(renderWindow); // 设置渲染窗口
    style->SetDefaultRenderer(renderer);        // 将 renderer 设置为默认的渲染器

    renderer->GradientBackgroundOn();       // 背景设置
    renderer->SetBackground(colors->GetColor3d("black").GetData());
    renderer->SetBackground2(colors->GetColor3d("Gray").GetData());
//    renderer->ResetCamera();

    ui->widgetPcd->GetRenderWindow()->AddRenderer(renderer);  // 添加renderer到渲染窗口
    //iren=ui->pclShow->GetInteractor();
    iren = ui->widgetPcd->GetRenderWindow()->GetInteractor(); // 交互
    iren->SetInteractorStyle(style);

    //
    cubeAxesActor->SetScreenSize(10);
    cubeAxesActor->DrawZGridlinesOff();
    cubeAxesActor->DrawXGridlinesOn();
    cubeAxesActor->DrawYGridlinesOn();
    cubeAxesActor->SetDrawXInnerGridlines(false);
    cubeAxesActor->SetDrawYInnerGridlines(false);
    cubeAxesActor->SetDrawZInnerGridlines(false);
    cubeAxesActor->SetGridLineLocation(2);
    cubeAxesActor->XAxisMinorTickVisibilityOff();
    cubeAxesActor->YAxisMinorTickVisibilityOff();
    cubeAxesActor->ZAxisMinorTickVisibilityOff();
    cubeAxesActor->SetCamera(renderer->GetActiveCamera());

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

    renderer->AddActor(cubeAxesActor);
    renderer->AddActor(actor);
    renderer->AddActor2D(scalarBar);
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

    ui->widgetPcd->update();
}
