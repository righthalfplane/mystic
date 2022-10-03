// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=20;
Mesh.MeshSizeMax=20;

Mesh.SubdivisionAlgorithm=2;

Mesh 3;


lc = 0.3;

top=146.6;

side=230.4/2;

side2=side+10;

Box(20) = {-side2,-side2,-20, 2*side2,2*side2,20};


