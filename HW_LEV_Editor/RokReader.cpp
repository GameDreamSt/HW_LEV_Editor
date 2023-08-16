
#include <RokReader.h>

bool ReadBytes(string filePath, vector<unsigned char>& data)
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

template <typename T>
T Read4B(vector<unsigned char> &data, size_t &ptr)
{
	T ul = *(T*)&data.data()[ptr];
	ptr += 4;
	return ul;
}

bool ImportStrataFromRok(string path)
{
	size_t pointer = 0;
	vector<unsigned char> data;

	HWTerrain* myTerrain = HWTerrain::myTerrain;

	if (myTerrain == nullptr)
	{
		cout << "My terrain is not loaded? wtf";
		return false;
	}

	if (!ReadBytes(path, data))
	{
		cout << "Failed to read the file!";
		return false;
	}

	myTerrain->strataLayers.clear();
	myTerrain->strataLayers.resize(myTerrain->materialCount);
	for (size_t i = 0; i < myTerrain->materialCount; i++)
	{
		StrataLayerList layerList;
		layerList.layerCount = Read4B<unsigned long>(data, pointer);
		layerList.maxDepth = Read4B<float>(data, pointer);

		layerList.layerList.resize(layerList.layerCount);
		for (unsigned long j = 0; j < layerList.layerCount; j++)
		{
			layerList.layerList[j] = StrataLayer(1.0f);

			StrataLayer* layer = &layerList.layerList[j];
			layer->depth = Read4B<float>(data, pointer);
			layer->x = Read4B<unsigned long>(data, pointer);
			layer->z = Read4B<unsigned long>(data, pointer);
			layer->resilience = Read4B<float>(data, pointer);

			layer->matList.resize(layer->x * (size_t)layer->z);
			size_t size = sizeof(unsigned char) * layer->matList.size();
			memcpy(layer->matList.data(), &data[pointer], size);
			pointer += size;

			layerList.layerList[j] = *layer;
		}

		myTerrain->strataLayers[i] = layerList;
	}

	return true;
}
