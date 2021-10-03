//
// Created by chasank on 03/10/2021.
//

#ifndef MEDX_IMAGEINTERACTIONCALLBACK_H
#define MEDX_IMAGEINTERACTIONCALLBACK_H

#include <vtkObject.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCommand.h>

class ImageInteractionCallback: public vtkCommand{
public:
    static ImageInteractionCallback *New() {
        return new ImageInteractionCallback;
    };
    void Execute(vtkObject *caller, unsigned long event, void *data) override;
private:
    static void Slice(vtkInteractorStyleImage *interactor, int dy);
};


#endif //MEDX_IMAGEINTERACTIONCALLBACK_H
