#pragma once

#include <string>

std::string RemoveQuotes(std::string str);

bool IsAPath(std::string str);

bool SeekTo(std::string path, int& slashI, char seekChar);

std::string RemovePathLast(std::string path);

std::string GetFileName(std::string path);

std::string RemoveExtension(std::string path);