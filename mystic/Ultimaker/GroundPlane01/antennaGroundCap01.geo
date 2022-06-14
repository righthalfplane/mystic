// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;


Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 20, 11.0, 2*Pi};


Cylinder(2) = {0.0, 0.0, 10., 0, 0, 20, 7.7, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

//Cylinder(4) = {-10.0, 0.0, 0.0, 10, 0, 14.14, 5.175, 2*Pi};

//Cylinder(5) = {10.0, 0.0, 0.0, -10, 0, 14.14, 5.175, 2*Pi};

//Cylinder(6) = {0.0, -10.0, 0.0, 0.0, 10, 14.14, 5.175, 2*Pi};

//Cylinder(7) = { 0.0, 10.0, 0.0, 0.0,-10, 14.14, 5.175, 2*Pi};


