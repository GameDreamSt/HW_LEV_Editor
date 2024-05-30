
#include <TerrainImporter.h>

#include <LevReader.h>
#include <LevData.h>

#include <ModelLoader.h>
#include <TGA_IO.h>

void ImportFromObj(HWTerrain* terr, string path)
{
	vector<ObjObject*> objs = LoadModelsFromFile(path);

	if (objs.size() == 0)
	{
		cout << "Failed to read .obj file at path:\n" + path + "\n";
		return;
	}

	ObjObject* myObj = objs[0];

	if (myObj->verts.size() != (terr->width * (size_t)terr->height))
	{
		cout << "Import vert count and current terrain points mismatch!";
		return;
	}

	float maxH = -99999;
	float minH = 99999;
	for (size_t i = 0; i < terr->height; i++)
		for (size_t j = 0; j < terr->width; j++)
		{
			TerrainPoint* p = terr->terrainPoints.at(i * terr->width + j);
			p->Height = myObj->verts[i * terr->width + j].y * ScaleUp;

			if (p->Height > maxH)
				maxH = p->Height;
			if (p->Height < minH)
				minH = p->Height;
		}

	terr->HighestPoint = maxH;
	terr->LowestPoint = minH;

	cout << "Terrain imported!\n";
}

float RemapFrom01(float var, float min, float high)
{
	return min + var * (high - min);
}

vector<unsigned char> pixelData;
vector<float> heights;
bool ImportFromTga(HWTerrain* myTerrain, string path)
{
	if (myTerrain == nullptr)
	{
		cout << "Terrain is not loaded?\n";
		return false;
	}

	unsigned int width = 0, height = 0, pixelSize = 0;

	TGAParams tgaParams;
	tgaParams.path = path;
	tgaParams.width = width;
	tgaParams.height = height;
	tgaParams.imageType = TGAImageType::UncompressedGrayscale;
	tgaParams.data = &pixelData;

	bool success = TGA_IO::ReadTGA(tgaParams);
	if (!success)
	{
		pixelData.clear();
		return false;
	}

	float low = myTerrain->LowestPoint;
	float high = myTerrain->HighestPoint;

	int lowMeter = (int)(low / 51.2f);
	int highMeter = (int)(high / 51.2f);

	string choice = "";
	cout << "Use terrain's lowest and highest height points? [Y/N]\n";
	cout << "Current terrain's lowest: " << lowMeter << " | highest: " << highMeter << "\n";
	cin >> choice;

	if (choice[0] != 'Y' && choice[0] != 'y')
	{
		cout << "Enter terrain low (in meters): ";
		cin >> low;
		low *= 51.2f;

		cout << "Enter terrain high (in meters): ";
		cin >> high;
		high *= 51.2f;
	}

	float var;

	heights.resize(width * height);
	for (size_t i = 0; i < heights.size(); i++)
	{
		var = pixelData[i * pixelSize] / 255.0f;
		int x = i % width;
		int y = i / height;
		heights[(height - y - 1) * width + x] = RemapFrom01(var, low, high);
	}

	int terrWidth = myTerrain->width;
	int terrHeight = myTerrain->height;

	float widthRatio = width / (float)myTerrain->width;
	float heightRatio = height / (float)myTerrain->height;

	vector<TerrainPoint*>* points = &myTerrain->terrainPoints;
	for (size_t i = 0; i < points->size(); i++)
	{
		size_t px = i % terrWidth;
		size_t py = i / terrWidth;
		int x = lroundf(px * widthRatio);
		int y = lroundf(py * heightRatio);
		points->at(py * terrWidth + px)->Height = heights[y * width + x];
	}

	heights.clear();
	pixelData.clear();
	return true;
}

bool ImportPaletteFromImage(string filepath)
{
	HWTerrain* terrain = HWTerrain::myTerrain;

	if (terrain == nullptr)
	{
		cout << "Error! Missing terrain!";
		return false;
	}

	vector<unsigned char> pixels;
	pixels.resize(256 * 3);

	TGAParams params;
	params.path = filepath;
	params.width = 16;
	params.height = 16;
	params.imageType = TGAImageType::UncompressedTrueColor;
	params.data = &pixels;

	if (!TGA_IO::ReadTGA(params))
		return false;

	unsigned char* p = pixels.data();

	vector<Colour>* colours = &terrain->colours;

	unsigned char tempColourData[3]{};
	Colour tempCol{};
	for (size_t i = 0; i < colours->size(); i++)
	{
		memcpy(&tempColourData, p, sizeof(unsigned char) * 3);
		p += 3;

		tempCol.x = tempColourData[0] / 255.0f;
		tempCol.y = tempColourData[1] / 255.0f;
		tempCol.z = tempColourData[2] / 255.0f;

		colours->at(i) = tempCol;
	}

	return true;
}
