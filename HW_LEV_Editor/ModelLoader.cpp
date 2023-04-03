
#include <ModelLoader.h>

int FaceVertCount(string& str)
{
	int spaces = 1;

	for (size_t i = 0; i < str.size(); i++)
		if (str[i] == ' ')
			spaces++;

	return spaces;
}

int FaceMode(string& str)
{
	int mode = 0;

	for (size_t i = 0; i < str.size(); i++)
		if (str[i] == '/')
			mode++;

	return mode;
}

void GetTwoNums(string& str, unsigned int& vert, int& tex)
{
	vert = tex = 0;
	for (size_t i = 0; i < str.length(); i++)
		if (str[i] == '/')
		{
			vert = stoi(str.substr(0, i)) - 1;
			tex = stoi(str.substr(i + 1, str.length() - i - 1)) - 1;
			break;
		}
}

void GetThreeNums(string& str, unsigned int& vert, int& tex, int& norm)
{
	int firstSlash = -1;
	size_t texDigits;
	vert = tex = norm = 0;
	for (size_t i = 0; i < str.length(); i++)
		if (str[i] == '/')
		{
			if (firstSlash == -1)
			{
				firstSlash = (int)i;
				vert = stoi(str.substr(0, i)) - 1;
			}
			else
			{
				texDigits = i - firstSlash - (size_t)1;
				if(texDigits > 0)
					tex = stoi(str.substr(firstSlash + (size_t)1, texDigits)) - 1;
				norm = stoi(str.substr(i + 1, str.length() - i - 1)) - 1;
				break;
			}
		}
}

void ReadFace(string str, vector<Vert>& verts, vector<Vector3>& positions, vector<Vector3>& normals, vector<Vector2>& uvs)
{
	int vertCount = FaceVertCount(str);

	string faceData;
	istringstream ss(str);
	ss >> faceData;

	int faceMode = FaceMode(faceData);

	ss = istringstream(str);

	unsigned int one, two, three, four;
	int texone, textwo, texthree, texfour;
	int normone, normtwo, normthree, normfour;
	Vector3 vec3;

	unsigned int in;
	int tex, norm;
	switch (faceMode)
	{
	case 0: // only vert info
		if (vertCount == 4)
		{
			ss >> one >> two >> three >> four;

			one--;
			two--;
			three--;
			four--;

			verts.push_back(Vert(positions[one], Vector2(), Vector3()));
			verts.push_back(Vert(positions[two], Vector2(), Vector3()));
			verts.push_back(Vert(positions[three], Vector2(), Vector3()));

			verts.push_back(Vert(positions[three], Vector2(), Vector3()));
			verts.push_back(Vert(positions[four], Vector2(), Vector3()));
			verts.push_back(Vert(positions[one], Vector2(), Vector3()));
		}
		else // 3
		{
			ss >> in; verts.push_back(Vert(positions[in], Vector2(), Vector3()));
			ss >> in; verts.push_back(Vert(positions[in], Vector2(), Vector3()));
			ss >> in; verts.push_back(Vert(positions[in], Vector2(), Vector3()));
		}
		break;

	case 1: // vert + texture
		if (vertCount == 4)
		{
			ss >> faceData;
			GetTwoNums(faceData, one, texone);
			ss >> faceData;
			GetTwoNums(faceData, two, textwo);
			ss >> faceData;
			GetTwoNums(faceData, three, texthree);
			ss >> faceData;
			GetTwoNums(faceData, four, texfour);

			verts.push_back(Vert(positions[one], uvs[texone], Vector3()));
			verts.push_back(Vert(positions[two], uvs[textwo], Vector3()));
			verts.push_back(Vert(positions[three], uvs[texthree], Vector3()));

			verts.push_back(Vert(positions[three], uvs[texthree], Vector3()));
			verts.push_back(Vert(positions[four], uvs[texfour], Vector3()));
			verts.push_back(Vert(positions[one], uvs[texone], Vector3()));
		}
		else // 3
		{
			ss >> faceData; GetTwoNums(faceData, in, tex); verts.push_back(Vert(positions[in], uvs[tex], Vector3()));
			ss >> faceData; GetTwoNums(faceData, in, tex); verts.push_back(Vert(positions[in], uvs[tex], Vector3()));
			ss >> faceData; GetTwoNums(faceData, in, tex); verts.push_back(Vert(positions[in], uvs[tex], Vector3()));
		}
		break;

	case 2: // vert + texture + normal
		if (vertCount == 4)
		{
			ss >> faceData;
			GetThreeNums(faceData, one, texone, normone);
			ss >> faceData;
			GetThreeNums(faceData, two, textwo, normtwo);
			ss >> faceData;
			GetThreeNums(faceData, three, texthree, normthree);
			ss >> faceData;
			GetThreeNums(faceData, four, texfour, normfour);

			if (uvs.size() == 0)
				uvs.push_back(Vector2());

			verts.push_back(Vert(positions[one], uvs[texone], normals[normone]));
			verts.push_back(Vert(positions[two], uvs[textwo], normals[normtwo]));
			verts.push_back(Vert(positions[three], uvs[texthree], normals[normthree]));

			verts.push_back(Vert(positions[three], uvs[texthree], normals[normthree]));
			verts.push_back(Vert(positions[four], uvs[texfour], normals[normfour]));
			verts.push_back(Vert(positions[one], uvs[texone], normals[normone]));
		}
		else // 3
		{
			ss >> faceData; GetThreeNums(faceData, in, tex, norm); verts.push_back(Vert(positions[in], uvs[tex], normals[norm]));
			ss >> faceData; GetThreeNums(faceData, in, tex, norm); verts.push_back(Vert(positions[in], uvs[tex], normals[norm]));
			ss >> faceData; GetThreeNums(faceData, in, tex, norm); verts.push_back(Vert(positions[in], uvs[tex], normals[norm]));
		}
		break;

	default: // WTF?
		in = 0;
		break;
	}
}

