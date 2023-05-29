#include "showimgpcddlg.h"
#include "ui_showimgpcddlg.h"

namespace InteractionStyle
{
    MouseInteractorStylePP* MouseInteractorStylePP::New()
    {
        return new MouseInteractorStylePP;
    }

    MouseInteractorStylePP::MouseInteractorStylePP()    // 构造函数
    {
        textActor = vtkSmartPointer<vtkTextActor>::New();
        cellpicker = vtkSmartPointer<vtkCellPicker>::New();
        cellpicker->SetTolerance(0.01);
        //        this->Interactor->SetPicker(cellpicker);

        clearDis = false;
        recordPoint = false;
    }

    void MouseInteractorStylePP::twoPointsDisCal()
    {
        disXX = abs(twoPoints[0]-twoPoints[3]);
        disYY = abs(twoPoints[1]-twoPoints[4]);
        disZZ = abs(twoPoints[2]-twoPoints[5]);
        disXYZ= sqrt(pow(disXX, 2) + pow(disYY, 2) + pow(disZZ, 2));

    }

    void MouseInteractorStylePP::OnRightButtonDown()   // 重载鼠标右键事件
    {
        // 获取鼠标点击位置的屏幕坐标
        int* screenPos = this->Interactor->GetEventPosition();

        cellpicker->Pick(screenPos[0], screenPos[1], 0, // always zero.
                this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

        cellpicker->GetPickPosition(picked);
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(textActor);
        if (clearDis)
        {
            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(textActor2);
        }

        // std::cout << cellpicker->GetCellId() << std::endl;
        if(cellpicker->GetCellId() != -1)
        {
            textActor = vtkSmartPointer<vtkTextActor>::New();
            vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
            std::string s = "( "+std::to_string(picked[0])+", "+std::to_string(picked[1])+", "+std::to_string(picked[2])+" )";

            textActor->SetInput(s.c_str());
            textActor->SetPosition(screenPos[0], screenPos[1]);
            // textActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
            // textActor->GetPositionCoordinate()->SetValue(0.01, 0.96);
            textActor->GetTextProperty()->SetFontSize(18);
            textActor->GetTextProperty()->SetColor(colors->GetColor3d("Gold").GetData());
            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(textActor);

            // std::cout << "Value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;

            if (recordPoint)
            {
                if (twoPoints.size() >= 6)
                    twoPoints.clear();
                twoPoints.push_back(picked[0]);
                twoPoints.push_back(picked[1]);
                twoPoints.push_back(picked[2]);
                if (twoPoints.size() == 6)
                {
                    this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(textActor2);
//                    std::cout << "选点完成！" << std::endl;
                    twoPointsDisCal();
//                    std::cout << "两点距离为： " << distance << std::endl;
                    textActor2 = vtkSmartPointer<vtkTextActor>::New();
                    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
                    std::string strD = "Distance: "+std::to_string(disXYZ)+"\n"
                                       +"DisXX: "+std::to_string(disXX)+"\n"
                                       +"DisYY: "+std::to_string(disYY)+"\n"
                                       +"DisZZ: "+std::to_string(disZZ);

                    textActor2->SetInput(strD.c_str());
//                    textActor2->SetPosition(screenPos[0], screenPos[1]-10);
                    textActor2->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
                    textActor2->GetPositionCoordinate()->SetValue(0.02, 0.80);
                    textActor2->GetTextProperty()->SetFontSize(18);
                    textActor2->GetTextProperty()->SetColor(colors->GetColor3d("Red").GetData());
                    this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(textActor2);
                }
            }

        }

        // Forward events
        vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }
}

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
//        actor->GetProperty()->SetPointSize(1.5);
        cubeAxesActor->SetBounds(points->GetBounds());

        renderer->ResetCamera();
        ui->widgetPcd->GetRenderWindow()->Render();
        ui->widgetPcd->update();

    }
}

void showImgPcdDlg::vtk_init()
{
    slotConnector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    this->Connections = slotConnector;

    colors = vtkSmartPointer<vtkNamedColors>::New();
    cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
//    points = vtkSmartPointer<vtkPoints>::New();
//    cells = vtkSmartPointer<vtkCellArray>::New();
//    polydata = vtkSmartPointer<vtkPolyData>::New();
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
    cubeAxesActor->SetFlyMode(2);
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
