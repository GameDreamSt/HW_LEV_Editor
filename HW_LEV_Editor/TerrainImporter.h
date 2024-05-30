#pragma once

#include <string>

class HWTerrain;

using namespace std;

#define ScaleUp 51.2f
#define ScaleMod 0.01953125f // 1/51.2f
#define QuadSize 11.52f

void ImportFromObj(HWTerrain* myTerrain, string path);
bool ImportFromTga(HWTerrain* myTerrain, string path);
bool ImportPaletteFromImage(string filepath);