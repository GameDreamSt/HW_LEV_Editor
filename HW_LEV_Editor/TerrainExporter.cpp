
#include <TerrainExporter.h>

unsigned short ConvertFlagsToMyFlags(unsigned short Flags)
{
    unsigned short MyFlags = 0;

    if (Flags & 1)
    {
        MyFlags |= 1;
        Flags &= ~1;
    }

    if (Flags & 2)
    {
        MyFlags |= 2;
        Flags &= ~2;
    }

    if (Flags & 4)
    {
        MyFlags |= 4;
        Flags &= ~4;
    }

    if (Flags & 8)
    {
        MyFlags |= 8;
        Flags &= ~8;
    }

    if (Flags & 16)
    {
        MyFlags |= 16;
        Flags &= ~16;
    }

    if (Flags & 32)
    {
        MyFlags |= 32;
        Flags &= ~32;
    }

    if (Flags & 64)
    {
        MyFlags |= 64;
        Flags &= ~64;
    }

    unsigned long BlendPoint = (Flags & 0x600) >> 9;
    MyFlags |= ((BlendPoint << 9) & 0x600);
    Flags &= ~0x600;

    if (Flags & 128)
    {
        MyFlags |= 128;
        Flags &= ~128;
    }

    if (Flags & 256)
    {
        MyFlags |= 256;
        Flags &= ~256;
    }

    return(MyFlags);
}

void ExportTerrainToLev(HWTerrain& myTerrain, string path)
{
    FILE* f;
    errno_t err = fopen_s(&f, path.c_str(), "wb");

    if (f == nullptr)
        return;

    Level0FileHeader    Head0 = Level0FileHeader(); // 44
    Level1FileHeader	Head1 = Level1FileHeader(); // 48

    size_t head0Size = sizeof(Level0FileHeader);
    size_t head1Size = sizeof(Level1FileHeader);

    unsigned long offset;

    if (myTerrain.versionID == LEVEL0_4CC)
        fwrite(&Head0, head0Size, 1, f);
    else
        fwrite(&Head1, head1Size, 1, f);

    if (myTerrain.versionID == LEVEL0_4CC)
    {
        Head0.FourCC = LEVEL0_4CC;
        Head0.Width = myTerrain.width;
        Head0.Height = myTerrain.height;
        Head0.HighestPoint = myTerrain.HighestPoint;
        Head0.ExtraModelListOffset = 0;

        Head0.TerrainPointDataOffset = offset = sizeof(Level0FileHeader);
    }
    else // if (id == LEVEL1_4CC)
    {
        Head1.FourCC = LEVEL1_4CC;
        Head1.Width = myTerrain.width;
        Head1.Height = myTerrain.height;
        Head1.HighestPoint = myTerrain.HighestPoint;
        Head1.LowestPoint = myTerrain.LowestPoint;
        Head1.ExtraModelListOffset = 0;

        Head1.TerrainPointDataOffset = offset = sizeof(Level1FileHeader);
    }

    if (myTerrain.versionID == LEVEL0_4CC)
    {
        Level0Point Point;
        for (unsigned long i = 0; i < myTerrain.height; i++)
            for (unsigned long j = 0; j < myTerrain.width; j++)
            {
                TerrainPoint TPoint = *myTerrain.terrainPoints.at(i * myTerrain.width + j);

                Point.Height = TPoint.Height;
                Point.Normal = TPoint.Normal;
                Point.Mat = TPoint.Mat;
                Point.Flags = ConvertFlagsToMyFlags(TPoint.Flags);
                Point.PaletteIndex = TPoint.PaletteIndex;
                Point.FlowDirection = TPoint.FlowDirection;
                Point.StrataIndex = TPoint.StrataIndex;

                fwrite(&Point, sizeof(Level0Point), 1, f);
            }

        offset += (sizeof(Level0Point) * myTerrain.height) * myTerrain.width;
        Head0.ObjectListOffset = offset;
    }
    else
    {
        Level1Point Point;
        for (unsigned long i = 0; i < myTerrain.height; i++)
            for (unsigned long j = 0; j < myTerrain.width; j++)
            {
                TerrainPoint TPoint = *myTerrain.terrainPoints.at(i * myTerrain.width + j);

                Point.Height = TPoint.Height;
                Point.Normal = TPoint.Normal;
                Point.Mat = TPoint.Mat;
                Point.Flags = ConvertFlagsToMyFlags(TPoint.Flags);
                Point.PaletteIndex = TPoint.PaletteIndex;
                Point.FlowDirection = TPoint.FlowDirection;
                Point.StrataIndex = TPoint.StrataIndex;
                Point.TextureDir = TPoint.TextureDir;
                Point.AINodeType = 0;

                fwrite(&Point, sizeof(Level1Point), 1, f);
            }

        offset += (sizeof(Level1Point) * myTerrain.height) * myTerrain.width;
        Head1.ObjectListOffset = offset;
    }

    fwrite(myTerrain.objectListData.data(), sizeof(char), myTerrain.objectListData.size(), f);
    offset += myTerrain.objectListData.size();
    if (myTerrain.versionID == LEVEL0_4CC)
        Head0.ModelListOffset = offset;
    else
        Head1.ModelListOffset = offset;
    
    fwrite(myTerrain.modelListData.data(), sizeof(char), myTerrain.modelListData.size(), f);
    offset += myTerrain.modelListData.size();
    if (myTerrain.versionID == LEVEL0_4CC)
        Head0.LandPaletteOffset = offset;
    else
        Head1.LandPaletteOffset = offset;

    fwrite(myTerrain.colours.data(), sizeof(Colour), myTerrain.colours.size(), f);
    offset += sizeof(Colour) * myTerrain.colours.size();
    if (myTerrain.versionID == LEVEL0_4CC)
        Head0.LevelConfigOffset = offset;
    else
        Head1.LevelConfigOffset = offset;

    fwrite(myTerrain.configData.data(), sizeof(char), myTerrain.configData.size(), f);
    offset += myTerrain.configData.size();

    if (myTerrain.versionID == LEVEL0_4CC)
    {
        Head0.EndOfLastBit = offset;

        fseek(f, 0, SEEK_SET);
        fwrite(&Head0, sizeof(Level0FileHeader), 1, f);
    }
    else // if (id == LEVEL1_4CC)
    {
        Head1.EndOfLastBit = offset;

        fseek(f, 0, SEEK_SET);
        fwrite(&Head1, sizeof(Level1FileHeader), 1, f);
    }

    fclose(f);
}

