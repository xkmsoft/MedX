//
// Created by chasank on 05/10/2021.
//

#ifndef MEDX_DICOMREADER_H
#define MEDX_DICOMREADER_H

#include "Readers/ImageReader.h"

using namespace std;

class DICOMReader: public ImageReader {
public:
    explicit DICOMReader(const string &path);
    ~DICOMReader();
    vtkImageData* Read() override;
private:
    string path;
};


#endif //MEDX_DICOMREADER_H
