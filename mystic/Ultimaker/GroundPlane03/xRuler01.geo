// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;


Box(1) = {0,0,0, 20,100,6};


Box(2) = {20,40,0, 28,20,4};

Box(3) = {48,40,0, 37,20,2};


BooleanUnion(8)={ Volume{1};  Delete; }{Volume{2,3};  Delete;};
