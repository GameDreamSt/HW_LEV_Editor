
#include <TGA_IO.h>

#define X_IMAGE_FLIP 4
#define Y_IMAGE_FLIP 5

bool TGA_IO::ReadTGA(string path, unsigned int& width, unsigned int& height, unsigned int &pixelSize, vector<unsigned char>& data)
{
    FILE* read;
    errno_t err = fopen_s(&read, path.c_str(), "rb");

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

    if (imageType <= 1 || imageType >= 4)
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

    if (colorMapType != 0)
    {
        cout << "Color map .tga unsupported!\n";
        fclose(read);
        return false;
    }

    width = imageSpec.width;
    height = imageSpec.height;

    pixelSize = imageSpec.pixelDepth / 8;

    if (pixelSize * 8 != imageSpec.pixelDepth)
    {
        cout << ".tga pixel depth is not multiple of 8!\n";
        fclose(read);
        return false;
    }

    data.resize(width * height * pixelSize);
    fread(data.data(), data.size(), 1, read);

    fclose(read);

    return true;
}

bool TGA_IO::WriteTGA(string path, unsigned int width, unsigned int height, vector<unsigned char>& data)
{
    FILE* write;
    errno_t err = fopen_s(&write, path.c_str(), "wb");

    if (write == nullptr)
    {
        cout << "Failed to open/create the file";
        return false;
    }

    string IDField = "";
    unsigned char IDFieldLength = (unsigned char)IDField.length();

    unsigned char colorMapType = 0;
    unsigned char imageType = TGAImageType::UncompressedGrayscale;
    ColorMapSpec colorMapSpec = ColorMapSpec(0, 0, 0);
    ImageSpec imageSpec;
    imageSpec.width = (unsigned short)width;
    imageSpec.height = (unsigned short)height;
    imageSpec.pixelDepth = 8;
    imageSpec.imageDescriptor = (1 << Y_IMAGE_FLIP);

    //https://en.wikipedia.org/wiki/Truevision_TGA
    fwrite(&IDFieldLength, 1, 1, write); // Field 1
    fwrite(&colorMapType, 1, 1, write); // Field 2
    fwrite(&imageType, 1, 1, write); // Field 3

    colorMapSpec.Save(write); // Field 4
    imageSpec.Save(write); // Field 5

    if (IDFieldLength > 0) // Field 6
        fwrite(IDField.data(), 1, IDField.length(), write);

    fwrite(data.data(), sizeof(unsigned char), data.size(), write);

    fclose(write);

	return true;
}
