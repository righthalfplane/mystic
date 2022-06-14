// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");



Mesh.MeshSizeMin=0.2;
Mesh.MeshSizeMax=0.2;

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;

r2=Sqrt(2.0);

r=1.375;

Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 32, 7.5, 2*Pi};

Cylinder(2) = {0.0, 0.0, 4.0, 0, 0, 50, 3.175, 2*Pi};

//BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Cylinder(4) = {-20.0, 0.0, 0.0, 20, 0, 14.14*2, 5.175, 2*Pi};

Cylinder(5) = {20.0, 0.0, 0.0, -20, 0, 14.14*2, 5.175, 2*Pi};

Cylinder(6) = {0.0, -20.0, 0.0, 0.0, 20, 14.14*2, 5.175, 2*Pi};

Cylinder(7) = { 0.0, 20.0, 0.0, 0.0,-20, 14.14*2, 5.175, 2*Pi};

BooleanUnion(8)={ Volume{1};  Delete; }{Volume{4,5,6,7};  Delete;};

BooleanDifference(9)={Volume{8}; Delete;}{ Volume{2}; Delete;};

Cylinder(14) = {-20.001, 0.0, 0.0, 20/r2, 0, 20, 3.175, 2*Pi};

Cylinder(15) = {20.001, 0.0, 0.0, -20/r2, 0,    20, 3.175, 2*Pi};

Cylinder(16) = {0.0, -20.001, 0.0, 0.0, 20/r2,  20, 3.175, 2*Pi};

Cylinder(17) = { 0.0, 20.001, 0.0, 0.0,-20/r2,  20, 3.175, 2*Pi};

BooleanDifference(19)={Volume{9}; Delete;}{ Volume{14,15,16,17}; Delete;};

Cylinder(20) = {-8.0, -8.0, 7.0,   16, 16.0, 0,r, 2*Pi};

BooleanDifference(21)={Volume{19}; Delete;}{ Volume{20}; Delete;};

Cylinder(22) = {-15.0,  -8.0, 7.0,   0, 16.0, 0,r, 2*Pi};

BooleanDifference(23)={Volume{21}; Delete;}{ Volume{22}; Delete;};

Cylinder(24) = {-8.0,  -15.0, 7.0,   16.0, 0.0, 0,r, 2*Pi};

BooleanDifference(25)={Volume{23}; Delete;}{ Volume{24}; Delete;};

Cylinder(26) = {-8.0,  15.0, 7.0,   16.0, 0.0, 0,r, 2*Pi};

BooleanDifference(27)={Volume{25}; Delete;}{ Volume{26}; Delete;};

Cylinder(28) = {15.0,  -8.0, 7.0,   0, 16.0, 0,r, 2*Pi};

BooleanDifference(29)={Volume{27}; Delete;}{ Volume{28}; Delete;};

