// Gmsh project created on Sat May 29 09:17:46 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin = 2.0;
Mesh.MeshSizeMax = 2.0;
Mesh.SubdivisionAlgorithm=2;


Box(1) = {-4,4,0, 8,1,20};
Box(2) = {-0.5,-4,0, 1,8,20};
Box(3) = {-4,-5,0, 8,1,20};

f() = BooleanFragments { Volume{1,2,3}; Delete; }{};
