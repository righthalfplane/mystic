// Gmsh project created on Fri May 28 11:18:05 2021
//
// Good for One color at 0.2
//
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=0.4;
Mesh.MeshSizeMax=0.4;

//Mesh.SubdivisionAlgorithm=2;

ro=177;
ri=175;
//h=0;



Box(1) = {0,0,0,  ro,20,10};


Box(2) = {1,1,1,  ri,20,10};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{2}; Delete;};


Box(4) = {35,4.5,0,  40,4,10};

BooleanDifference(5)={Volume{3}; Delete;}{Volume{4}; Delete;};


//Cone(1) = {0,0,0.0, 0.,0,6.6, ro-0.2,ro+0.4};

//Cone(2) = {0,0,0.0, 0.,0,5.6, ri-0.2,ri+0.4};



//BooleanDifference(9)={Volume{7}; Delete;}{Volume{8}; Delete;};

//Cylinder(10) = {ro-10, ro-10, h, 0, 0, 2, 10, 2*Pi};

//BooleanUnion(11)={ Volume{9};  Delete; }{Volume{1,10};  Delete;};

//BooleanDifference(12)={Volume{11}; Delete;}{Volume{2}; Delete;};

//Cylinder(14) = {ro-8, ro-8, h, 0, 0, 2, 3, 2*Pi};

//BooleanDifference(15)={Volume{12}; Delete;}{Volume{14}; Delete;};

//Cone(16) = {0,0,6.6, 0.,0,1, ro,0};

//BooleanUnion(17)={ Volume{15};  Delete; }{Volume{16};  Delete;};
