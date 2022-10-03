// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Box(1) = {0,0,0, 10,10,10};

Mesh.MeshSizeMin=0.6;
Mesh.MeshSizeMax=0.6;

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

Mesh.SubdivisionAlgorithm=2;

Cylinder(2) = {0.0, 0.0, 0.0, 0, 0, 10, 4.0, 2*Pi};

BooleanFragments{ Volume{1,2}; Delete; }{}

Box(6) = {0,0,0, 10,10,10};

BooleanIntersection(7)={Volume{6}; Delete;}{Volume{3}; Delete;};
