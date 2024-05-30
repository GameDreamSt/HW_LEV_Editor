
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
                TerrainPoint TPoint = *myTerrain.terrainPoints.at(i * (size_t)myTerrain.width + j);

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
                TerrainPoint TPoint = *myTerrain.terrainPoints.at(i * (size_t)myTerrain.width + j);

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
    offset += (unsigned long)myTerrain.objectListData.size();
    if (myTerrain.versionID == LEVEL0_4CC)
        Head0.ModelListOffset = offset;
    else
        Head1.ModelListOffset = offset;
    
    fwrite(myTerrain.modelListData.data(), sizeof(char), myTerrain.modelListData.size(), f);
    offset += (unsigned long)myTerrain.modelListData.size();
    if (myTerrain.versionID == LEVEL0_4CC)
        Head0.LandPaletteOffset = offset;
    else
        Head1.LandPaletteOffset = offset;

    fwrite(myTerrain.colours.data(), sizeof(Colour), myTerrain.colours.size(), f);
    offset += (unsigned long)(sizeof(Colour) * myTerrain.colours.size());
    if (myTerrain.versionID == LEVEL0_4CC)
        Head0.LevelConfigOffset = offset;
    else
        Head1.LevelConfigOffset = offset;

    fwrite(myTerrain.configData.data(), sizeof(char), myTerrain.configData.size(), f);
    offset += (unsigned long)myTerrain.configData.size();

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

class QuadTexIds
{
public:
    int a, b, c, d;
    QuadTexIds() { a = 0; b = 0; c = 0; d = 0; }
};

#define Q00 (int)(y * (size_t)lineSizeBigger + 1 + x)
#define Q01 (int)(y * (size_t)lineSizeBigger + (size_t)2 + x)
#define Q10 (int)(y * (size_t)lineSizeBigger + lineSize + (size_t)2 + x)
#define Q11 (int)(y * (size_t)lineSizeBigger + lineSize + (size_t)3 + x)

void GenerateTextureVerts(int amountOfMats, vector<pair<float, float>> &texVerts, vector<QuadTexIds> &quadTexIds)
{
    int lineSize = 10;
    int lineSizeBigger = lineSize + 1;
    int matLines = amountOfMats / lineSize;
    int matRemain = amountOfMats - matLines * lineSize;
    int allMatLines = 0;

    int texVertAmount = 0;
    if (matLines > 0)
    {
        texVertAmount = lineSizeBigger + matLines * lineSizeBigger;
        allMatLines = matLines;
    }

    if (matRemain > 0)
    {
        allMatLines++;
        if (texVertAmount == 0)
            texVertAmount = (matRemain + 1) * 2;
        else
            texVertAmount += (matRemain + 1);
    }

    texVerts.resize(texVertAmount);

    for (int i = 0; i < texVertAmount; i++)
    {
        int x = i % lineSizeBigger;
        int y = i / lineSizeBigger;

        texVerts.at(i).first = x / (float)(lineSize);
        texVerts.at(i).second = 1 - y / (float)allMatLines;
    }

    if (allMatLines == 0)
        return;

    quadTexIds.resize(amountOfMats * 8); // 8 rotations

    for (int i = 0; i < quadTexIds.size(); i++)
    {
        int rot = i / amountOfMats;
        int I = i % amountOfMats;

        int x = I % lineSize;
        int y = I / lineSize;

        switch (rot)
        {
            default:
            case 0:
                quadTexIds[i].a = Q00;
                quadTexIds[i].b = Q10;
                quadTexIds[i].c = Q01;
                quadTexIds[i].d = Q11;
                break;
            case 1:
                quadTexIds[i].a = Q01; // Q10
                quadTexIds[i].b = Q00; // Q11
                quadTexIds[i].c = Q11; // Q00
                quadTexIds[i].d = Q10; // Q01
                break;
            case 2:
                quadTexIds[i].a = Q11;
                quadTexIds[i].b = Q01;
                quadTexIds[i].c = Q10;
                quadTexIds[i].d = Q00;
                break;
            case 3:
                quadTexIds[i].a = Q10; // Q01
                quadTexIds[i].b = Q11; // Q00
                quadTexIds[i].c = Q00; // Q11
                quadTexIds[i].d = Q01; // Q10
                break;
            case 4:
                quadTexIds[i].a = Q01; // Q00
                quadTexIds[i].b = Q11; // Q01
                quadTexIds[i].c = Q00; // Q10
                quadTexIds[i].d = Q10; // Q11
                break;
            case 5:
                quadTexIds[i].a = Q11;
                quadTexIds[i].b = Q10;
                quadTexIds[i].c = Q01;
                quadTexIds[i].d = Q00;
                break;
            case 6:
                quadTexIds[i].a = Q10;
                quadTexIds[i].b = Q00;
                quadTexIds[i].c = Q11;
                quadTexIds[i].d = Q01;
                break;
            case 7:
                quadTexIds[i].a = Q00;
                quadTexIds[i].b = Q01;
                quadTexIds[i].c = Q10;
                quadTexIds[i].d = Q11;
                break;
        }
    }
}

