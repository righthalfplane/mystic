// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=2.0;
Mesh.MeshSizeMax=2.0;

//Mesh.SubdivisionAlgorithm=2;

r1=70/2;

r2=80/2;


//BooleanUnion(17)={ Volume{3};  Delete; }{Volume{4};  Delete;};

Cone(1) = {0,0,0, 0.,0,60, r1,r2};

Cone(2) = {0,0,0, 0.,0,60, r1+2,r2+2};

Box(4) = {-21-(r2+2),-10,58, 21+2,20,2};

Box(5) = {-23-(r2+2),-10,0, 2,20,60};

BooleanUnion(7)={ Volume{2};  Delete; }{Volume{4,5};  Delete;};

BooleanDifference(8)={Volume{7}; Delete;}{Volume{1}; Delete;};

