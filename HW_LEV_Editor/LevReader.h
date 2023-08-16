#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include <LevData.h>

using namespace std;

#define READ_ULONG(x,y) *(unsigned long*)&x[y]; y+=4;

bool ReadLevFile(std::string path);