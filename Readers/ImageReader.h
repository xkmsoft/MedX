//
// Created by chasank on 05/10/2021.
//

#ifndef MEDX_IMAGEREADER_H
#define MEDX_IMAGEREADER_H

#include <vtkImageData.h>

class ImageReader {
public:
    virtual vtkImageData* Read() = 0;
};


#endif //MEDX_IMAGEREADER_H
