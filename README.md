# Hostile Waters .LEV File Editor
Uses Visual studio 2019

## Features:
1. Export to .lev
2. Export to .obj wavefront
3. Export level to .tga Truevision image heightmap
4. Import from .obj wavefront

## Note
It uses an existing and working .lev file to make modifications to it. This editor won't allow you to make custom levels or extend/reduce the size of the map.

## Workflow
To modify the heights of the terrain you need to:
1. export the terrain to .obj
2. import it to a model editing software like blender
3. modify the heights by hand or by sculpting
4. export from blender to .obj
5. import that modified .obj back to .lev editor
6. save .lev file
