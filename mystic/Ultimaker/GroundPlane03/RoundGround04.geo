// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;

r2=Sqrt(2.0);

l=15;

r1=7.825+0.2;

r4=5.175;

r42=r4*0.5;

r3=3.175+0.3;

l2=l-r4+3;

l3=r2*l2;

zh=20;

i3=1.5;

//Box(1) = {-17,-17,0, 34,34,4};

Cylinder(1) = {0.0,0.0,0.0,  0,0,4, 24, 2*Pi};

Cylinder(2) = {0.0,0.0,0.0,  0,0,20, r1, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Box(17) = {-20,-5,3, 40,10,10};

Box(18) = {-5,-20,3, 10,40,10};

BooleanDifference(19)={Volume{3}; Delete;}{ Volume{17,18}; Delete;};

Cylinder(4) = {l3,0,r4+i3, l3,0,-l3, r4, 2*Pi};

Cylinder(5) = {0,l3,r4+i3, 0,l3,-l3, r4, 2*Pi};

Cylinder(6) = {0,-l3,r4+i3, 0,-l3,-l3, r4, 2*Pi};

Cylinder(7) = {-l3,0,r4+i3, -l3,0,-l3, r4, 2*Pi};

//Box(19) = {l2-r42,l2-r42,r4+i3, l2+2*r42,l2+2*r42,-l3};

BooleanUnion(8)={ Volume{19};  Delete; }{Volume{4,5,6,7};  Delete;};

Cylinder(9) = {l3,0,r4+i3, l3,0,-l3, r3, 2*Pi};

Cylinder(10) = {0,l3,r4+i3, 0,l3,-l3, r3, 2*Pi};

Cylinder(11) = {0,-l3,r4+i3, 0,-l3,-l3, r3, 2*Pi};

Cylinder(12) = {-l3,0,r4+i3, -l3,0.,-l3, r3, 2*Pi};




//Cylinder(14) = {l3,0,r4+3, -2*l3,0,l3*2, r3, 2*Pi};

//Cylinder(15) = {0,l3,r4+3, 0,-2*l3,l3*2, r3, 2*Pi};

//Cylinder(16) = {0,-l3,r4+3, 0,2*l3,l3*2, r3, 2*Pi};

//Cylinder(17) = {-l3,0,r4+3, 2*l3,0.,l3*2, r3, 2*Pi};



BooleanDifference(14)={Volume{8}; Delete;}{ Volume{9,10,11,12}; Delete;};

Box(15) = {-50,-50,-20, 100,100,12};

BooleanDifference(16)={Volume{14}; Delete;}{ Volume{15}; Delete;};

