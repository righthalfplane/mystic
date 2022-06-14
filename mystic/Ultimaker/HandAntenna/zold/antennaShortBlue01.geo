// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 71, 7.5, 2*Pi};

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;

Cylinder(2) = {0.0, 0.0, 51., 0, 0, 30, 5.25, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Cylinder(4) = {0.0, 0.0, 0.0, 0, 0, 9.6, 3.85, 2*Pi};

BooleanDifference(5)={Volume{3}; Delete;}{Volume{4}; Delete;};

Cylinder(6) = {-15.0, 0.0, 5.82,   30, 0.0, 0, 2.2, 2*Pi};

Cylinder(7) = {-15.0, 0.0, 54.5,   30, 0.0, 0, 2.2, 2*Pi};

BooleanDifference(8)={Volume{5}; Delete;}{Volume{6};Volume{7}; Delete;};

Cylinder(9) = {0.0, 0.0, 14.0, 0, 0, 3, 9.7, 2*Pi};

Cylinder(10) = {0.0, 0.0, 41.0, 0, 0, 3, 9.7, 2*Pi};

Box(12) = {7.5,-1.0,0.0,    3,2,95};

Box(14) = {7.5,-1.0,0.0,    3,2,95};

BooleanDifference(15)={Volume{9}; Delete;}{Volume{12}; Delete;};

BooleanDifference(16)={Volume{10}; Delete;}{Volume{14}; Delete;};

f() = BooleanFragments { Volume{8,15,16};  Delete; }{};

