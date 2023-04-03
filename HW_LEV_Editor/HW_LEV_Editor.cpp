
#include <iostream>
#include <string>

#include <Files.h>
#include <LevReader.h>

#include <TerrainExporter.h>
#include <TerrainImporter.h>

using namespace std;

bool needsToSave = false;

void EditorExportLevelToLev(string path)
{
	string dir = RemovePathLast(path);
	string filename = RemoveExtension(GetFileName(path));

	ExportTerrainToLev(*GetHWTerrain(), dir + "\\" + filename + "NEW.lev");
	cout << ".lev Export complete\n";
	needsToSave = false;
	system("pause");
}

void EditorExportLevelToObj(string path, string filename)
{
	ExportTerrainToObj(*GetHWTerrain(), path, filename);
	cout << ".obj Export complete\n";
	system("pause");
}

void ExportLevelImage(string path)
{
	ExportImage(*GetHWTerrain(), path);
	cout << ".tga Export complete\n";
	system("pause");
}

void EditorImportLevel(string path)
{
	path = RemoveQuotes(path);

	ImportFromObj(GetHWTerrain(), path);
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
	else
	{
		/*string dir = RemovePathLast(path);
		string filename = RemoveExtension(GetFileName(path));

		//EditorImportLevel(dir + "\\level1E.obj");
		//ExportLevelImage(dir + "\\" + RemoveExtension(GetFileName(path)));
		EditorExportLevelToLev(dir + "\\" + filename + ".lev");
		return 0;*/
	}

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
			<< "4. Import level from .obj\n"
			<< "5. Credits\n"
			<< "6. Quit\n";

		int choice = -1;
		cin >> choice;

		if (choice == 1)
			EditorExportLevelToLev(path);
		else if (choice == 2)
			EditorExportLevelToObj(RemovePathLast(path), RemoveExtension(file));
		else if (choice == 3)
			ExportLevelImage(RemovePathLast(path) + "\\" + RemoveExtension(GetFileName(path)));
		else if (choice == 4)
			EditorImportLevel();
		else if (choice == 5)
			PrintCredits();
		else
			editing = false;

	} while (editing);

	cout << "Quitting...\n";

	return 0;
}