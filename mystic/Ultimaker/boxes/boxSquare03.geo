// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");


Box(1) = {0,0,2, 40,20,4};

//Mesh.MeshSizeMin=0.3;
//Mesh.MeshSizeMax=0.3;

Mesh.CharacteristicLengthMax = 0.5;
Mesh.CharacteristicLengthMin = 0.5;

//Mesh.SubdivisionAlgorithm=2;

Box(2) = {5,5,0, 30,10,2};

Sphere(3) = {20,10,11, 3.0};

Cylinder(4) = {20.0, 10.0, 6.0, 0, 0, 3.0, 1.5, 2*Pi};

//f() = BooleanFragments { Volume{1,2,3,4};  Delete; }{};
