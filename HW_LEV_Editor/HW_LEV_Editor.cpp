
#include <iostream>
#include <string>

#include <Files.h>
#include <LevReader.h>
#include <RokReader.h>

#include <TerrainExporter.h>
#include <TerrainImporter.h>

using namespace std;

bool needsToSave = false;

void EditorExportLevelToLev(string path)
{
	string dir = RemovePathLast(path);
	string filename = RemoveExtension(GetFileName(path));

	ExportTerrainToLev(*HWTerrain::myTerrain, dir + "\\" + filename + "NEW.lev");
	cout << ".lev Export complete\n";
	needsToSave = false;
	system("pause");
}

void EditorExportLevelToObj(string path, string filename)
{
	string terrainTextureTilePath = "";
	
	string choice;
	cout << "Use texture? [Y/N]\n";
	cin >> choice;

	if (choice[0] == 'Y' || choice[0] == 'y')
	{
		cout << "Drop a texture file:\n";
		cin.ignore();
		getline(cin, choice);
		terrainTextureTilePath = RemoveQuotes(choice);
	}

	ExportTerrainToObj(*HWTerrain::myTerrain, path, filename, terrainTextureTilePath);
	cout << ".obj Export complete\n";
	system("pause");
}

void EditorExportLevelImageHeight(string path)
{
	ExportImageHeight(*HWTerrain::myTerrain, path);
	cout << ".tga Export complete\n";
	system("pause");
}

void EditorImportLevelImageHeight()
{
	string tgaPath = "";
	
	cout << "Drop a .tga file to import the heights\n";

	cin.ignore();
	getline(cin, tgaPath);
	tgaPath = RemoveQuotes(tgaPath);

	bool success = ImportFromTga(HWTerrain::myTerrain, tgaPath);

	if (success)
		cout << ".tga import complete!\n";
	system("pause");
}

void ExportLevelImageTiles(string path)
{
	ExportImageTiles(*HWTerrain::myTerrain, path);
	cout << ".tga Export complete\n";
	system("pause");
}

void EditorImportLevel(string path)
{
	path = RemoveQuotes(path);

	ImportFromObj(HWTerrain::myTerrain, path);

	needsToSave = true;
	system("pause");
}

void EditorImportLevel()
{
	string path;

	cout << "Drop a file to import the .obj\n";
	cin.ignore();
	getline(cin, path);

	EditorImportLevel(path);
}

void EditorImportStrata()
{
	string path = "";

	cout << "Drop the .rok strata file\n";
	cin.ignore();
	getline(cin, path);

	ImportStrataFromRok(path);

	needsToSave = true;
	system("pause");
}

void PrintCredits()
{
	system("cls");
	cout << "Made by Game Dream St.\nJoin our Hostile Waters Community Discord server! https://discord.gg/sqsKRw4 \n";
	system("pause");
}

int main(int argc, char* argv[])
{
	string path = "";

	// Command line
	if (argc > 1)
		path = argv[1];

	bool editing = true;

	if (path == "")
	{
		typeFilename:;

		printf("What .lev file do you want to read?\n(Enter absolute or relative path. You can also drag and drop the file)\n");

		string fileName;
		getline(std::cin, fileName);

		fileName = RemoveQuotes(fileName);

		if (!IsAPath(fileName))
			path = RemovePathLast(string(argv[0])) + "\\" + fileName;
		else
			path = fileName;
	}
	else
		path = RemoveQuotes(path);

	if (!ReadLevFile(path))
	{
		system("cls");
		goto typeFilename;
	}
	/*else // For debugging
	{
		string dir = RemovePathLast(path);
		string filename = RemoveExtension(GetFileName(path));

		//EditorImportLevel(dir + "\\level1E.obj");
		//ExportLevelImage(dir + "\\" + RemoveExtension(GetFileName(path)));
		//EditorExportLevelToLev(dir + "\\" + filename + ".lev");
		//EditorImportStrata();
		//EditorExportLevelToObj(dir, filename);
		//EditorExportLevelImageHeight(RemovePathLast(path) + "\\" + RemoveExtension(GetFileName(path)));
		EditorImportLevelImageHeight();
		EditorExportLevelToLev(path);
		//return 0;
	}*/

	string file = GetFileName(path);

	do
	{
		editing = true;
		system("cls");

		if (needsToSave)
			cout << "Don't forget to save!\n\n";

		cout << "Curently editing " << file << '\n'
			<< "What would you like to do?\n"
			<< "1. Export level to .lev\n"
			<< "2. Export level to .obj\n"
			<< "3. Export level to .tga heightmap\n"
			<< "4. Export level to .tga tilemap (just for show)\n"
			<< "5. Import level from .obj\n"
			<< "6. Import strata info from .rok (not used right now)\n"
			<< "7. Modify terrain with .tga heightmap\n"
			<< "8. Credits\n"
			<< "9. Quit\n";

		int choice = -1;
		cin >> choice;

		if (choice == 1)
			EditorExportLevelToLev(path);
		else if (choice == 2)
			EditorExportLevelToObj(RemovePathLast(path), RemoveExtension(file));
		else if (choice == 3)
			EditorExportLevelImageHeight(RemovePathLast(path) + "\\" + RemoveExtension(GetFileName(path)));
		else if (choice == 4)
			ExportLevelImageTiles(RemovePathLast(path) + "\\" + RemoveExtension(GetFileName(path)));
		else if (choice == 5)
			EditorImportLevel();
		else if (choice == 6)
			EditorImportStrata();
		else if (choice == 7)
			EditorImportLevelImageHeight();
		else if (choice == 8)
			PrintCredits();
		else
			editing = false;

	} while (editing);

	cout << "Quitting...\n";

	return 0;
}