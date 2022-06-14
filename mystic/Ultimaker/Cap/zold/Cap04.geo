// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=0.4;
Mesh.MeshSizeMax=0.4;

//Mesh.SubdivisionAlgorithm=2;

ri=43.34-0.2;
ro=44.84-0.2;

Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 6.5,ro, 2*Pi};

Cylinder(2) = {0.0, 0.0, 1.0, 0, 0, 6.5, ri, 2*Pi};

//BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Cylinder(4) = {0.0, 0.0, 6.5-1.68, 0, 0, 1.68, ri, 2*Pi};

Cylinder(5) = {0.0, 0.0, 6.5-1.68, 0, 0, 1.68, ri-0.4, 2*Pi};

BooleanDifference(6)={Volume{4}; Delete;}{Volume{5}; Delete;};

Box(7) = {0,0,6.5-2,  ro,ro,2};

Box(8) = {ro-10,ro-10,6.5-2,  10,10,2};

BooleanDifference(9)={Volume{7}; Delete;}{Volume{8}; Delete;};

Cylinder(10) = {ro-10, ro-10, 6.5-2, 0, 0, 2, 10, 2*Pi};

BooleanUnion(11)={ Volume{9};  Delete; }{Volume{1,10};  Delete;};

BooleanDifference(12)={Volume{11}; Delete;}{Volume{2}; Delete;};

Cylinder(14) = {ro-8, ro-8, 6.5-2, 0, 0, 2, 3, 2*Pi};

BooleanDifference(15)={Volume{12}; Delete;}{Volume{14}; Delete;};
