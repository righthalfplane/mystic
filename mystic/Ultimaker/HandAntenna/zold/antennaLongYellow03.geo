// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;

w=6.36/2;

y=1.77+0.06;

t=8;

r1=7.10/2+0.2;

r2=2.78/2+.2;

Box(21) = {-w,-y,0.0,    2*w,2*y,t};

Box(22) = {-w,-y,0.0,    2*w,2*y,t};

Box(23) = {-w,-y,0.0,    2*w,2*y,t};


Rotate{{0,0,1}, {0,0,0},6*Pi/9}{ Volume{22}; }

Rotate{{0,0,1}, {0,0,0},2*6*Pi/9}{ Volume{23}; }

BooleanUnion(30)={ Volume{21};  Delete; }{Volume{22,23};  Delete;};

Rotate{{0,1,0}, {0,0,0},Pi/2}{ Volume{30}; }

//v()=Translate {-8.0, 0.0, 5.82} { Duplicata { Volume{30}; } };

//z()=Translate {-8.0, 0.0, 78.5} { Volume{30}; };

z()=Translate {-8.0, 0.0, 5.82} { Volume{30}; };

Cylinder(1) = {0.0, 0.0, 0.0, 0, 0, 15, 7.5, 2*Pi};

//Cylinder(2) = {0.0, 0.0, 75., 0, 0, 30, 5.2, 2*Pi};

BooleanDifference(3)={Volume{1}; Delete;}{Volume{30}; Delete;};

Cylinder(4) = {0.0, 0.0, 0.0, 0, 0, 9.6, r1, 2*Pi};

//BooleanDifference(5)={Volume{3}; Delete;}{Volume{4}; Delete;};

Cylinder(6) = {-15.0, 0.0, 5.82,   30, 0.0, 0, r2, 2*Pi};

BooleanDifference(7)={Volume{3}; Delete;}{Volume{4};Volume{6}; Delete;};



//Cylinder(7) = {-15.0, 0.0, 78.5,   30, 0.0, 0, 2.2, 2*Pi};

//BooleanDifference(8)={Volume{5}; Delete;}{Volume{6};Volume{7}; Delete;};

//Cylinder(9) = {0.0, 0.0, 14.0, 0, 0, 3, 12, 2*Pi};

//Cylinder(10) = {0.0, 0.0, 65.0, 0, 0, 3, 12, 2*Pi};

//Box(12) = {7.5,-1.0,0.0,    6,2,95};

//Box(14) = {7.5,-1.0,0.0,    6,2,95};

//BooleanDifference(15)={Volume{9}; Delete;}{Volume{12}; Delete;};

//BooleanDifference(16)={Volume{10}; Delete;}{Volume{14}; Delete;};

//BooleanUnion(17)={ Volume{8};  Delete; }{Volume{15,16};  Delete;};

//BooleanDifference(18)={Volume{17}; Delete;}{Volume{v(0),z(0)}; Delete;};

