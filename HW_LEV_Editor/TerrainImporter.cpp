
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
	for (size_t i = 0; i < terr->width; i++)
		for (size_t j = 0; j < terr->height; j++)
		{
			TerrainPoint* p = terr->terrainPoints.at(i).at(j);
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

void ImportFromTga(HWTerrain* myTerrain, string path)
{
	return;
}
