// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");



Mesh.MeshSizeMin=0.2;
Mesh.MeshSizeMax=0.2;

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

r=3.42+0.2;

r1=1.68+0.2;

r3=5.57+0.3;

//Mesh.SubdivisionAlgorithm=2;


Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 95, 7.5, 2*Pi};

//Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 20, 7.5, 2*Pi};

Cylinder(2) = {0.0, 0.0, 10.0, 0, 0, 8.35, r, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Cylinder(4) = {-15.0, 0.0, 14.5,   30, 0.0, 0, r1, 2*Pi};

BooleanDifference(5)={Volume{3}; Delete;}{Volume{4}; Delete;};

Cylinder(6) = {0.0,0.0,0.0,   0,0,10.0, r3, 2*Pi};

BooleanDifference(7)={Volume{5}; Delete;}{Volume{6}; Delete;};

Cylinder(8) = {-15.0, 0.0, 78.5,   30, 0.0, 0, r1, 2*Pi};

BooleanDifference(9)={Volume{7}; Delete;}{Volume{8}; Delete;};

Cylinder(10) = {0.0, 0.0, 75., 0, 0, 30, r, 2*Pi};

BooleanDifference(11)={Volume{9}; Delete;}{Volume{10}; Delete;};

Cylinder(12) = {0.0, 0.0, 20.0, 0, 0, 3, 9.7, 2*Pi};

Cylinder(14) = {0.0, 0.0, 70.0, 0, 0, 3, 9.7, 2*Pi};

BooleanUnion(15)={ Volume{11};  Delete; }{Volume{12,14};  Delete;};

Box(16) = {7.5,-1.0,0.0,    3,2,95};

Box(17) = {7.5,-1.0,0.0,    3,2,95};

BooleanDifference(18)={Volume{15}; Delete;}{Volume{16,17}; Delete;};

