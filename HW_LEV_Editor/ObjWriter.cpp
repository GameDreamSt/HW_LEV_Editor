
#include <ObjWriter.h>

bool HasAnyTextures(vector<ObjObject>& objects)
{
    for (int i = 0; i < objects.size(); i++)
        if (objects[i].textureFilenames.size() > 0)
            return true;

    return false;
}

string GetFileName(string const& path)
{
    return path.substr(path.find_last_of("/\\") + 1);
}

void WriteObjFile(vector<ObjObject> objects, string objFilePath, string texturesFilepath)
{
    try
    {
        string output = "";

        int vertCounter = 0;
        int texVertCounter = 0;

        bool hasAnyTextures = HasAnyTextures(objects);

        if (hasAnyTextures)
        {
            vector<string> textures = vector<string>();

            for (int i = 0; i < objects.size(); i++)
                textures.insert(textures.end(), objects[i].textureFilenames.begin(), objects[i].textureFilenames.end());

            vector<string>::iterator it = unique(textures.begin(), textures.end());
            textures.resize(std::distance(textures.begin(), it));

            for (int i = 0; i < textures.size(); i++)
            {
                output += "newmtl material_" + textures[i].substr(0, textures[i].length() - 4) + '\n';
                output += "Ns 0.000000\nKa 1.000000 1.000000 1.000000\nKd 1.000000 1.000000 1.000000\nKs 0.000000 0.000000 0.000000\nKe 0.000000 0.000000 0.000000\nNi 1.450000\nd 1.000000\nillum 1\n";
                output += "map_Kd " + texturesFilepath + textures[i].substr(0, textures[i].length() - 4) + ".png\n\n";
            }

            string mtlPath = objFilePath.substr(0, objFilePath.length() - 3) + "mtl";
            ofstream writeMtl(mtlPath);
            writeMtl << output;
            writeMtl.close();

            output = "mtllib " + GetFileName(mtlPath) + "\n";
        }

        for (int i = 0; i < objects.size(); i++)
        {
            // verts info
            for (int j = 0; j < objects[i].verts.size(); j++)
            {
                //objects[i].verts[j].RotateVector(objects[i].rotMatrix);
                objects[i].verts[j] += objects[i].position;

                output += "v " + objects[i].verts[j].ToString() + "\n";
            }

            // texture verts
            for (int j = 0; j < objects[i].texVerts.size(); j++)
                output += "vt " + to_string(objects[i].texVerts[j].first) + " " + to_string(objects[i].texVerts[j].second) + "\n";
        }

        for (int i = 0; i < objects.size(); i++)
        {
            output += "o " + objects[i].name + "\n";

            if (hasAnyTextures && objects[i].textureFilenames.size() > 0)
            {
                string name = objects[i].textureFilenames[0];
                output += "usemtl material_" + name.substr(0, name.length() - 4) + "\n";
            }

            // *****
            // faces

            // quads
            for (int j = 0; j < objects[i].quads.size(); j++)
            {
                if (objects[i].quads[j].texVertIds[0] == -1) // No textures...
                {
                    output += "f " + to_string(objects[i].quads[j].vertIds[0] + vertCounter) +
                        " " + to_string(objects[i].quads[j].vertIds[1] + vertCounter) +
                        " " + to_string(objects[i].quads[j].vertIds[3] + vertCounter) +
                        " " + to_string(objects[i].quads[j].vertIds[2] + vertCounter) +
                        "\n";
                }
                else
                {
                    output += "f " + to_string(objects[i].quads[j].vertIds[0] + vertCounter) + "/" + to_string(objects[i].quads[j].texVertIds[0] + texVertCounter) +
                        " " + to_string(objects[i].quads[j].vertIds[1] + vertCounter) + "/" + to_string(objects[i].quads[j].texVertIds[1] + texVertCounter) +
                        " " + to_string(objects[i].quads[j].vertIds[3] + vertCounter) + "/" + to_string(objects[i].quads[j].texVertIds[3] + texVertCounter) +
                        " " + to_string(objects[i].quads[j].vertIds[2] + vertCounter) + "/" + to_string(objects[i].quads[j].texVertIds[2] + texVertCounter) +
                        "\n";
                }
            }

            // tris
            for (int j = 0; j < objects[i].tris.size(); j++)
            {
                if (objects[i].tris[j].texVertIds[0] == -1) // No textures...
                {
                    output += "f " + to_string(objects[i].tris[j].vertIds[0] + vertCounter) +
                        " " + to_string(objects[i].tris[j].vertIds[1] + vertCounter) +
                        " " + to_string(objects[i].tris[j].vertIds[2] + vertCounter) +
                        "\n";
                }
                else
                {
                    output += "f " + to_string(objects[i].tris[j].vertIds[0] + vertCounter) + "/" + to_string(objects[i].tris[j].texVertIds[0] + texVertCounter) +
                        " " + to_string(objects[i].tris[j].vertIds[1] + vertCounter) + "/" + to_string(objects[i].tris[j].texVertIds[1] + texVertCounter) +
                        " " + to_string(objects[i].tris[j].vertIds[2] + vertCounter) + "/" + to_string(objects[i].tris[j].texVertIds[2] + texVertCounter) +
                        "\n";
                }


            }

            //vertCounter += (int)objects[i].verts.size();
            texVertCounter += (int)objects[i].texVerts.size();
        }

        ofstream write(objFilePath);
        write << output;
        write.close();
    }
    catch (...)
    {
        printf("Failed to write to file! \n ");
    }
}