#pragma once

#include <string>
#include <vector>
#include <Object.h>
#include <fstream>
#include <algorithm>

using namespace std;

void WriteObjFile(ObjObject &objects, string filename, string objFilePath, string texturesFilepath);