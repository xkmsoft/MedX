//
// Created by chasank on 02/10/2021.
//

#include "MedicalImageReaderThread.h"

void MedicalImageReaderThread::begin(int type, const std::string &imagePath) {
    imageType = type;
    path = imagePath;
    start(QThread::HighestPriority);
}

void MedicalImageReaderThread::run() {
    if (imageType == ImageType::NIFTI) {
        vtkNew<vtkNIFTIImageReader> reader;
        reader->SetFileName(path.c_str());
        reader->Update();
        vtkImageData *image = reader->GetOutput();
        vtkImageData *result = image->NewInstance();
        result->ShallowCopy(image);
        emit readFinished(result);
    }
    if (imageType == ImageType::DICOM) {
        vtkNew<vtkDICOMImageReader> reader;
        reader->SetDirectoryName(path.c_str());
        reader->Update();
        vtkImageData *image = reader->GetOutput();
        vtkImageData *result = image->NewInstance();
        result->ShallowCopy(image);
        emit readFinished(result);
    }
}
