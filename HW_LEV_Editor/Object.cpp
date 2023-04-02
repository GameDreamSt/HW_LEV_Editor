
#include <Object.h>

Quad::Quad()
{
	vertIds[0] = vertIds[1] = vertIds[2] = vertIds[3] = -1;
	texVertIds[0] = texVertIds[1] = texVertIds[2] = texVertIds[3] = -1;

	memset(&normals, 0, 12);

	textureName = "";
	glow = 0;

	opacityMode = "";
	opacityStrength = 0;

	enviroMapTex = "";
	enviroMapStrength = 0;
}

Tris::Tris()
{
	texVertIds[0] = -1;

	textureName = "";
	glow = 0;

	opacityMode = "";
	opacityStrength = 0;

	enviroMapTex = "";
	enviroMapStrength = 0;
}

ObjObject::ObjObject()
{
	name = "";
}

ObjObject::ObjObject(string name)
{
	this->name = name;
}
