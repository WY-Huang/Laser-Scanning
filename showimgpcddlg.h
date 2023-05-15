#ifndef SHOWIMGPCDDLG_H
#define SHOWIMGPCDDLG_H

#include <QDialog>
#include "opencv2/opencv.hpp"
#include "mypclfunction.h"
#include "label_image_viewer.h"

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

#include <vtkCellPicker.h>

namespace Ui {
class showImgPcdDlg;
}

namespace {

// Define interaction style
class MouseInteractorStylePP : public vtkInteractorStyleTrackballCamera
{
public:
    static MouseInteractorStylePP* New();
    vtkSmartPointer<vtkTextActor> textActor;
    vtkSmartPointer<vtkCellPicker> cellpicker;

    MouseInteractorStylePP()    // 构造函数
    {
        textActor = vtkSmartPointer<vtkTextActor>::New();
        cellpicker = vtkSmartPointer<vtkCellPicker>::New();
        cellpicker->SetTolerance(0.005);
//        this->Interactor->SetPicker(cellpicker);
    }

    vtkTypeMacro(MouseInteractorStylePP, vtkInteractorStyleTrackballCamera);

    virtual void OnRightButtonDown() override   // 重载鼠标右键事件
    {
//        std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0]
//                  << " " << this->Interactor->GetEventPosition()[1] << "\t";
        cellpicker->Pick(this->Interactor->GetEventPosition()[0],
                                            this->Interactor->GetEventPosition()[1],
                                            0, // always zero.
                                            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

        double picked[3];
        cellpicker->GetPickPosition(picked);
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(textActor);

//        std::cout << cellpicker->GetCellId() << std::endl;
        if(cellpicker->GetCellId() != -1)
        {
            vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
            std::string s = "Picked: ( "+std::to_string(picked[0])+" ,"+std::to_string(picked[1])+" ,"+std::to_string(picked[2])+" )";

            textActor->SetInput(s.c_str());
            textActor->SetPosition2(10, 40);
            textActor->GetTextProperty()->SetFontSize(24);
            textActor->GetTextProperty()->SetColor(colors->GetColor3d("Gold").GetData());
            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(textActor);

//            std::cout << "Value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
        }

        // Forward events
        vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }

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
    int indexLabel;     // 图像label窗口的索引

    // VTK显示点云
    vtkSmartPointer<vtkNamedColors> colors;
    vtkSmartPointer<vtkEventQtSlotConnect> slotConnector;
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkCellArray> cells;
    vtkSmartPointer<vtkPolyData> polydata;
    vtkSmartPointer<vtkFloatArray> scalars;
    vtkSmartPointer<vtkLookupTable> lut;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkScalarBarActor> scalarBar;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<vtkAxesActor> axes_actor;
    vtkSmartPointer<MouseInteractorStylePP> style;
    vtkSmartPointer<vtkCubeAxesActor> cubeAxesActor;
    vtkSmartPointer<vtkPropPicker> propPicker;
    vtkSmartPointer<vtkRenderWindowInteractor> iren;
    vtkSmartPointer<vtkOrientationMarkerWidget> axes_actorWidget;
    vtkSmartPointer<vtkScalarBarWidget> scalarBarWidget;
    vtkPropPicker*  Picker;          // Pointer to the picker
    void vtk_init();

private:
    Ui::showImgPcdDlg *ui;
    MyPclFunction pclclass;//点云算法

    LabelImageViewer * imgLabel;

    // vtk显示点云
    vtkSmartPointer<vtkEventQtSlotConnect> Connections;


};

#endif // SHOWIMGPCDDLG_H
