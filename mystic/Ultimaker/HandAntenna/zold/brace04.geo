// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;


Box(1) = {0,0,4, 20,40,10};

Cylinder(2) = {0.0, 20.0, 14.0, 20, 0, 0, 5.25, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Box(4) = {170.0,0,4, 20,40,10};

Cylinder(5) = {170.0, 20.0, 14.0, 20, 0, 0, 5.25, 2*Pi};

BooleanDifference(6)={Volume{4}; Delete;}{Volume{5}; Delete;};

Box(7) = {0,0,0, 190,40,4};

Box(8) = {55.0,0,4, 20,40,10};

Cylinder(9) = {55.0, 20.0, 14.0, 20, 0, 0, 5.25, 2*Pi};

BooleanDifference(10)={Volume{8}; Delete;}{Volume{9}; Delete;};

Box(11) = {115.0,0,4, 20,40,10};

Cylinder(12) = {115.0, 20.0, 14.0, 20, 0, 0, 5.25, 2*Pi};

BooleanDifference(14)={Volume{11}; Delete;}{Volume{12}; Delete;};


f() = BooleanFragments { Volume{3,6,7,10,14};  Delete; }{};

