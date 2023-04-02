#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <Files.h>

#include <Vectors.h>
#include <Object.h>

using namespace std;

struct Vert
{
	Vector3 pos;
	Vector2 uv;
	Vector3 norm;

	Vert() {}
	Vert(Vector3 pos, Vector2 uv, Vector3 norm)
	{
		this->pos = pos;
		this->uv = uv;
		this->norm = norm;
	}
};

struct StructMaterial
{
	string name;
	string texturePath;

	StructMaterial() {}
	StructMaterial(string name, string texturePath)
	{
		this->name = name;
		this->texturePath = texturePath;
	}
};

vector<ObjObject*> LoadModelsFromFile(string path);