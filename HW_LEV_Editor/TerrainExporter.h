#pragma once

#include <LevReader.h>
#include <LevData.h>

#include <ObjWriter.h>

#include <string>

using namespace std;

#define ScaleMod 0.01953125f // 1/51.2f
#define QuadSize 11.52f

void ExportTerrainToLev(HWTerrain& myTerrain, string path);
void ExportTerrainToObj(HWTerrain &myTerrain, string path, string filename);
void ExportImage(HWTerrain& myTerrain, string path);