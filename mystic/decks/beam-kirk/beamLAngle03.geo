// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Box(1) = {0,0,0, 1.5,1.5,34};

Mesh.MeshSizeMin=0.3;
Mesh.MeshSizeMax=0.3;

Mesh.SubdivisionAlgorithm=2;

Box(2) = {0.18,0.18,0, 1.75,1.75,34};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Box(4) = {0,0,34, 1.5,1.5,34};

Box(5) = {0.18,0.18,34, 1.75,1.75,34};

BooleanDifference(6)={Volume{4}; Delete;}{Volume{5}; Delete;};

f() = BooleanFragments { Volume{3,6}; Delete; }{};