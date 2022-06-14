// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=0.4;
Mesh.MeshSizeMax=0.4;

//Mesh.SubdivisionAlgorithm=2;

ri=43.34-0.2;
ro= 44.84-0.2;

Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 5.5, ro, 2*Pi};

Cylinder(2) = {0.0, 0.0, 1.0, 0, 0, 5.5, ri, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};



