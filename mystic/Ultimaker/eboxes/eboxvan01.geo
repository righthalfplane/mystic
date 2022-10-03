// Gmsh project created on Fri May 28 11:18:05 2021
//
// Good for One color at 0.2
//
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=0.4;
Mesh.MeshSizeMax=0.4;

//Mesh.SubdivisionAlgorithm=2;

t=4;

l=280;


Box(1) = {0,0,0,  40,t,10};


Box(2) = {40-t,-40,0,  t,40+t,10};


Rotate{{0,0,1}, {40,0,0},Pi/8}{ Volume{2}; }


BooleanUnion(5)={ Volume{1};  Delete; }{Volume{2};  Delete;};


//BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};


//Box(4) = {35,4.5,0,  40,4,10};

//BooleanDifference(5)={Volume{3}; Delete;}{Volume{4}; Delete;};

