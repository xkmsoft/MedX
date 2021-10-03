//
// Created by chasank on 02/10/2021.
//
#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageData.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkRenderer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkLookupTable.h>
#include <vtkVolume.h>
#include <vtkBoxWidget.h>
#include <vtkPolyData.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include "VolumeRenderingWidget.h"
#include <Utils/LookupTableReader.h>

VolumeRenderingWidget::VolumeRenderingWidget(QWidget *parent) : QVTKOpenGLNativeWidget(parent) {
    rendered = false;
    renderer = nullptr;
    renderWindow = nullptr;
    volumeMapper = nullptr;
    volumeColor = nullptr;
    lookupTable = nullptr;
    volumeScalarOpacity = nullptr;
    volumeGradientOpacity = nullptr;
    volumeProperty = nullptr;
    volume = nullptr;
}

void VolumeRenderingWidget::updatePipeline(vtkImageData *data) {
    if (rendered) {
        releaseResources();
    }
    renderWindow = vtkGenericOpenGLRenderWindow::New();
    renderer = vtkRenderer::New();
    renderer->SetBackground(0.0, 0.0, 0.0);

    setRenderWindow(renderWindow);
    renderWindow->AddRenderer(renderer);

    volumeMapper = vtkSmartVolumeMapper::New();
    volumeMapper->SetInputData(data);
    volumeMapper->SetFinalColorLevel(0.5);
    volumeMapper->SetFinalColorWindow(1.0);
    volumeMapper->ReleaseDataFlagOn();
    volumeMapper->SetBlendModeToComposite();
    volumeMapper->SetRequestedRenderModeToRayCast();
    volumeMapper->Update();

    volumeColor = vtkColorTransferFunction::New();
    volumeColor->ClampingOn();
    volumeColor->SetScaleToLinear();

    double scalarRange[2];
    data->GetScalarRange(scalarRange);

    lookupTable = vtkLookupTable::New();
    lookupTable->SetNumberOfTableValues(256);
    lookupTable->SetTableRange(scalarRange[0], scalarRange[1]);
    lookupTable->SetRampToLinear();
    lookupTable->SetScaleToLinear();
    vtkLookupTable::GlobalWarningDisplayOff();
    lookupTable->Build();
    LookupTableReader::buildLookUpTable(lookupTable, LUTNames::GRAYSCALE);

    double *range = lookupTable->GetTableRange();
    auto factor = (double)((range[1] - range[0]) / 256.0);
    double midpoint = 0.5;
    double sharpness = 0.0;
    for(int i = 0; i <= 255; i++) {
        double *rgb = lookupTable->GetTableValue(i);
        auto point = (double)((factor * i) + range[0]);
        volumeColor->AddRGBPoint(point, rgb[0], rgb[1], rgb[2], midpoint, sharpness);
    }
    volumeColor->Build();

    volumeScalarOpacity = vtkPiecewiseFunction::New();
    for(int x = 0; x <= 255; x++) {
        auto point = (double)((factor * x) + range[0]);
        if (x < 5) {
            volumeScalarOpacity->AddPoint(point, 0.0, midpoint, sharpness);
        } else if (x > 250) {
            volumeScalarOpacity->AddPoint(point, 1.0, midpoint, sharpness);
        } else {
            volumeScalarOpacity->AddPoint(point, (double) x / 255.0, midpoint, sharpness);
        }
    }
    volumeScalarOpacity->Modified();

    volumeGradientOpacity = vtkPiecewiseFunction::New();
    volumeGradientOpacity->AddPoint(range[0], 0.2, midpoint, sharpness);
    volumeGradientOpacity->AddPoint(range[1]/2, 0.7, midpoint, sharpness);
    volumeGradientOpacity->AddPoint(range[1], 1.0, midpoint, sharpness);
    volumeGradientOpacity->Modified();

    volumeProperty = vtkVolumeProperty::New();
    volumeProperty->SetColor(volumeColor);
    volumeProperty->SetScalarOpacity(volumeScalarOpacity);
    volumeProperty->SetGradientOpacity(volumeGradientOpacity);
    volumeProperty->ShadeOff();
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->SetAmbient(1.0);
    volumeProperty->SetDiffuse(0.0);
    volumeProperty->SetSpecular(0.0);
    volumeProperty->SetSpecularPower(15.0);

    volume = vtkVolume::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    renderer->AddViewProp(volume);
    renderer->ResetCamera();

    renderWindow->Render();
    rendered = true;
}

VolumeRenderingWidget::~VolumeRenderingWidget() {
    releaseResources();
}

void VolumeRenderingWidget::releaseResources() {
    renderer->Delete();
    renderWindow->Delete();
    volumeMapper->GetInput()->Delete();
    volumeMapper->Delete();
    volumeColor->Delete();
    lookupTable->Delete();
    volumeScalarOpacity->Delete();
    volumeGradientOpacity->Delete();
    volumeProperty->Delete();
    volume->Delete();
}