void ExportTerrainToObj(HWTerrain &myTerrain, string outputFolder, string filename, string textureFile)
{
    ObjObject exportTerrain;

    exportTerrain.name = filename;

    vector<Vector3> verts;
    verts.resize(myTerrain.width * (size_t)myTerrain.height);

    int materialCount = 0;
    for (size_t i = 0; i < myTerrain.height; i++)
        for (size_t j = 0; j < myTerrain.width; j++)
        {
            TerrainPoint* p = myTerrain.terrainPoints.at(i * myTerrain.width + j);
            if (p->Mat > materialCount)
                materialCount = p->Mat;
            verts[i * myTerrain.width + j] = Vector3(i * QuadSize, p->Height * ScaleMod, j * QuadSize);
        }
    materialCount++; // Because of 0 mat

    exportTerrain.verts = verts;

    vector<QuadTexIds> quadTexIds;
    GenerateTextureVerts(materialCount, exportTerrain.texVerts, quadTexIds);

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

            TerrainPoint* p = myTerrain.terrainPoints.at(i * myTerrain.width + j);

            int* ptr = &q.texVertIds[0];
            memcpy(ptr, &quadTexIds.at(p->Mat + materialCount * p->TextureDir), sizeof(QuadTexIds));

            quads[i * widthSmol + j] = q;
        }

    exportTerrain.quads = quads;

    if(textureFile != "")
        exportTerrain.textureFilenames.push_back(textureFile);

    string outputFilepath;
    if (outputFolder != "")
        outputFilepath = outputFolder + "\\"  + filename + ".obj";

    WriteObjFile(exportTerrain, filename, outputFilepath, "");
}

float Remap01(float value, float low1, float high1)
{
    return (value - low1) / (high1 - low1);
}

void ExportImageHeight(HWTerrain& myTerrain, string path)
{
    path += "_height.tga";

    unsigned int width = myTerrain.width, height = myTerrain.height;

    vector<TerrainPoint*>* terrainPoints = &myTerrain.terrainPoints;

    size_t size = (size_t)width * (size_t)height;
    vector<unsigned char> pixels;
    pixels.resize(size);

    unsigned char* p = pixels.data();
    for (size_t x = 0; x < height; x++)
        for (size_t y = 0; y < width; y++)
        {
            TerrainPoint* point = terrainPoints->at((height - x - 1) * (width)+y);
            *p++ =(unsigned char)(255 * Remap01(point->Height, myTerrain.LowestPoint, myTerrain.HighestPoint));
        }

    TGAParams tgaParams;
    tgaParams.path = path;
    tgaParams.width = width;
    tgaParams.height = height;
    tgaParams.imageType = TGAImageType::UncompressedGrayscale;
    tgaParams.data = &pixels;
    bool success = TGA_IO::WriteTGA(tgaParams);
}

class Colour255
{
public:
    unsigned char r, g, b;

    Colour255() { r = 0; g = 0; b = 0; }
    Colour255(unsigned char R, unsigned char G, unsigned char B){ r = R; g = G; b = B; }

    static Colour255 black, white, red, green, blue, orange, yellow, cyan, purple, pink, 
        lime, darkRed, darkLime, darkGreen, darkBlue, darkOrange, darkYellow, darkCyan, darkPurple, darkPink;
};

Colour255 Colour255::black = Colour255(0, 0, 0);
Colour255 Colour255::white = Colour255(255, 255, 255);
Colour255 Colour255::red = Colour255(255, 0, 0);
Colour255 Colour255::lime = Colour255(136, 255, 0);
Colour255 Colour255::green = Colour255(0, 255, 0);
Colour255 Colour255::blue = Colour255(0, 0, 255);
Colour255 Colour255::orange = Colour255(255, 128, 0);
Colour255 Colour255::yellow = Colour255(255, 255, 0);
Colour255 Colour255::cyan = Colour255(0, 255, 255);
Colour255 Colour255::purple = Colour255(128, 0, 255);
Colour255 Colour255::pink = Colour255(255, 0, 255);

Colour255 Colour255::darkRed = Colour255(128, 0, 0);
Colour255 Colour255::darkLime = Colour255(136, 128, 0);
Colour255 Colour255::darkGreen = Colour255(0, 128, 0);
Colour255 Colour255::darkBlue = Colour255(0, 0, 128);
Colour255 Colour255::darkOrange = Colour255(128, 128, 0);
Colour255 Colour255::darkYellow = Colour255(128, 128, 0);
Colour255 Colour255::darkCyan = Colour255(0, 128, 128);
Colour255 Colour255::darkPurple = Colour255(128, 0, 128);
Colour255 Colour255::darkPink = Colour255(128, 0, 128);

Colour255 mapCols[] = { Colour255::black, Colour255::red, Colour255::green, Colour255::blue,
Colour255::orange, Colour255::yellow, Colour255::cyan, Colour255::purple, Colour255::pink, Colour255::lime, Colour255::darkRed, 
Colour255::darkLime, Colour255::darkGreen , Colour255::darkBlue , Colour255::darkOrange , Colour255::darkYellow , Colour255::darkCyan,
Colour255::darkPurple , Colour255::darkPink,  Colour255::white };

Colour255 MatToCol(int mat)
{
    if (mat >= 20 || mat < 0)
        return Colour255::white;

    return mapCols[mat];
}

bool ExportImageTiles(HWTerrain& myTerrain, string path)
{
    path += "_tiles.tga";

    unsigned int width = myTerrain.width, height = myTerrain.height;

    vector<TerrainPoint*>* terrainPoints = &myTerrain.terrainPoints;

    size_t size = (size_t)width * (size_t)height * (size_t)3;
    vector<unsigned char> pixels;
    pixels.resize(size);

    size_t x, y;

    unsigned char* p = pixels.data();
    for (x = 0; x < height; x++)
        for (y = 0; y < width; y++)
        {
            TerrainPoint* point = terrainPoints->at((height - x - 1) * (width)+y);

            //Colour255 col = MatToCol(point->Mat);

            Colour255 col = Colour255(0, point->TextureDir, point->Mat); // BGR
            memcpy(p, &col, 3);
            p += 3;
        }
   
    TGAParams params;
    params.path = path;
    params.width = width;
    params.height = height;
    params.imageType = TGAImageType::UncompressedTrueColor;
    params.data = &pixels;

    return TGA_IO::WriteTGA(params);
}
