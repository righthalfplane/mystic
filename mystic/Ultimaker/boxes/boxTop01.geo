// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");



//Mesh.MeshSizeMin=0.3;
//Mesh.MeshSizeMax=0.3;

Mesh.CharacteristicLengthMax = 0.5;
Mesh.CharacteristicLengthMin = 0.5;

Mesh.CharacteristicLengthMax = 1.0;
Mesh.CharacteristicLengthMin = 1.0;

//Mesh.SubdivisionAlgorithm=2;


Box(1) = {0,0,2, 40,20,4};

Box(2) = {5,5,0, 30,10,2};

Sphere(3) = {20,10,11, 3.0};

Box(4) = {17,7,8, 6,6,5};

Cone(5) = {20,10,5.99, 0.0,0,3.0, 3.0,0.5};

BooleanIntersection(6) = { Volume{3}; Delete; }{ Volume{4}; Delete; };




//f() = BooleanFragments { Volume{1,2,3,4};  Delete; }{};
//Cylinder(4) = {20.0, 10.0, 6.0, 0, 0, 3.0, 1.5, 2*Pi};
//Torus(7) = {0,0,0, 3.0, 1.0, 2*Pi};
//Cone(9) = {6,0,0, 1.5,0,0, 0.5,1.0};
