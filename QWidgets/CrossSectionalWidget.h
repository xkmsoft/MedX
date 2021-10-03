//
// Created by chasank on 02/10/2021.
//

#ifndef MEDX_CROSSSECTIONALWIDGET_H
#define MEDX_CROSSSECTIONALWIDGET_H

#include <vtkImageData.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <Utils/LookupTableReader.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageResliceMapper.h>
#include <vtkLookupTable.h>
#include <vtkInteractorStyleImage.h>

enum Plane {
    SAGITTAL = 1,
    CORONAL = 2,
    AXIAL = 3,
};

class CrossSectionalWidget: public QVTKOpenGLNativeWidget {
    Q_OBJECT
public:
    explicit CrossSectionalWidget(int plane, QWidget *parent= nullptr);
    void updatePipeline(vtkImageData *image);
    ~CrossSectionalWidget() override;
    static double X_VIEW_RIGHT_VECTOR [3];
    static double X_VIEW_UP_VECTOR [3];
    static double Y_VIEW_RIGHT_VECTOR [3];
    static double Y_VIEW_UP_VECTOR [3];
    static double Z_VIEW_RIGHT_VECTOR [3];
    static double Z_VIEW_UP_VECTOR [3];
private:
    int plane;
    void releaseResources();
    bool rendered;
    vtkRenderer *renderer;
    vtkGenericOpenGLRenderWindow *renderWindow;
    vtkImageResliceMapper *resliceMapper;
    vtkLookupTable *lookupTable;
    vtkImageProperty *imageProperty;
    vtkImageSlice *imageSlice;
    vtkInteractorStyleImage *imageStyle;
};
#endif //MEDX_CROSSSECTIONALWIDGET_H
