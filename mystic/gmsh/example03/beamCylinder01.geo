// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 10, 1.0, 2*Pi};

Mesh.MeshSizeMin=0.2;
Mesh.MeshSizeMax=0.2;

Mesh.MeshSizeMin=0.4;
Mesh.MeshSizeMax=0.4;

Mesh.SubdivisionAlgorithm=2;

Cylinder(2) = {0.0, 0.0, 0.0, 0, 0, 10, 0.5, 2*Pi};


BooleanDifference(7)={Volume{1}; Delete;}{Volume{2}; Delete;};
