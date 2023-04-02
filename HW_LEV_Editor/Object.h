#pragma once

#include <string>
#include <vector>
#include <Vectors.h>

using namespace std;

class Quad
{
public:
    Quad();

    int vertIds[4];
    int texVertIds[4];
    float normals[12];

    string textureName;
    float glow;
    Vector3 flat;

    string opacityMode;
    float opacityStrength;

    string enviroMapTex;
    float enviroMapStrength;
};

class Tris
{
public:
    Tris();

    int vertIds[3];
    int texVertIds[3];
    float normals[9];

    string textureName;
    float glow;
    Vector3 flat;

    string opacityMode;
    float opacityStrength;

    string enviroMapTex;
    float enviroMapStrength;
};

class ObjObject
{
public:
    ObjObject();
    ObjObject(string name);

    string name;

    Vector3 position;

    vector<Vector3> verts; // x y z
    vector<pair<float, float>> texVerts; // uvs

    vector<Quad> quads;
    vector<Tris> tris;

    vector<string> textureFilenames;
};