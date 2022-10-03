// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Box(1) = {0,0,0, 1,1,10};

Mesh.MeshSizeMin=0.6;
Mesh.MeshSizeMax=0.6;

Mesh.SubdivisionAlgorithm=2;

Box(2) = {0.25,0.25,0, 0.5,0.5,10};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};
