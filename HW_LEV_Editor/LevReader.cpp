
#include <LevReader.h>

HWTerrain* myTerrain;

bool ReadAllBytes(string filePath, vector<unsigned char>& data)
{
	ifstream ifs(filePath, ios::binary | ios::ate);

	if (!ifs.is_open())
		return false;

	ifstream::pos_type pos = ifs.tellg();

	if (pos == 0)
		return false;

	ifs.seekg(0, ios::beg);

	vector<char> tempData;
	tempData.resize((size_t)pos);

	ifs.read(&tempData[0], pos);

	std::copy(tempData.begin(), tempData.end(), std::back_inserter(data));

	ifs.close();

	return true;
}

unsigned short ConvertMyFlagsToFlags(unsigned short MyFlags)
{
	unsigned short Flags = 0;

	if (MyFlags & 1) // LEVFLAGS_TP_DRAW
		Flags |= 1; // TP_DRAW

	if (MyFlags & 2) // LEVFLAGS_TP_WET
		Flags |= 2; // TP_WET

	if (MyFlags & 4) // LEVFLAGS_TP_DRYPOINT
		Flags |= 4; // TP_DRYPOINT

	if (MyFlags & 8) // LEVFLAGS_TP_WETPOINT
		Flags |= 8; // TP_WETPOINT

	if (MyFlags & 16) // LEVFLAGS_TP_SPLITDIR
		Flags |= 16; // TP_SPLITDIR

	if (MyFlags & 32) // LEVFLAGS_TP_SHOREPOINT
		Flags |= 32; // TP_SHOREPOINT

	if (MyFlags & 64) // LEVFLAGS_TP_NOBLEND
		Flags |= 64; // TP_NOBLEND

	unsigned long BlendPoint = (MyFlags & 0x600) >> 9; // LEVFLAGS_TP_NOBLENDMASK, LEVFLAGS_TP_NOBLENDSHIFT
	Flags |= ((BlendPoint << 9) & 0x600); // TP_NOBLENDSHIFT, TP_NOBLENDMASK

	if (MyFlags & 256) // LEVFLAGS_TP_STILLWATER
	{
		//LogFile("Loaded Stillwater flag");
		Flags |= 256; // TP_STILLWATER
	}

	if (MyFlags & 128) // LEVFLAGS_TP_NOQUAD
		Flags |= 128; // TP_NOQUAD

	return(Flags);
}

void ReadTerrainPoint_Format0(vector<unsigned char>& allData, TerrainPoint* TPoint, unsigned long &cursor)
{
	Level0Point	LPoint = *(Level0Point*)&allData[cursor];
	cursor += sizeof(LPoint);

	TPoint->Height = LPoint.Height;
	TPoint->Normal = LPoint.Normal;
	TPoint->Flags = ConvertMyFlagsToFlags(LPoint.Flags);
	TPoint->Mat = LPoint.Mat;
	TPoint->StartMat = LPoint.Mat;
	TPoint->StrataIndex = LPoint.StrataIndex;
	TPoint->PaletteIndex = LPoint.PaletteIndex;
	TPoint->TextureDir = 0;
	TPoint->FlowDirection = 0;
}

void ReadTerrainPoint_Format1(vector<unsigned char>& allData, TerrainPoint* TPoint, unsigned long& cursor)
{
	Level1Point	LPoint = *(Level1Point*)&allData[cursor];
	cursor += sizeof(LPoint);

	TPoint->Height = LPoint.Height;
	TPoint->Normal = LPoint.Normal;

	if (LPoint.Normal > 4096)
		cout << "Illegal terrainpoint normal !! at {" << cursor << "}\n";

	TPoint->Flags = ConvertMyFlagsToFlags(LPoint.Flags);

	TPoint->Mat = LPoint.Mat;
	TPoint->StartMat = LPoint.Mat;

	TPoint->PaletteIndex = LPoint.PaletteIndex;
	TPoint->StrataIndex = LPoint.StrataIndex;
	TPoint->StrataDepthMultiplier = 255;

	if (LPoint.TextureDir > 7) // Support legacy levels
		TPoint->TextureDir = LPoint.TextureDir >> 6;
	else
		TPoint->TextureDir = LPoint.TextureDir;

	TPoint->FlowDirection = LPoint.FlowDirection;
}

