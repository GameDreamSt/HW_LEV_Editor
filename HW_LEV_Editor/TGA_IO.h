#pragma once

#include <iostream>
#include <string>
#include <vector>

//https://en.wikipedia.org/wiki/Truevision_TGA

using namespace std;

enum TGAImageType
{
    NoData,
    UncompressedColorMap,
    UncompressedTrueColor,
    UncompressedGrayscale,
    UNKNOWN4, UNKNOWN5, UNKNOWN6, UNKNOWN7, UNKNOWN8,
    RunLengthColorMap,
    RunLengthTrueColor,
    RunLengthGrayscale
};

struct TGAParams
{
public:
    string path;
    unsigned int width;
    unsigned int height;
    TGAImageType imageType;
    vector<unsigned char> *data;

    TGAParams()
    {
        path = "";
        height = width = 0;
        imageType = TGAImageType::NoData;
        data = nullptr;
    }
};

class TGA_IO
{
public:
    static bool ReadTGA(TGAParams &params);
	static bool WriteTGA(TGAParams &params);
};