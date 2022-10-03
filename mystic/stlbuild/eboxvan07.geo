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


Merge "vanLeft02.stl";

//Surface Loop(1) = {1};

//Volume(1) = {1};

//BooleanUnion(6)={ Volume{1};  Delete; }{Volume{"vanLeft02.stl"};  Delete;};

BooleanUnion(6)={ Volume{0};  Delete; }{Volume{1,2,3};  Delete;};



//BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};


//Box(4) = {35,4.5,0,  40,4,10};

//BooleanDifference(5)={Volume{3}; Delete;}{Volume{4}; Delete;};


