// Gmsh project created on Fri May 28 11:18:05 2021
//
// Good for One color at 0.2
//
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=0.4;
Mesh.MeshSizeMax=0.4;

//Mesh.SubdivisionAlgorithm=2;

t=3;

l=280;


Box(1) = {0,0,0,  40,t,l/2};

Box(2) = {40-t,-40,0,  t,40+t,l/2-20};

Box(3) = {0,-20,l/2-20,  t,20+t,20};

Rotate{{0,0,1}, {40,t,0},Pi/8}{ Volume{2}; }


BooleanUnion(6)={ Volume{1};  Delete; }{Volume{2,3};  Delete;};


//BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};


//Box(4) = {35,4.5,0,  40,4,10};

//BooleanDifference(5)={Volume{3}; Delete;}{Volume{4}; Delete;};

