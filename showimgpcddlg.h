#ifndef SHOWIMGPCDDLG_H
#define SHOWIMGPCDDLG_H

#include <QDialog>
#include <opencv2/opencv.hpp>

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
#include <vtkPropPicker.h>
#include <vtkCylinderSource.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkHexahedron.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkCellPicker.h>
#include <vtkActor2DCollection.h>

#include <vector>

#include "mypclfunction.h"
#include "label_image_viewer.h"

//using namespace std;
namespace Ui {
class showImgPcdDlg;
}

namespace InteractionStyle {
// Define interaction style
class MouseInteractorStylePP : public vtkInteractorStyleTrackballCamera
{
public:
    static MouseInteractorStylePP* New();
    vtkSmartPointer<vtkTextActor> textActor;
    vtkSmartPointer<vtkTextActor> textActor2;
    vtkSmartPointer<vtkCellPicker> cellpicker;

    std::vector<double> twoPoints;
    double picked[3];
    bool clearDis;
    bool recordPoint;

    MouseInteractorStylePP();

    void twoPointsDisCal();

    vtkTypeMacro(InteractionStyle::MouseInteractorStylePP, vtkInteractorStyleTrackballCamera)

    virtual void OnRightButtonDown() override;

private:
    double disXX;
    double disYY;
    double disZZ;
    double disXY;
    double disXZ;
    double disYZ;
    double disXYZ;


};
//vtkStandardNewMacro(InteractionStyle::MouseInteractorStylePP)
}

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
    vtkSmartPointer<InteractionStyle::MouseInteractorStylePP> style;
    vtkSmartPointer<vtkCubeAxesActor> cubeAxesActor;
    vtkSmartPointer<vtkPropPicker> propPicker;
    vtkSmartPointer<vtkRenderWindowInteractor> iren;
    vtkSmartPointer<vtkOrientationMarkerWidget> axes_actorWidget;
    vtkSmartPointer<vtkScalarBarWidget> scalarBarWidget;
//    vtkPropPicker*  Picker;          // Pointer to the picker
    void vtk_init();

private:
    Ui::showImgPcdDlg *ui;
    MyPclFunction pclclass;//点云算法

    LabelImageViewer * imgLabel;

    // vtk显示点云
    vtkSmartPointer<vtkEventQtSlotConnect> Connections;


};

#endif // SHOWIMGPCDDLG_H
