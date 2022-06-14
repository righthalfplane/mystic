// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=1.0;
Mesh.MeshSizeMax=1.0;

//Mesh.SubdivisionAlgorithm=2;

w=6.36/2;

y=1.77+0.06;

t=1;

Box(21) = {-w,-y,0.0,    2*w,2*y,t};

Box(22) = {-w,-y,0.0,    2*w,2*y,t};

Box(23) = {-w,-y,0.0,    2*w,2*y,t};


Rotate{{0,0,1}, {0,0,0},6*Pi/9}{ Volume{22}; }

Rotate{{0,0,1}, {0,0,0},2*6*Pi/9}{ Volume{23}; }

BooleanUnion(30)={ Volume{21};  Delete; }{Volume{22,23};  Delete;};


v()=Translate {3*w, 0, 0} { Duplicata { Volume{30}; } };