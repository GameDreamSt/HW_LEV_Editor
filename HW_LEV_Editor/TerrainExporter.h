#pragma once

#include <string>

using namespace std;

class HWTerrain;

#define ScaleMod 0.01953125f // 1/51.2f
#define QuadSize 11.52f

void ExportTerrainToLev(HWTerrain& myTerrain, string path);
void ExportTerrainToObj(HWTerrain &myTerrain, string path, string filename, string textureFile);
void ExportImageHeight(HWTerrain& myTerrain, string path);
bool ExportImageTiles(HWTerrain& myTerrain, string path);
bool ExportPaletteToImage(string filePath);