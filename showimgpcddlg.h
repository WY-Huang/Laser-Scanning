#ifndef SHOWIMGPCDDLG_H
#define SHOWIMGPCDDLG_H

#include <QDialog>
#include "opencv2/opencv.hpp"
#include "mypclfunction.h"

#define vtkRenderingCore_AUTOINIT 3(vtkRenderingOpenGL2, vtkInteractionStyle,vtkRenderingFreeType)

#include <vtkExtractSelection.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkCellPicker.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkScalarBarWidget.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkCubeAxesActor2D.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCubeAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCommand.h>
#include <vtkBorderWidget.h>
#include <vtkCamera.h>
#include <vtkBorderWidget.h>
#include <vtkCommand.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkVersion.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkAssembly.h>

#include <QVTKOpenGLNativeWidget.h>
#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/console/parse.h>
#include <pcl/range_image/range_image.h>
#include <pcl/visualization/common/float_image_utils.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/common/common.h>

#include <pcl/io/png_io.h>
#include <vtkColorSeries.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>
#include "vtkPropPicker.h"
#include "vtkCylinderSource.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkHexahedron.h"
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

namespace Ui {
class showImgPcdDlg;
}

namespace {

// Define interaction style
class MouseInteractorStylePP : public vtkInteractorStyleTrackballCamera
{
public:
  static MouseInteractorStylePP* New();
  MouseInteractorStylePP(){
    textActor=vtkSmartPointer<vtkTextActor>::New();
  }
  vtkTypeMacro(MouseInteractorStylePP, vtkInteractorStyleTrackballCamera);
  virtual void OnLeftButtonDown() override
  {
    std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0]
              << " " << this->Interactor->GetEventPosition()[1] << std::endl;
    this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],
                                        this->Interactor->GetEventPosition()[1],
                                        0, // always zero.
                                        this->Interactor->GetRenderWindow()
                                            ->GetRenderers()
                                            ->GetFirstRenderer());
    double picked[3];
    this->Interactor->GetPicker()->GetPickPosition(picked);
    this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(textActor);
   // vtkNew<vtkTextActor> textActor;
    vtkNew<vtkNamedColors> colors;
    std::string s="( "+std::to_string(picked[0])+" ,"+std::to_string(picked[1])+" ,"+std::to_string(picked[2])+" )";


    textActor->SetInput(s.c_str());
    textActor->SetPosition2(10, 40);
    textActor->GetTextProperty()->SetFontSize(24);
    textActor->GetTextProperty()->SetColor(colors->GetColor3d("Gold").GetData());
    this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(textActor);

      //renderer->AddActor2D(textActor);
    std::cout << "Picked value: " << picked[0] << " " << picked[1] << " "
              << picked[2] << std::endl;
   // emit Show_data(picked);

    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
  }
  vtkSmartPointer<vtkTextActor> textActor;
};

vtkStandardNewMacro(MouseInteractorStylePP);

} // namespace


class showImgPcdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit showImgPcdDlg(QWidget *parent = nullptr);
    ~showImgPcdDlg();

    pcl::PointCloud<pcl::PointXYZ>::Ptr pclclould;
    void showpoint(std::string filename);      //显示图像及点云

    // VTK显示点云
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    void vtk_init();

private:
    Ui::showImgPcdDlg *ui;
    MyPclFunction pclclass;//点云算法

    // vtk显示点云
    vtkSmartPointer<vtkEventQtSlotConnect> Connections;


};

#endif // SHOWIMGPCDDLG_H
