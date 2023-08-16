
#include <LevData.h>

HWTerrain* HWTerrain::myTerrain = nullptr;

HWTerrain::HWTerrain()
{
	versionID = 0;

	TerrainPointDataOffset = 0;
	width = 0;
	height = 0;
	LowestPoint = 0;
	HighestPoint = 0;
	materialCount = 1;

	LandPaletteOffset = 0;
	ObjectListOffset = 0;
	ModelListOffset = 0;
	LevelConfigOffset = 0;
	EndOfLastBit = 0;
}

HWTerrain::~HWTerrain()
{
}

StrataLayer::StrataLayer()
{
	depth = 150;
	matList.resize(1);
	matList[0] = 0;
	x = z = 1;
	resilience = 1;
}

StrataLayer::StrataLayer(float LayerDepth)
{
	depth = LayerDepth;
	matList.resize(1);
	matList[0] = 0;
	x = z = 1;
	resilience = 1;
}

StrataLayer::StrataLayer(float LayerDepth, const unsigned char Mat)
{
	depth = LayerDepth;
	matList.resize(1);
	matList[0] = Mat;
	x = z = 1;
	resilience = 1;
}
