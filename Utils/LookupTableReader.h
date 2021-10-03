//
// Created by chasank on 02/10/2021.
//

#ifndef MEDX_LOOKUPTABLEREADER_H
#define MEDX_LOOKUPTABLEREADER_H

#include <vtkLookupTable.h>
#include <string>
#include <map>

using namespace std;

enum LUTNames {
    HOT1 = 0,
    WINTER2 = 1,
    WARM3 = 2,
    COOL4 = 3,
    RED_YELL5 = 4,
    BLUE_GREEN6 = 5,
    ACTC = 6,
    BLACKBDY = 7,
    BLUE = 8,
    BLUEGRAY = 9,
    BONE = 10,
    CARDIAC = 11,
    CORTEX = 12,
    FLOW = 13,
    FRENCH = 14,
    GE_COLOR = 15,
    GOLD = 16,
    GOOCH = 17,
    GRAYSCALE = 18,
    GRAYSCALE_REVERSE = 19,
    GREEN = 20,
    GREENGRAY = 21,
    HOTIRON = 22,
    NIH = 23,
    NIH_FIRE = 24,
    NIH_ICE = 25,
    PINK = 26,
    RED = 27,
    RED_REVERSE = 28,
    RED_GRAY = 29,
    ROYAL = 30,
    SPECTRUM = 31,
    SURFACE = 32,
    X_HOT = 33,
    X_RAIN = 34
};

class LookupTableReader {
public:
    explicit LookupTableReader(const std::string &fileName);
    unsigned char* GetRed() const;
    unsigned char* GetGreen() const;
    unsigned char* GetBlue() const;
    static void buildLookUpTable(vtkLookupTable *table, int lut);
    static std::string lutNames[];
private:
    static std::string getFileName(int lut);
    unsigned char red[256]{};
    unsigned char green[256]{};
    unsigned char blue[256]{};
};


#endif //MEDX_LOOKUPTABLEREADER_H
