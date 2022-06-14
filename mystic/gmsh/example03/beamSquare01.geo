// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Box(1) = {0,0,0, 5,5,50};

Mesh.MeshSizeMin=0.6;
Mesh.MeshSizeMax=0.6;

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

Mesh.SubdivisionAlgorithm=2;
