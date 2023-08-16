
#include <TerrainImporter.h>

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

	unsigned int width, height, pixelSize;

	bool success = TGA_IO::ReadTGA(path, width, height, pixelSize, pixelData);
	if (!success)
	{
		pixelData.clear();
		return false;
	}

	float low = myTerrain->LowestPoint;
	float high = myTerrain->HighestPoint;

	int lowMeter = low / 51.2f;
	int highMeter = high / 51.2f;

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
		heights[i] = RemapFrom01(var, low, high);
	}

	int terrWidth = myTerrain->width;
	int terrHeight = myTerrain->height;

	float widthRatio = width / myTerrain->width;
	float heightRatio = height / myTerrain->height;

	vector<TerrainPoint*>* points = &myTerrain->terrainPoints;
	for (size_t i = 0; i < points->size(); i++)
	{
		int px = i % terrWidth;
		int py = i / terrWidth;
		int x = lroundf(px * widthRatio);
		int y = lroundf(py * heightRatio);
		points->at(py * terrWidth + px)->Height = heights[y * width + x];
	}

	heights.clear();
	pixelData.clear();
	return true;
}
