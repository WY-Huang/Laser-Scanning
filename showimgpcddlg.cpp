#include "showimgpcddlg.h"
#include "ui_showimgpcddlg.h"
#include <vtkActor2DCollection.h>


showImgPcdDlg::showImgPcdDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showImgPcdDlg)
{
    ui->setupUi(this);
    // 设置窗体最大化和最小化
    this->setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);

    renderWindow=vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widgetPcd->SetRenderWindow(renderWindow);
   // style->SetDefaultRenderer(renderer);
    renderer=vtkSmartPointer<vtkRenderer>::New();
   // renderer->SetBackground2(colors->GetColor3d("NavajoWhite").GetData());
    vtkSmartPointer<vtkNamedColors>  colors=vtkSmartPointer<vtkNamedColors>::New();
    renderer->GradientBackgroundOn();
    renderer->SetBackground(colors->GetColor3d("Black").GetData()); // Black, DarkSlateBlue
    renderer->ResetCamera();
    ui->widgetPcd->GetRenderWindow()->AddRenderer(renderer);
  //  iren=ui->widgetPcd->GetInteractor();

    pclclould.reset(new pcl::PointCloud<pcl::PointXYZ>);

    //vtk_init();
}

showImgPcdDlg::~showImgPcdDlg()
{

    renderer->Delete();
    renderWindow->Delete();

    delete ui;

}


