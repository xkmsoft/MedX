//
// Created by chasank on 05/10/2021.
//

#include <itkImageToVTKImageFilter.h>
#include "DICOMReader.h"
#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"

DICOMReader::DICOMReader(const string &path) {
    this->path = path;
}

DICOMReader::~DICOMReader() = default;

vtkImageData *DICOMReader::Read() {
    using PixelType = signed short;
    constexpr unsigned int Dimension = 3;
    using ImageType = itk::Image<PixelType, Dimension>;

    using NamesGeneratorType = itk::GDCMSeriesFileNames;
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

    nameGenerator->SetUseSeriesDetails(true);
    nameGenerator->AddSeriesRestriction("0008|0021");
    nameGenerator->SetGlobalWarningDisplay(false);
    nameGenerator->SetDirectory(this->path);
    try {
        using SeriesIdContainer = std::vector<std::string>;
        const SeriesIdContainer &seriesUID = nameGenerator->GetSeriesUIDs();
        auto seriesItr = seriesUID.begin();
        auto seriesEnd = seriesUID.end();
        if (seriesItr != seriesEnd) {
            std::cout << "The directory: " << this->path << " contains the following DICOM series: " << std::endl;
        } else {
            std::cout << "There are no DICOM series in the directory: " << this->path << std::endl;
            return nullptr;
        }

        while (seriesItr != seriesEnd) {
            std::cout << seriesItr->c_str() << std::endl;
            ++seriesItr;
        }

        seriesItr = seriesUID.begin();
        while (seriesItr != seriesUID.end()) {
            std::string seriesIdentifier = *seriesItr;
            seriesItr++;
            using FileNamesContainer = std::vector<std::string>;
            FileNamesContainer fileNames = nameGenerator->GetFileNames(seriesIdentifier);

            using ReaderType = itk::ImageSeriesReader<ImageType>;
            ReaderType::Pointer reader = ReaderType::New();
            using ImageIOType = itk::GDCMImageIO;
            ImageIOType::Pointer dicomIO = ImageIOType::New();
            dicomIO->SetMaxSizeLoadEntry(0xffff);
            reader->SetImageIO(dicomIO);
            reader->SetFileNames(fileNames);
            reader->ForceOrthogonalDirectionOff();
            reader->UpdateLargestPossibleRegion();

            // Converting ImageType to vtkImageData
            using FilterType = itk::ImageToVTKImageFilter<ImageType>;
            FilterType::Pointer filter = FilterType::New();
            filter->SetInput(reader->GetOutput());
            filter->Update();

            vtkImageData *result = vtkImageData::New();
            result->DeepCopy(filter->GetOutput());
            return result;
        }
    }
    catch (itk::ExceptionObject &ex) {
        std::cout << ex << std::endl;
        return nullptr;
    }
    return nullptr;
}