unsigned long interestingHalfBytes[2] = { '0VEL', '1VEL' };
string interestingHalfBytesStr[2] = { "0VEL", "1VEL"};
bool ReadLevFile(std::string path)
{
	vector<unsigned char> allData;

	if (!ReadAllBytes(path, allData))
	{
		cout << "Error finding the file: " << path << '\n';
		return false;
	}

	if (allData.size() <= 8)
		return false;

	unsigned long versionRaw = *(unsigned long*)(&allData[0]);
	string fileVersion = "";

	for (int j = 0; j < 10; j++)
		if (versionRaw == interestingHalfBytes[j])
		{
			fileVersion = interestingHalfBytesStr[j];
			reverse(fileVersion.begin(), fileVersion.end());
			cout << "Version: " << fileVersion << '\n';
			break;
		}

	cout << "File size in bytes: " << allData.size() << '\n';

	if (fileVersion == "")
	{
		cout << "No valid file version found!\n";
		return false;
	}

	if (fileVersion != "LEV0" && fileVersion != "LEV1")
	{
		cout << "The file version is not LEV0 or LEV1, conversions are not supported!\n";
		return false;
	}

	myTerrain = new HWTerrain();

	unsigned long cursor = 0;
	unsigned long id = myTerrain->versionID = *(unsigned long*)&allData[0];
	myTerrain->versionStr = fileVersion;

	Level0FileHeader	Head0;
	Level1FileHeader	Head1;

	if (id == LEVEL0_4CC)
	{
		Head0 = *(Level0FileHeader*)&allData[0];

		myTerrain->TerrainPointDataOffset = Head0.TerrainPointDataOffset;
		myTerrain->width = Head0.Width;
		myTerrain->height = Head0.Height;
		myTerrain->HighestPoint = Head0.HighestPoint;
		myTerrain->LowestPoint = 0.0f;

		myTerrain->LandPaletteOffset = Head0.LandPaletteOffset;
		myTerrain->ObjectListOffset = Head0.ObjectListOffset;
		myTerrain->ModelListOffset = Head0.ModelListOffset;
		myTerrain->LevelConfigOffset = Head0.LevelConfigOffset;
		myTerrain->EndOfLastBit = Head0.EndOfLastBit;
	}
	else // if (id == LEVEL1_4CC)
	{
		Head1 = *(Level1FileHeader*)&allData[0];

		myTerrain->TerrainPointDataOffset = Head1.TerrainPointDataOffset;
		myTerrain->width = Head1.Width;
		myTerrain->height = Head1.Height;
		myTerrain->HighestPoint = Head1.HighestPoint;
		myTerrain->LowestPoint = Head1.LowestPoint;

		myTerrain->LandPaletteOffset = Head1.LandPaletteOffset;
		myTerrain->ObjectListOffset = Head1.ObjectListOffset;
		myTerrain->ModelListOffset = Head1.ModelListOffset;
		myTerrain->LevelConfigOffset = Head1.LevelConfigOffset;
		myTerrain->EndOfLastBit = Head1.EndOfLastBit;
	}

	cursor = myTerrain->TerrainPointDataOffset;

	// Load in the landscape
	myTerrain->terrainPoints.resize(myTerrain->height);
	for (unsigned long i = 0; i < myTerrain->height; i++)
	{
		myTerrain->terrainPoints[i].resize(myTerrain->width);
		for (unsigned long j = 0; j < myTerrain->width; j++)
		{
			TerrainPoint* TPoint = new TerrainPoint();

			switch (id)
			{
			case LEVEL0_4CC:
				ReadTerrainPoint_Format0(allData, TPoint, cursor);
				break;
			case LEVEL1_4CC:
				ReadTerrainPoint_Format1(allData, TPoint, cursor);
				break;
			}

			myTerrain->terrainPoints[i][j] = TPoint;
		}
	}

	if (myTerrain->ObjectListOffset != 0 && myTerrain->ModelListOffset != 0)
	{
		cursor = myTerrain->ObjectListOffset;
		size_t listSize = myTerrain->ModelListOffset - myTerrain->ObjectListOffset;
		myTerrain->objectListData.resize(listSize);
		memcpy(myTerrain->objectListData.data(), &allData[cursor], listSize);
	}

	if (myTerrain->ModelListOffset != 0 && myTerrain->LandPaletteOffset != 0)
	{
		cursor = myTerrain->ModelListOffset;
		size_t listSize = myTerrain->LandPaletteOffset - myTerrain->ModelListOffset;
		myTerrain->modelListData.resize(listSize);
		memcpy(myTerrain->modelListData.data(), &allData[cursor], listSize);
	}

	size_t colSize = sizeof(Colour);
	size_t colsSize = colSize * 256;
	cursor = myTerrain->LandPaletteOffset;
	myTerrain->colours.resize(256);

	for (size_t i = 0; i < myTerrain->colours.size(); i++)
	{
		myTerrain->colours[i] = *(Colour*)&allData[cursor];
		cursor += 12;
	}

	if (myTerrain->LevelConfigOffset != 0 && myTerrain->EndOfLastBit != 0)
	{
		cursor = myTerrain->LevelConfigOffset;
		size_t listSize = myTerrain->EndOfLastBit - myTerrain->LevelConfigOffset;
		myTerrain->configData.resize(listSize);
		memcpy(myTerrain->configData.data(), &allData[cursor], listSize);
	}

	return true;
}

HWTerrain* GetHWTerrain()
{
	return myTerrain;
}

void SetHWTerrain(HWTerrain* terr)
{
	if (myTerrain != nullptr)
		delete myTerrain;
	myTerrain = terr;
}