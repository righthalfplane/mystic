// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 50, 7.5, 2*Pi};

Mesh.MeshSizeMin=0.2;
Mesh.MeshSizeMax=0.2;

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;

Cylinder(2) = {0.0, 0.0, 30., 0, 0, 20, 5.25, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Cylinder(4) = {0.0, 0.0, 0.0, 0, 0, 20, 5.25, 2*Pi};

BooleanDifference(5)={Volume{3}; Delete;}{Volume{4}; Delete;};

Cylinder(6) = {-15.0, 0.0, 10.0,   30, 0.0, 0, 2.2, 2*Pi};

Cylinder(7) = {-15.0, 0.0, 33.5,   30, 0.0, 0, 2.2, 2*Pi};

BooleanDifference(8)={Volume{5}; Delete;}{Volume{6};Volume{7}; Delete;};
