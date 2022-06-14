// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 20, 5.0, 2*Pi};

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;


Cylinder(2) = {0.0, 0.0, 0.0, 0, 0, 11.0, 3.325, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Cylinder(6) = {-15.0, 0.0, 15.5,   30, 0.0, 0, 1.2, 2*Pi};

BooleanDifference(8)={Volume{3}; Delete;}{Volume{6}; Delete;};


//Cylinder(4) = {0.0, 0.0, 15.0, 0, 0, 10.0, 3.0, 2*Pi};

//f() = BooleanFragments { Volume{3,4};  Delete; }{};

//Cylinder(7) = {-15.0, 0.0, 61.5,   30, 0.0, 0, 2.2, 2*Pi};
