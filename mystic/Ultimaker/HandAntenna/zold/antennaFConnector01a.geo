// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Mesh.MeshSizeMin=0.2;
Mesh.MeshSizeMax=0.2;

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

r=3.42+0.2;

r1=1.68+0.2;

//Mesh.SubdivisionAlgorithm=2;

Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 25, 7.5, 2*Pi};

Cylinder(4) = {0.0, 0.0, 5.0, 0, 0, 20., r, 2*Pi};

BooleanDifference(5)={Volume{1}; Delete;}{Volume{4}; Delete;};

Cylinder(6) = {-15.0, 0.0, 8.5,   30, 0.0, 0, r1, 2*Pi};

BooleanDifference(8)={Volume{5}; Delete;}{Volume{6}; Delete;};