void ParseMTL(string path, vector<StructMaterial>& materials)
{
	ifstream read(path);
	path = RemovePathLast(path);

	if (!read.is_open())
		return;

	string line;
	while (getline(read, line))
	{
		if (line.rfind("newmtl", 0) != 0)
			continue;

		string name = line.substr(7);

		while (getline(read, line))
			if (line.rfind("map_Kd", 0) == 0)
			{
				string texturePath = path + "\\" + line.substr(7);
				bool found = false;
				for (size_t i = 0; i < materials.size(); i++)
					if (materials[i].texturePath == texturePath)
					{
						found = true;
						break;
					}
				if (!found)
					materials.push_back(StructMaterial(name, texturePath));
				break;
			}
	}

	read.close();
}

vector<ObjObject*> LoadModelsFromFile(string path)
{
	path = RemoveQuotes(path);

	ifstream read(path);

	vector<ObjObject*> loadedModels;
	ObjObject* currentModel = nullptr;

	if (read.fail())
		return loadedModels;

	vector<Vector3> positions;
	vector<Vector3> normals;
	vector<Vector2> uvs;

	vector<Vert> verts;

	Vector2 vec2;
	Vector3 vec3;

	istringstream ss;
	string line;

	string myTexturePath = "";
	vector<StructMaterial> materials;

	string mtllib = "mtllib", usemtl = "usemtl", vn = "vn", vt = "vt";
	string mtlPath, materialName;

	size_t lineLength;
	while (getline(read, line))
	{
		lineLength = line.length();
		if (lineLength <= 2)
			continue;

		switch (line[0])
		{
		default:
			break;

		case 'o':
			if (currentModel != nullptr)
			{
				currentModel->verts = positions;
				loadedModels.push_back(currentModel);
			}

			currentModel = new ObjObject(line.substr(2, lineLength - 2));
			break;

		case 'v':
			switch (line[1])
			{
			case ' ':
				if (currentModel == nullptr)
					break;

				ss = istringstream(line.substr(2));
				ss >> vec3.x >> vec3.y >> vec3.z;
				positions.push_back(vec3);
				break;

			case 'n':
				if (currentModel == nullptr)
					break;

				ss = istringstream(line.substr(2));
				ss >> vec3.x >> vec3.y >> vec3.z;
				normals.push_back(vec3);
				break;

			case 't':
				if (currentModel == nullptr)
					break;

				ss = istringstream(line.substr(2));
				ss >> vec2.x >> vec2.y;
				uvs.push_back(vec2);
				break;

			default:
				break;
			}
			break;

		case 'f':
			if (currentModel == nullptr)
				break;

			ReadFace(line.substr(2), verts, positions, normals, uvs);
			break;

		case 'm': // assume mtlib
			mtlPath = RemovePathLast(path) + "\\" + line.substr(7);
			ParseMTL(mtlPath, materials);
			break;

		case 'u': // assume usemtl
			materialName = line.substr(7);
			for (size_t i = 0; i < materials.size(); i++)
				if (materials[i].name == materialName)
				{
					myTexturePath = materials[i].texturePath;
					break;
				}
			break;
		}
	}

	read.close();

	if (currentModel != nullptr)
	{
		currentModel->verts = positions;
		loadedModels.push_back(currentModel);
	}

	return loadedModels;
}