//
// Created by chasank on 03/10/2021.
//

#include "ImageInteractionCallback.h"
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

void ImageInteractionCallback::Execute(vtkObject *caller, unsigned long event, void *data) {
    auto *interactor = dynamic_cast<vtkInteractorStyleImage *>(caller);
    if (event == vtkCommand::MouseWheelForwardEvent) {
        ImageInteractionCallback::Slice(interactor, 2);
    }
    if (event == vtkCommand::MouseWheelBackwardEvent ) {
        ImageInteractionCallback::Slice(interactor, -2);
    }
}

void ImageInteractionCallback::Slice(vtkInteractorStyleImage *interactor, int dy) {
    if (interactor->GetCurrentRenderer() == nullptr) {
        return;
    }

    vtkCamera* camera = interactor->GetCurrentRenderer()->GetActiveCamera();
    double* range = camera->GetClippingRange();
    double distance = camera->GetDistance();

    // scale the interaction by the height of the viewport
    double viewportHeight = 0.0;
    if (camera->GetParallelProjection()) {
        viewportHeight = camera->GetParallelScale();
    }
    else {
        double angle = vtkMath::RadiansFromDegrees(camera->GetViewAngle());
        viewportHeight = 2.0 * distance * tan(0.5 * angle);
    }

    const int* size = interactor->GetCurrentRenderer()->GetSize();
    double delta = dy * viewportHeight / size[1];
    distance += delta;

    // clamp the distance to the clipping range
    if (distance < range[0]) {
        distance = range[0] + viewportHeight * 1e-3;
    }
    if (distance > range[1]) {
        distance = range[1] - viewportHeight * 1e-3;
    }
    camera->SetDistance(distance);

    interactor->GetCurrentRenderer()->GetRenderWindow()->Render();
}
