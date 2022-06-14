// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=0.4;
Mesh.MeshSizeMax=0.4;

//Mesh.SubdivisionAlgorithm=2;

ri=43.34-0.2;
ro=44.84-0.2;
h=6.5-1;

Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, h,ro, 2*Pi};

Cylinder(2) = {0.0, 0.0, 0.0, 0, 0, h-1.0,ri, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};

Cone(4) = {0,0,h, 0.,0,1, ro,0};

BooleanUnion(7)={ Volume{3};  Delete; }{Volume{4};  Delete;};

