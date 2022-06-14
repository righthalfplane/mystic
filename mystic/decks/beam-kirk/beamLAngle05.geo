// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Box(1) = {0,0,0, 0.18,1.5,7};

Box(2) = {0.18,0,0, 1.32,0.18,7};

Box(3) = {0,0,7, 0.18,1.5,54};

Box(4) = {0.18,0,7, 1.32,0.18,54};

Box(5) = {0,0,61, 0.18,1.5,7};

Box(6) = {0.18,0,61, 1.32,0.18,7};


Mesh.MeshSizeMin=0.15;
Mesh.MeshSizeMax=0.15;

Mesh.MeshSizeMin=0.3;
Mesh.MeshSizeMax=0.3;

Mesh.SubdivisionAlgorithm=2;


f() = BooleanFragments { Volume{1,2,3,4,5,6}; Delete; }{};