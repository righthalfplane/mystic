// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Box(1) = {0,0,0, 40,20,2};

//Mesh.MeshSizeMin=0.3;
//Mesh.MeshSizeMax=0.3;

Mesh.CharacteristicLengthMax = 0.5;
Mesh.CharacteristicLengthMin = 0.5;

//Mesh.SubdivisionAlgorithm=2;

Box(2) = {5,5,0, 30,10,2};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};
