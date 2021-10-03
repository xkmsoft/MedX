//
// Created by chasank on 02/10/2021.
//

#include "CrossSectionalWidget.h"
#include <vtkLookupTable.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageProperty.h>
#include <QWheelEvent>
#include <Callbacks/ImageInteractionCallback.h>
#include "Utils/LookupTableReader.h"

double CrossSectionalWidget::X_VIEW_RIGHT_VECTOR[3] = { 0.0, 1.0, 0.0 };
double CrossSectionalWidget::X_VIEW_UP_VECTOR [3] = { 0.0, 0.0, 1.0 };
double CrossSectionalWidget::Y_VIEW_RIGHT_VECTOR [3] = { 1.0, 0.0, 0.0 };
double CrossSectionalWidget::Y_VIEW_UP_VECTOR [3] = { 0.0, 0.0, 1.0 };
double CrossSectionalWidget::Z_VIEW_RIGHT_VECTOR [3] = { 1.0, 0.0, 0.0 };
double CrossSectionalWidget::Z_VIEW_UP_VECTOR [3] = {0.0, 1.0, 0.0};


CrossSectionalWidget::CrossSectionalWidget(int plane, QWidget *parent): QVTKOpenGLNativeWidget(parent) {
    this->plane = plane;
    rendered = false;
    renderer = nullptr;
    renderWindow = nullptr;
    resliceMapper = nullptr;
    lookupTable = nullptr;
    imageProperty = nullptr;
    imageSlice = nullptr;
    imageStyle = nullptr;
}

void CrossSectionalWidget::releaseResources() {
    renderer->Delete();
    renderWindow->Delete();
    resliceMapper->GetInput()->Delete();
    lookupTable->Delete();
    imageProperty->Delete();
    imageSlice->Delete();
    imageStyle->Delete();
}

CrossSectionalWidget::~CrossSectionalWidget() {
    releaseResources();
}

void CrossSectionalWidget::updatePipeline(vtkImageData *image) {
    if (rendered) {
        releaseResources();
    }
    renderWindow = vtkGenericOpenGLRenderWindow::New();
    renderer = vtkRenderer::New();
    renderer->SetBackground(0.0, 0.0, 0.0);
    renderWindow->AddRenderer(renderer);
    setRenderWindow(renderWindow);

    double scalarRange[2];
    image->GetScalarRange(scalarRange);

    resliceMapper = vtkImageResliceMapper::New();
    resliceMapper->SetInputData(image);
    resliceMapper->SliceFacesCameraOn();
    resliceMapper->SliceAtFocalPointOn();
    resliceMapper->SetImageSampleFactor(2);
    resliceMapper->BorderOn();
    resliceMapper->Modified();

    lookupTable = vtkLookupTable::New();
    vtkLookupTable::GlobalWarningDisplayOff();
    lookupTable->SetNumberOfTableValues(256);
    lookupTable->SetTableRange(scalarRange[0], scalarRange[1]);
    lookupTable->SetRampToLinear();
    lookupTable->SetScaleToLinear();
    lookupTable->Build();
    LookupTableReader::buildLookUpTable(lookupTable, LUTNames::GRAYSCALE);

    imageProperty = vtkImageProperty::New();
    imageProperty->SetLookupTable(lookupTable);
    imageProperty->SetInterpolationTypeToLinear();
    imageProperty->SetAmbient(1.0);
    imageProperty->SetDiffuse(1.0);
    imageProperty->SetOpacity(1.0);
    imageProperty->Modified();

    imageSlice = vtkImageSlice::New();
    imageSlice->SetMapper(resliceMapper);
    imageSlice->SetProperty(imageProperty);
    imageSlice->Update();

    ImageInteractionCallback *callback = ImageInteractionCallback::New();
    imageStyle = vtkInteractorStyleImage::New();
    imageStyle->SetCurrentRenderer(renderer);
    imageStyle->HandleObserversOn();
    imageStyle->AutoAdjustCameraClippingRangeOn();
    imageStyle->AddObserver(vtkCommand::MouseWheelForwardEvent, callback);
    imageStyle->AddObserver(vtkCommand::MouseWheelBackwardEvent, callback);
    imageStyle->SetInteractionModeToImageSlicing();
    imageStyle->SetXViewRightVector(CrossSectionalWidget::X_VIEW_RIGHT_VECTOR);
    imageStyle->SetXViewUpVector(CrossSectionalWidget::X_VIEW_UP_VECTOR);
    imageStyle->SetYViewRightVector(CrossSectionalWidget::Y_VIEW_RIGHT_VECTOR);
    imageStyle->SetYViewUpVector(CrossSectionalWidget::Y_VIEW_UP_VECTOR);
    imageStyle->SetZViewRightVector(CrossSectionalWidget::Z_VIEW_RIGHT_VECTOR);
    imageStyle->SetZViewUpVector(CrossSectionalWidget::Z_VIEW_UP_VECTOR);
    if ( plane == Plane::SAGITTAL ) {
        imageStyle->SetImageOrientation(CrossSectionalWidget::X_VIEW_RIGHT_VECTOR, CrossSectionalWidget::X_VIEW_UP_VECTOR);
    }
    if ( plane == Plane::CORONAL ) {
        imageStyle->SetImageOrientation(CrossSectionalWidget::Y_VIEW_RIGHT_VECTOR, CrossSectionalWidget::Y_VIEW_UP_VECTOR);
    }
    if ( plane == Plane::AXIAL ){
        imageStyle->SetImageOrientation(CrossSectionalWidget::Z_VIEW_RIGHT_VECTOR, CrossSectionalWidget::Z_VIEW_UP_VECTOR);
    }
    renderWindow->GetInteractor()->SetInteractorStyle(imageStyle);

    renderer->AddViewProp(imageSlice);
    renderer->ResetCamera();

    renderWindow->Render();
    rendered = true;
}