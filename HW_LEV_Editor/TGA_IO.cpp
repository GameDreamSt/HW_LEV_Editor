
#include <TGA_IO.h>

//https://en.wikipedia.org/wiki/Truevision_TGA

#define X_IMAGE_FLIP 4
#define Y_IMAGE_FLIP 5

#define COLOR_MAP_DEPTH 16 // 16bit color lookup
#define TRUE_COLOR_DEPTH 24 // RGB 888

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

unsigned char TGATypeToPixelDepth(TGAImageType type)
{
    switch (type)
    {
    default:
    case NoData:
    case UncompressedGrayscale:
    case RunLengthGrayscale:
    case UNKNOWN4:
    case UNKNOWN5:
    case UNKNOWN6:
    case UNKNOWN7:
    case UNKNOWN8:
        return 8;
        break;

    case RunLengthColorMap:
    case UncompressedColorMap:
        return COLOR_MAP_DEPTH;
        break;

    case UncompressedTrueColor:
    case RunLengthTrueColor:
        return TRUE_COLOR_DEPTH;
    }
}

bool TGA_IO::ReadTGA(TGAParams& params)
{
    FILE* read;
    errno_t err = fopen_s(&read, params.path.c_str(), "rb");

    if (read == nullptr)
    {
        cout << "Failed to open/read the file";
        return false;
    }

    string IDField = "";
    unsigned char IDFieldLength;

    unsigned char colorMapType;
    unsigned char imageType;
    ColorMapSpec colorMapSpec;
    ImageSpec imageSpec;

    fread(&IDFieldLength, 1, 1, read); // Field 1
    fread(&colorMapType, 1, 1, read); // Field 2
    fread(&imageType, 1, 1, read); // Field 3

    if (colorMapType != 0)
    {
        cout << "Color map .tga unsupported!\n";
        fclose(read);
        return false;
    }

    if (imageType <= 1 || imageType >= 4) // Only support uncompressed true color and grayscale
    {
        cout << "Image type unsupported!\n";
        fclose(read);
        return false;
    }

    colorMapSpec.Load(read); // Field 4
    imageSpec.Load(read); // Field 5

    if (IDFieldLength > 0) // Field 6
    {
        char* IDFieldData = new char[IDFieldLength];
        fread(IDFieldData, 1, IDFieldLength, read);
        IDField = string(IDFieldData);
        delete[] IDFieldData;
    }

    params.width = imageSpec.width;
    params.height = imageSpec.height;

    int pixelSize = (int)ceill(imageSpec.pixelDepth / 8.0f); // e.g. 15 / 8 = 1.875, ceil(1.875) = 2 bytes per pixel

    size_t bytesTotal = imageSpec.width * imageSpec.height * pixelSize;
    params.data->resize(bytesTotal);
    fread(params.data->data(), bytesTotal, 1, read);

    fclose(read);

    return true;
}

void CheckExtension(TGAParams& params)
{
    string path = params.path;
    if (path.length() >= 4 && path[path.length() - 4] != '.')
        path += ".tga";
    params.path = path;
}

bool TGA_IO::WriteTGA(TGAParams& params)
{
    CheckExtension(params);

    FILE* write;
    errno_t err = fopen_s(&write, params.path.c_str(), "wb");

    if (write == nullptr)
    {
        cout << "Failed to open/create the file";
        return false;
    }

    string IDField = "";
    unsigned char IDFieldLength = (unsigned char)IDField.length();

    unsigned char colorMapType = 0;
    unsigned char imageType = params.imageType;
    ColorMapSpec colorMapSpec = ColorMapSpec(0, 0, 0);
    ImageSpec imageSpec;
    imageSpec.width = (unsigned short)params.width;
    imageSpec.height = (unsigned short)params.height;
    imageSpec.pixelDepth = TGATypeToPixelDepth(params.imageType);
    imageSpec.imageDescriptor = (1 << Y_IMAGE_FLIP);

    fwrite(&IDFieldLength, 1, 1, write); // Field 1
    fwrite(&colorMapType, 1, 1, write); // Field 2
    fwrite(&imageType, 1, 1, write); // Field 3

    colorMapSpec.Save(write); // Field 4
    imageSpec.Save(write); // Field 5

    if (IDFieldLength > 0) // Field 6
        fwrite(IDField.data(), 1, IDField.length(), write);

    int pixelSize = (int)ceill(imageSpec.pixelDepth / 8.0f); // e.g. 15 / 8 = 1.875, ceil(1.875) = 2 bytes per pixel
    size_t bytesTotal = imageSpec.width * imageSpec.height * pixelSize;

    if (bytesTotal != params.data->size())
    {
        cout << "Failed to write .tga, source bytes doesn't match output bytes length!";
        return false;
    }

    fwrite(params.data->data(), bytesTotal, 1, write);

    fclose(write);

	return true;
}
