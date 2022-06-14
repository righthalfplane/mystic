// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Box(1) = {0,0,0, 1,1,10};

Mesh.MeshSizeMin=0.3;
Mesh.MeshSizeMax=0.3;

Mesh.SubdivisionAlgorithm=2;

Box(2) = {0.125,0.125,0, 1.75,1.75,10};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};
