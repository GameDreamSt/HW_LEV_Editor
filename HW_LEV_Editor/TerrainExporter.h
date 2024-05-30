#pragma once

#include <LevReader.h>
#include <LevData.h>

#include <ObjWriter.h>
#include <TGA_IO.h>

#include <string>

using namespace std;

#define ScaleMod 0.01953125f // 1/51.2f
#define QuadSize 11.52f

void ExportTerrainToLev(HWTerrain& myTerrain, string path);
void ExportTerrainToObj(HWTerrain &myTerrain, string path, string filename, string textureFile);
void ExportImageHeight(HWTerrain& myTerrain, string path);
bool ExportImageTiles(HWTerrain& myTerrain, string path);