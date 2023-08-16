#pragma once

#include <string>
#include <vector>

using namespace std;

#define	LEVEL0_4CC	'0VEL'

class Level0FileHeader
{
public:

	unsigned long	FourCC;
	unsigned long  Width;
	unsigned long  Height;
	float	HighestPoint;

	unsigned long TerrainPointDataOffset;
	unsigned long ObjectListOffset;
	unsigned long ModelListOffset;
	unsigned long ExtraModelListOffset;

	unsigned long LandPaletteOffset;
	unsigned long LevelConfigOffset;
	unsigned long EndOfLastBit;
};


#define	LEVEL1_4CC	'1VEL'

class Level1FileHeader
{
public:

	unsigned long	FourCC;
	unsigned long 	TerrainPointDataOffset;
	unsigned long  Width;
	unsigned long  Height;

	float	HighestPoint;
	float	LowestPoint;

	unsigned long ObjectListOffset;
	unsigned long ModelListOffset;
	unsigned long ExtraModelListOffset;

	unsigned long LandPaletteOffset;
	unsigned long LevelConfigOffset;
	unsigned long EndOfLastBit;
};

class Level0Point
{
public:
	float Height;

	unsigned short Normal;
	unsigned char Mat;
	unsigned char Flags;

	unsigned char PaletteIndex;
	unsigned char FlowDirection;
	unsigned char StrataIndex;
	unsigned char Pad;

	Level0Point()
	{
		Height = 0;

		Normal = 0;
		Mat = 0;
		Flags = 0;

		PaletteIndex = 0;
		FlowDirection = 0;
		StrataIndex = 0;
		Pad = 0;
	}
};

class Level1Point
{
public:
	float Height;

	unsigned short Normal;
	unsigned short Flags;

	unsigned char PaletteIndex;
	unsigned char FlowDirection;
	unsigned char StrataIndex;
	unsigned char Mat;

	unsigned char TextureDir;
	unsigned char UOff;
	unsigned char VOff;
	unsigned char AINodeType;

	Level1Point()
	{
		Height = 0;

		Normal = 0;
		Flags = 0;

		PaletteIndex = 0;
		FlowDirection = 0;
		StrataIndex = 0;
		Mat = 0;

		TextureDir = 0;
		UOff = 0;
		VOff = 0;
		AINodeType = 0;
	}
};

class StrataLayer
{
public:
	vector<unsigned char> matList;

	unsigned long x, z;

	float depth;
	float resilience; // 100.0f hard  ----  1.0f soft

	StrataLayer();
	StrataLayer(float LayerDepth);
	StrataLayer(float LayerDepth, const unsigned char Mat);
};

class StrataLayerList
{
public:
	vector<StrataLayer> layerList;

	unsigned long layerCount;
	float maxDepth;
};

class TerrainPoint
{
public:
	float Height;
	unsigned short Normal;
	unsigned short Flags;

	unsigned char Mat;
	unsigned char TextureDir;
	unsigned char PaletteIndex;
	unsigned char FlowDirection;

	unsigned char StrataIndex;
	unsigned char StrataDepthMultiplier;

	unsigned char StartMat;
	unsigned char ReverbHeight;
	unsigned long Pad2;
	unsigned long Pad3;

	TerrainPoint()
	{
		Height = 0;
		Normal = 0;
		Flags = 0;

		Mat = 0;
		TextureDir = 0;
		PaletteIndex = 0;
		FlowDirection = 0;

		StrataIndex = 0;
		StrataDepthMultiplier = 0;

		StartMat = 0;
		ReverbHeight = 0;
		Pad2 = 0;
		Pad3 = 0;
	}
};

struct Colour
{
	float x, y, z;
};


class HWTerrain
{
public:
	HWTerrain();
	~HWTerrain();

	static HWTerrain* myTerrain;

	unsigned long versionID;
	string versionStr;

	vector<TerrainPoint*> terrainPoints;
	vector<Colour> colours;

	int materialCount;
	vector<StrataLayerList> strataLayers;

	unsigned long TerrainPointDataOffset;
	unsigned long width;
	unsigned long height;
	float LowestPoint;
	float HighestPoint;

	unsigned long LandPaletteOffset;
	unsigned long ObjectListOffset;
	unsigned long ModelListOffset;
	unsigned long LevelConfigOffset;
	unsigned long EndOfLastBit;

	vector<char> objectListData;
	vector<char> modelListData;
	vector<char> configData;
};