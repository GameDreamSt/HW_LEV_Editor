#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ColorMapSpec
{
public:
    short firstEntryIndex;
    short colorMapLength;
    unsigned char colorMapEntrySize;

    ColorMapSpec()
    {
        firstEntryIndex = colorMapLength = 0;
        colorMapEntrySize = 0;
    }

    ColorMapSpec(short firstEntryIndex, short colorMapLength, unsigned char colorMapEntrySize)
    {
        this->firstEntryIndex = firstEntryIndex;
        this->colorMapLength = colorMapLength;
        this->colorMapEntrySize = colorMapEntrySize;
    }

    void Load(FILE* read)
    {
        fread(&firstEntryIndex, 2, 1, read);
        fread(&colorMapLength, 2, 1, read);
        fread(&colorMapEntrySize, 1, 1, read);
    }

    void Save(FILE* read)
    {
        fwrite(&firstEntryIndex, 2, 1, read);
        fwrite(&colorMapLength, 2, 1, read);
        fwrite(&colorMapEntrySize, 1, 1, read);
    }
};

class ImageSpec
{
public:
    short xOrigin, yOrigin;
    unsigned short width, height;
    unsigned char pixelDepth;
    unsigned char imageDescriptor; // bits 3-0 give the alpha channel depth, bits 5-4 give pixel ordering
    // Bit 4 of the image descriptor byte indicates right-to-left pixel ordering if set. Bit 5 indicates an ordering of top-to-bottom. 
    // Otherwise, pixels are stored in bottom-to-top, left-to-right order.

    ImageSpec() :xOrigin(0), yOrigin(0), width(0), height(0), pixelDepth(0), imageDescriptor(0)
    {
    }

    void Load(FILE* read)
    {
        fread(&xOrigin, 2, 1, read);
        fread(&yOrigin, 2, 1, read);
        fread(&width, 2, 1, read);
        fread(&height, 2, 1, read);
        fread(&pixelDepth, 1, 1, read);
        fread(&imageDescriptor, 1, 1, read);
    }

    void Save(FILE* read)
    {
        fwrite(&xOrigin, 2, 1, read);
        fwrite(&yOrigin, 2, 1, read);
        fwrite(&width, 2, 1, read);
        fwrite(&height, 2, 1, read);
        fwrite(&pixelDepth, 1, 1, read);
        fwrite(&imageDescriptor, 1, 1, read);
    }
};

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

class TGA_IO
{
public:
    static bool ReadTGA(string path, unsigned int& width, unsigned int& height, unsigned int& pixelSize, vector<unsigned char>& data);
	static bool WriteTGA(string path, unsigned int width, unsigned int height, vector<unsigned char> &data); // Grayscale
};