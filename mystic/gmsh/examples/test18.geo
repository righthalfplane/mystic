// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Box(1) = {0,0,0, 40,8,4};

Mesh.MeshSizeMin=0.2;
Mesh.MeshSizeMax=0.2;

Mesh.SubdivisionAlgorithm=2;

Mesh 3;

Box(2) = {17.5,0,0, 5,4,4};

Cylinder(3) = {20.0, 4.0, 0.0, 0, 0, 5, 2.0, 2*Pi};

f() = BooleanDifference { Volume{1}; Delete; }{ Volume{2,3}; Delete;};