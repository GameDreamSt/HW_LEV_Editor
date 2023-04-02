#pragma once

#include <LevReader.h>
#include <LevData.h>

#include <ModelLoader.h>

#include <string>

using namespace std;

#define ScaleUp 51.2f
#define ScaleMod 0.01953125f // 1/51.2f
#define QuadSize 11.52f

void ImportFromObj(HWTerrain* myTerrain, string path);
void ImportFromTga(HWTerrain* myTerrain, string path);