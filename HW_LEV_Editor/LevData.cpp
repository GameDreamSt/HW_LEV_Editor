
#include <LevData.h>

HWTerrain::HWTerrain()
{
	versionID = 0;

	TerrainPointDataOffset = 0;
	width = 0;
	height = 0;
	LowestPoint = 0;
	HighestPoint = 0;

	LandPaletteOffset = 0;
	ObjectListOffset = 0;
	ModelListOffset = 0;
	LevelConfigOffset = 0;
	EndOfLastBit = 0;
}

HWTerrain::~HWTerrain()
{
}
