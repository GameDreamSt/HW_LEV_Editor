#pragma once

#include <string>
#include <vector>
#include <Object.h>
#include <fstream>
#include <algorithm>

using namespace std;

bool HasAnyTextures(vector<ObjObject>& objects);
void WriteObjFile(vector<ObjObject> objects, string objFilePath, string texturesFilepath);