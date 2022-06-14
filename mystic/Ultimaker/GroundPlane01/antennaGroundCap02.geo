// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;


Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 20, 11.0, 2*Pi};


Cylinder(2) = {0.0, 0.0, 0., 0, 0, 10, 7.7, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Box(4) = {-10.5,-2.5,20, 21,5,46};

BooleanUnion(8)={ Volume{5};  Delete; }{Volume{4};  Delete;};