void showImgPcdDlg::showpoint(std::string filename)
{

    QString msg=filename.c_str();

    QStringList msgList = msg.split(".");
    if(msgList[msgList.size()-1]=="BMP"||
       msgList[msgList.size()-1]=="bmp")
    {
//        ui->widget->setVisible(false);
//        ui->label->setVisible(true);
        ui->stackedWidget->setCurrentIndex(0);
        cv::Mat m_srcImage = cv::imread(filename);
        if (m_srcImage.empty()==0)
        {
            cvtColor(m_srcImage, m_srcImage, cv::COLOR_BGR2RGB);//BGR转化为RGB
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
            img = img.scaled(ui->labelImg->width(), ui->labelImg->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//图片自适应lab大小
            ui->labelImg->setPixmap(QPixmap::fromImage(img));
            ui->labelImg->setScaledContents(true);
        }
    }
    else if(msgList[msgList.size()-1]=="TIFF"||
            msgList[msgList.size()-1]=="tiff")
    {
//        ui->widget->setVisible(false);
//        ui->label->setVisible(true);
        ui->stackedWidget->setCurrentIndex(0);
        cv::Mat m_fsrcImage = cv::imread(filename,cv::IMREAD_UNCHANGED);
        cv::Mat m_srcImage;
        if(m_fsrcImage.type()!=CV_32FC1)
            return;
        pclclass.cv_f32deepimg_to_show8deepimg(m_fsrcImage,&m_srcImage);
        if (m_srcImage.empty()==0)
        {
            cvtColor(m_srcImage, m_srcImage, cv::COLOR_BGR2RGB);//BGR转化为RGB
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
            img = img.scaled(ui->labelImg->width(), ui->labelImg->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//图片自适应lab大小
            ui->labelImg->setPixmap(QPixmap::fromImage(img));
            ui->labelImg->setScaledContents(true);
        }
    }
    else if(msgList[msgList.size()-1]=="PCD"||
            msgList[msgList.size()-1]=="pcd")
    {
      //  vtk_init();

        vtkSmartPointer<vtkEventQtSlotConnect> slotConnector=vtkSmartPointer<vtkEventQtSlotConnect>::New();
        this->Connections=slotConnector;
        vtkSmartPointer<vtkCubeAxesActor> cubeAxesActor=vtkSmartPointer<vtkCubeAxesActor>::New();
        vtkSmartPointer<vtkPoints> points=vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkCellArray> cells=vtkSmartPointer<vtkCellArray>::New();
        vtkSmartPointer<vtkPolyData> polydata=vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkFloatArray> scalars=vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkLookupTable> lut=vtkSmartPointer<vtkLookupTable>::New();
        vtkSmartPointer<vtkPolyDataMapper> mapper=vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> actor=vtkSmartPointer<vtkActor>::New();
    //    renderer=vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkScalarBarActor> scalarBar=vtkSmartPointer<vtkScalarBarActor>::New();
        vtkSmartPointer<vtkScalarBarWidget> scalarBarWidget=vtkSmartPointer<vtkScalarBarWidget>::New();
        //vtkSmartPointer<MouseInteractorStyle> style=vtkSmartPointer<MouseInteractorStyle>::New();
        vtkSmartPointer<vtkRenderWindowInteractor> iren=vtkSmartPointer<vtkRenderWindowInteractor>::New();
        vtkSmartPointer<vtkAxesActor> axes_actor=vtkSmartPointer<vtkAxesActor>::New();
        vtkSmartPointer<vtkOrientationMarkerWidget> axes_actorWidget=vtkSmartPointer<vtkOrientationMarkerWidget>::New();

        ui->widgetPcd->update();
        ui->stackedWidget->setCurrentIndex(1);
        vtkActorCollection* actorCollection = renderer->GetActors();
        vtkActor2DCollection* actorCollection2D=renderer->GetActors2D();
        int num = actorCollection->GetNumberOfItems();
        int num1=actorCollection2D->GetNumberOfItems();
//        cout<<num<<endl;
//        cout<<num1<<endl;
        //这个函数比较重要，否则getNextActor将没法得到正确的actor
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
            idtype = points->InsertNextPoint(pclclould->points[i].x
                                            , pclclould->points[i].y
                                            , pclclould->points[i].z);
            cells->InsertNextCell(1, &idtype);
            scalars->SetValue(i, static_cast<float>(pclclould->points[i].z) );
            //scalars->SetValue(i, static_cast<float>(cloud->points[i].z) / max_p.z);
          //  i++;
        }
        lut->Build();
        polydata->SetPoints(points);
        polydata->SetVerts(cells);
        polydata->GetPointData()->SetScalars(scalars);

        mapper->SetInputData(polydata);
        mapper->ScalarVisibilityOn();
        //mapper->SetScalarModeToUsePointData();
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
        vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
//        cylinder->SetResolution(8);

//        vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
//        vtkSmartPointer<vtkPolyDataMapper> cylinderMapper=vtkPolyDataMapper::New();
//        cylinderMapper->SetInputConnection(cylinder->GetOutputPort());


//        cylinderActor->SetMapper(cylinderMapper);
//        cylinderActor->GetProperty()->SetColor(1.0000, 0.3883, 0.2784);
//        cylinderActor->RotateX(30.0);
//        cylinderActor->RotateY(-45.0);
//        scalarBar->SetTitle("Distance");
        scalarBar->SetNumberOfLabels(5);
        scalarBar->SetLookupTable(lut);

        // 获取ColorBarActor的TextProperty
        vtkTextProperty* textProp = scalarBar->GetLabelTextProperty();
        // 更改TextProperty的字体大小
        textProp->SetFontSize(12);
        // 更改ColorBarActor的高度和宽度
        scalarBar->SetWidth(0.08);    // 设置宽度为0.1
        scalarBar->SetHeight(0.6);   // 设置高度为0.8
        // 设置ColorBarActor的位置
        scalarBar->GetPositionCoordinate()->SetValue(0.9, 0.2);

        //cubeAxesActor->Modified();
//        renderer->AddActor(cylinderActor);
        renderer->AddActor(cubeAxesActor);
        renderer->AddActor(actor);
        renderer->AddActor2D(scalarBar);
        renderer->ResetCamera();
        //renderer->
  //      style->Data=polydata;
        ui->widgetPcd->GetRenderWindow()->Render();

       // ui->widgetPcd->GetRenderWindow()->Finalize();
        ui->widgetPcd->update();

    }
}

void showImgPcdDlg::vtk_init()
{

}