void ExportTerrainToObj(HWTerrain &myTerrain, string outputFolder, string filename)
{
    ObjObject exportTerrain;

    exportTerrain.name = filename;

    vector<Vector3> verts;
    verts.resize(myTerrain.width * (size_t)myTerrain.height);

    for (size_t i = 0; i < myTerrain.height; i++)
        for (size_t j = 0; j < myTerrain.width; j++)
        {
            TerrainPoint* p = myTerrain.terrainPoints.at(i * myTerrain.width + j);
            verts[i * myTerrain.width + j] = Vector3(i * QuadSize, p->Height * ScaleMod, j * QuadSize);
        }

    exportTerrain.verts = verts;

    unsigned long widthSmol = myTerrain.width - 1, heightSmol = myTerrain.height - 1;

    vector<Quad> quads;
    quads.resize(widthSmol * (size_t)heightSmol);
    for (size_t i = 0; i < heightSmol; i++)
        for (size_t j = 0; j < widthSmol; j++)
        {
            Quad q;

            q.vertIds[0] = (int)(i * myTerrain.width + 1 + j);
            q.vertIds[1] = (int)(i * myTerrain.width + (size_t)2 + j);
            q.vertIds[2] = (int)(i * myTerrain.width + myTerrain.width + (size_t)1 + j);
            q.vertIds[3] = (int)(i * myTerrain.width + myTerrain.width + (size_t)2 + j);

            quads[i * widthSmol + j] = q;
        }

    exportTerrain.quads = quads;

    vector<ObjObject> objects;
    objects.push_back(exportTerrain);

    string outputFilepath;
    if (outputFolder != "")
        outputFilepath = outputFolder + "\\"  + filename + ".obj";

    WriteObjFile(objects, outputFilepath, "");
}

float Remap01(float value, float low1, float high1)
{
    return (value - low1) / (high1 - low1);
}

void ExportImage(HWTerrain& myTerrain, string path)
{
    path += ".tga";

    FILE* write;
    errno_t err = fopen_s(&write, path.c_str(), "wb");

    if (write == nullptr)
    {
        cout << "Failed to open/create the file";
        return;
    }

    unsigned int width = myTerrain.width, height = myTerrain.height;

    vector<TerrainPoint*>* terrainPoints = &myTerrain.terrainPoints;

    size_t size = (size_t)width * (size_t)height * (size_t)3;
    vector<unsigned char> pixels;
    pixels.resize(size);

    unsigned char tga[18]{};
    size_t x, y;

    unsigned char* p = pixels.data();
    for (x = 0; x < height; x++)
        for (y = 0; y < width; y++)
        {
            TerrainPoint* point = terrainPoints->at((height - x - 1) * (width) + y);

            *p++ = *p++ = *p++ = (unsigned char)(255 * Remap01(point->Height, myTerrain.LowestPoint, myTerrain.HighestPoint));
        }

    tga[2] = 2;
    tga[12] = 255 & width;
    tga[13] = 255 & (width >> 8);
    tga[14] = 255 & height;
    tga[15] = 255 & (height >> 8);
    tga[16] = 24;
    tga[17] = 32;

    fwrite(tga, sizeof(unsigned char), 18, write);
    fwrite(pixels.data(), sizeof(unsigned char), size, write);

    fclose(write);
}
