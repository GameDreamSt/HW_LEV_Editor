
#include <ObjWriter.h>

string GetFileName(string const& path)
{
    return path.substr(path.find_last_of("/\\") + 1);
}

void WriteObjFile(ObjObject &object, string filename, string objFilePath, string texturesFilepath)
{
    try
    {
        string output = "";

        bool hasAnyTextures = object.textureFilenames.size() > 0;

        if (hasAnyTextures)
        {
            vector<string> textures = vector<string>();

            textures.insert(textures.end(), object.textureFilenames.begin(), object.textureFilenames.end());

            vector<string>::iterator it = unique(textures.begin(), textures.end());
            textures.resize(std::distance(textures.begin(), it));

            for (int i = 0; i < textures.size(); i++)
            {
                output += "newmtl material_" + filename + '\n';
                output += "Ns 0.000000\nKa 1.000000 1.000000 1.000000\nKd 1.000000 1.000000 1.000000\nKs 0.000000 0.000000 0.000000\nKe 0.000000 0.000000 0.000000\nNi 1.450000\nd 1.000000\nillum 1\n";
                output += "map_Kd " + filename + ".png\n\n";
            }

            string mtlPath = objFilePath.substr(0, objFilePath.length() - 3) + "mtl";
            ofstream writeMtl(mtlPath);
            writeMtl << output;
            writeMtl.close();

            output = "mtllib " + GetFileName(mtlPath) + "\n";
        }

        output += "o " + object.name + "\n";

        // verts info
        for (int j = 0; j < object.verts.size(); j++)
        {
            object.verts[j] += object.position;

            output += "v " + object.verts[j].ToString() + "\n";
        }

        // texture verts
        for (int j = 0; j < object.texVerts.size(); j++)
            output += "vt " + to_string(object.texVerts[j].first) + " " + to_string(object.texVerts[j].second) + "\n";

        if (hasAnyTextures)
        {
            string name = object.textureFilenames[0];
            output += "usemtl material_" + filename + "\n";
        }

        // ***********
        // Faces

        // Quads
        for (int j = 0; j < object.quads.size(); j++)
        {
            if (object.quads[j].texVertIds[0] == -1) // No textures...
            {
                output += "f " + to_string(object.quads[j].vertIds[0]) +
                    " " + to_string(object.quads[j].vertIds[1]) +
                    " " + to_string(object.quads[j].vertIds[3]) +
                    " " + to_string(object.quads[j].vertIds[2]) +
                    "\n";
            }
            else
            {
                output += "f " + to_string(object.quads[j].vertIds[0]) + "/" + to_string(object.quads[j].texVertIds[0]) +
                    " " + to_string(object.quads[j].vertIds[1]) + "/" + to_string(object.quads[j].texVertIds[1]) +
                    " " + to_string(object.quads[j].vertIds[3]) + "/" + to_string(object.quads[j].texVertIds[3]) +
                    " " + to_string(object.quads[j].vertIds[2]) + "/" + to_string(object.quads[j].texVertIds[2]) +
                    "\n";
            }
        }

        // Tris
        for (int j = 0; j < object.tris.size(); j++)
        {
            if (object.tris[j].texVertIds[0] == -1) // No textures...
            {
                output += "f " + to_string(object.tris[j].vertIds[0]) +
                    " " + to_string(object.tris[j].vertIds[1]) +
                    " " + to_string(object.tris[j].vertIds[2]) +
                    "\n";
            }
            else
            {
                output += "f " + to_string(object.tris[j].vertIds[0]) + "/" + to_string(object.tris[j].texVertIds[0]) +
                    " " + to_string(object.tris[j].vertIds[1]) + "/" + to_string(object.tris[j].texVertIds[1]) +
                    " " + to_string(object.tris[j].vertIds[2]) + "/" + to_string(object.tris[j].texVertIds[2]) +
                    "\n";
            }


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