//
// Created by chasank on 02/10/2021.
//

#include <iostream>
#include "LookupTableReader.h"

using namespace std;

std::string LookupTableReader::lutNames[] = {
        "1hot",
        "2winter",
        "3warm",
        "4cool",
        "5redyell",
        "6bluegrn",
        "actc",
        "blackbdy",
        "blue",
        "bluegray",
        "bone",
        "cardiac",
        "cortex",
        "flow",
        "french",
        "GE_color",
        "gold",
        "gooch",
        "grayscale",
        "grayscale_reverse",
        "green",
        "greengray",
        "HOTIRON",
        "NIH",
        "NIH_fire",
        "NIH_ice",
        "pink",
        "Rainramp",
        "red",
        "redgray",
        "red_reverse",
        "royal",
        "spectrum",
        "surface",
        "x_hot",
        "x_rain",
};

LookupTableReader::LookupTableReader(const std::string &fileName) {
    std::ifstream file(fileName.c_str(), std::ios::in|std::ios::binary);
    if (file.is_open()) {
        while(!file.eof()) {
            file.read((char *) this->red, 256);
            file.read((char *) this->green, 256);
            file.read((char *) this->blue, 256);
        }
        file.close();
    } else {
        std::cerr << fileName << " could not be opened." << std::endl;
    }
}

unsigned char *LookupTableReader::GetRed() const {
    return const_cast< unsigned char* > ( this->red );
}

unsigned char *LookupTableReader::GetGreen() const {
    return const_cast< unsigned char* > ( this->green );
}

unsigned char *LookupTableReader::GetBlue() const {
    return const_cast< unsigned char* > ( this->blue );
}

void LookupTableReader::buildLookUpTable(vtkLookupTable *table, int lut) {
    const std::string fileName = LookupTableReader::getFileName(lut);
    auto reader = new LookupTableReader(fileName);
    unsigned char *r = reader->GetRed();
    unsigned char *g = reader->GetGreen();
    unsigned char *b = reader->GetBlue();
    for(int i = 0; i <= 255; i++) {
        if (i < 5) {
            table->SetTableValue(i, (double)r[i]/255.0, (double)g[i]/255.0, (double)b[i]/255.0, 0.0);
        } else if(i > 250) {
            table->SetTableValue(i, (double)r[i]/255.0, (double)g[i]/255.0, (double)b[i]/255.0, 1.0);
        } else {
            table->SetTableValue(i, (double)r[i]/255.0, (double)g[i]/255.0, (double)b[i]/255.0, (double) i/255.0);
        }
    }
    delete reader;
}

std::string LookupTableReader::getFileName(int lut) {
    return "../Luts/" + LookupTableReader::lutNames[lut];
}
