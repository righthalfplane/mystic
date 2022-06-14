// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");



Mesh.MeshSizeMin=0.2;
Mesh.MeshSizeMax=0.2;

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;

Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 40, 7.5, 2*Pi};

Cylinder(2) = {0.0, 0.0, 5.0, 0, 0, 50, 3.175, 2*Pi};

//BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Cylinder(4) = {-20.0, 0.0, 0.0, 20, 0, 14.14*2, 5.175, 2*Pi};

Cylinder(5) = {20.0, 0.0, 0.0, -20, 0, 14.14*2, 5.175, 2*Pi};

Cylinder(6) = {0.0, -20.0, 0.0, 0.0, 20, 14.14*2, 5.175, 2*Pi};

Cylinder(7) = { 0.0, 20.0, 0.0, 0.0,-20, 14.14*2, 5.175, 2*Pi};

BooleanUnion(8)={ Volume{1};  Delete; }{Volume{4,5,6,7};  Delete;};

BooleanDifference(9)={Volume{8}; Delete;}{ Volume{2}; Delete;};

Cylinder(14) = {-20.0, 0.0, 0.0, 20, 0, 14.14*2, 3.175, 2*Pi};

Cylinder(15) = {20.0, 0.0, 0.0, -20, 0, 14.14*2, 3.175, 2*Pi};

Cylinder(16) = {0.0, -20.0, 0.0, 0.0, 20, 14.14*2, 3.175, 2*Pi};

Cylinder(17) = { 0.0, 20.0, 0.0, 0.0,-20, 14.14*2, 3.175, 2*Pi};

BooleanDifference(19)={Volume{9}; Delete;}{ Volume{14,15,16,17}; Delete;};



//BooleanUnion(28)={ Volume{3};  Delete; }{Volume{4,5,6,7};  Delete;};

//BooleanUnion(28)={ Volume{3,4,5,6,7};  Delete; }{};

//f() = BooleanFragments { Volume{1,4,5,6,7};  Delete; }{};

//BooleanDifference(33)={Volume{f()}; Delete;}{Surface{2}; Delete;};

//Cylinder(6) = {-15.0, 0.0, 5.82,   30, 0.0, 0, 2.2, 2*Pi};

//Cylinder(7) = {-15.0, 0.0, 78.5,   30, 0.0, 0, 2.2, 2*Pi};

//BooleanDifference(8)={Volume{5}; Delete;}{Volume{6};Volume{7}; Delete;};

//Cylinder(9) = {0.0, 0.0, 14.0, 0, 0, 3, 9.7, 2*Pi};

//Cylinder(10) = {0.0, 0.0, 65.0, 0, 0, 3, 9.7, 2*Pi};

//Box(12) = {7.5,-1.0,0.0,    3,2,95};

//Box(14) = {7.5,-1.0,0.0,    3,2,95};

//BooleanDifference(15)={Volume{9}; Delete;}{Volume{12}; Delete;};

//BooleanDifference(16)={Volume{10}; Delete;}{Volume{14}; Delete;};

//f() = BooleanFragments { Volume{8,15,16};  Delete; }{};

