//
// Created by chasank on 02/10/2021.
//

#ifndef MEDX_VOLUMERENDERINGWIDGET_H
#define MEDX_VOLUMERENDERINGWIDGET_H

#include <QWidget>
#include <vtkImageData.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageData.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkLookupTable.h>
#include <vtkVolume.h>
#include <vtkProperty.h>

class VolumeRenderingWidget: public QVTKOpenGLNativeWidget {
    Q_OBJECT
public:
    explicit VolumeRenderingWidget(QWidget *parent= nullptr);
    void updatePipeline(vtkImageData *image);
    ~VolumeRenderingWidget() override;
private:
    void releaseResources();
    bool rendered;
    vtkRenderer *renderer;
    vtkGenericOpenGLRenderWindow *renderWindow;
    vtkSmartVolumeMapper *volumeMapper;
    vtkColorTransferFunction *volumeColor;
    vtkLookupTable *lookupTable;
    vtkPiecewiseFunction *volumeScalarOpacity;
    vtkPiecewiseFunction *volumeGradientOpacity;
    vtkVolumeProperty *volumeProperty;
    vtkVolume *volume;
};


#endif //MEDX_VOLUMERENDERINGWIDGET_H