// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=0.4;
Mesh.MeshSizeMax=0.4;

//Mesh.SubdivisionAlgorithm=2;

ri=43.34-0.2;
ro=44.84-0.2;
h=5.5-2;
h=0;

Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 6.6,ro, 2*Pi};

Cylinder(2) = {0.0, 0.0, 0.0, 0, 0, 5.6, ri, 2*Pi};

//BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Box(7) = {0,0,h,  ro,ro,2};

Box(8) = {ro-10,ro-10,h,  10,10,2};

BooleanDifference(9)={Volume{7}; Delete;}{Volume{8}; Delete;};

Cylinder(10) = {ro-10, ro-10, h, 0, 0, 2, 10, 2*Pi};

BooleanUnion(11)={ Volume{9};  Delete; }{Volume{1,10};  Delete;};

BooleanDifference(12)={Volume{11}; Delete;}{Volume{2}; Delete;};

Cylinder(14) = {ro-8, ro-8, h, 0, 0, 2, 3, 2*Pi};

BooleanDifference(15)={Volume{12}; Delete;}{Volume{14}; Delete;};

Cone(16) = {0,0,6.6, 0.,0,1, ro,0};

BooleanUnion(17)={ Volume{15};  Delete; }{Volume{16};  Delete;};
