//
// Created by chasank on 02/10/2021.
//

#ifndef MEDX_MEDICALIMAGEREADERTHREAD_H
#define MEDX_MEDICALIMAGEREADERTHREAD_H

#include <QThread>
#include <vtkImageData.h>
#include <vtkNIFTIImageReader.h>
#include <vtkDICOMImageReader.h>
#include <vtkNew.h>


enum ImageType {
    NIFTI = 1,
    DICOM = 2
};

class MedicalImageReaderThread : public QThread {
    Q_OBJECT
public:
    explicit MedicalImageReaderThread(QObject *parent= nullptr): QThread(parent) {}
    void begin(int type, const std::string &imagePath);
    signals:
    void readFinished(vtkImageData *image);
protected:
    void run() override;
private:
    std::string path;
    int imageType{};
};


#endif //MEDX_MEDICALIMAGEREADERTHREAD_H
