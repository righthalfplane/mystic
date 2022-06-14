// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Box(1) = {0,0,0, 40,20,10};

//Mesh.MeshSizeMin=0.3;
//Mesh.MeshSizeMax=0.3;

Mesh.SubdivisionAlgorithm=2;

Box(2) = {4,4,4, 32,12,20};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};
