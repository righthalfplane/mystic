// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;

r2=Sqrt(2.0);

l=15;

r1=7.825+0.2;

r4=5.175;

r3=3.175+0.3;

l2=l-r4;

l3=r2*l2;

zh=20;

Box(1) = {-15,-15,0, 30,30,4};


Cylinder(2) = {0.0,0.0,0.0,  0,0,20, r1, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Cylinder(4) = {l2,l2,r4+3, l3,l3,-zh, r4, 2*Pi};

Cylinder(5) = {-l2,l2,r4+3, -l3,l3,-zh, r4, 2*Pi};

Cylinder(6) = {l2,-l2,r4+3, l3,-l3,-zh, r4, 2*Pi};

Cylinder(7) = {-l2,-l2,r4+3, -l3,-l3,-zh, r4, 2*Pi};

BooleanUnion(8)={ Volume{3};  Delete; }{Volume{4,5,6,7};  Delete;};

Cylinder(9) = {l2,l2,r4+3, l3,l3,-zh, r3, 2*Pi};

Cylinder(10) = {-l2,l2,r4+3, -l3,l3,-zh, r3, 2*Pi};

Cylinder(11) = {l2,-l2,r4+3, l3,-l3,-zh, r3, 2*Pi};

Cylinder(12) = {-l2,-l2,r4+3, -l3,-l3,-zh, r3, 2*Pi};

BooleanDifference(14)={Volume{8}; Delete;}{ Volume{9,10,11,12}; Delete;};

Box(15) = {-50,-50,4, 100,100,20};

BooleanDifference(16)={Volume{14}; Delete;}{ Volume{15}; Delete;};

Box(17) = {-50,-50,-20, 100,100,14};

BooleanDifference(18)={Volume{16}; Delete;}{ Volume{17}; Delete;};
