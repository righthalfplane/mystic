// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");



Mesh.MeshSizeMin=0.2;
Mesh.MeshSizeMax=0.2;

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;

r2=Sqrt(2.0);

r=1.695+0.3; // 0.25 perhaps better - 0.2 was tite - 0.3 was loose

r3=3.175+0.3;

r4=5.175;

l=30;

l2=40;

Cylinder(1) = {0.0, 0.0, -14.0, 0, 0, 42, 7.5, 2*Pi};

Cylinder(2) = {0.0, 0.0,  -5.0, 0, 0, 54, r3, 2*Pi};

//BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Cylinder(4) = {-l2, 0.0, -18.0,  l2,0,l2, r4, 2*Pi};

Cylinder(5) = {l2, 0.0, -18.0,  -l2,0,l2, r4, 2*Pi};

Cylinder(6) = {0.0, -l2, -18.0,  0.0,l2,l2, r4, 2*Pi};

Cylinder(7) = { 0.0, l2, -18.0,  0.0,-l2,l2, r4, 2*Pi};

BooleanUnion(8)={ Volume{1};  Delete; }{Volume{4,5,6,7};  Delete;};

BooleanDifference(9)={Volume{8}; Delete;}{ Volume{2}; Delete;};

Cylinder(14) = {-l2, 0.0, -18.0, l, 0,    l, r3, 2*Pi};

Cylinder(15) = {l2, 0.0, -18.0, -l, 0,    l, r3, 2*Pi};

Cylinder(16) = {0.0, -l2, -18.0, 0.0, l,  l, r3, 2*Pi};

Cylinder(17) = { 0.0, l2, -18.0, 0.0,-l,  l, r3, 2*Pi};

BooleanDifference(19)={Volume{9}; Delete;}{ Volume{14,15,16,17}; Delete;};

Cylinder(20) = {-8.0, -8.0, 2.0,   16, 16.0, 0,r, 2*Pi};

BooleanDifference(21)={Volume{19}; Delete;}{ Volume{20}; Delete;};

Cylinder(22) = {-15.0,  -10.0, 7.0,   0, 16.0, 0,r, 2*Pi};

BooleanDifference(23)={Volume{21}; Delete;}{ Volume{22}; Delete;};

Cylinder(24) = {-10.0,  -15.0, 7.0,   16.0, 0.0, 0,r, 2*Pi};

BooleanDifference(25)={Volume{23}; Delete;}{ Volume{24}; Delete;};

Cylinder(26) = {-10.0,  15.0, 7.0,   16.0, 0.0, 0,r, 2*Pi};

BooleanDifference(27)={Volume{25}; Delete;}{ Volume{26}; Delete;};

Cylinder(28) = {15.0,  -10.0, 7.0,   0, 16.0, 0,r, 2*Pi};

BooleanDifference(29)={Volume{27}; Delete;}{ Volume{28}; Delete;};

Box(30) = {-50,-50,-21, 100,100,7.8};

BooleanDifference(31)={Volume{29}; Delete;}{ Volume{30}; Delete;};

//Box(32) = {-30,-30,22, 60,60,7.8};

//BooleanDifference(33)={Volume{31}; Delete;}{ Volume{32}; Delete;};
