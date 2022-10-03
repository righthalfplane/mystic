// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

Mesh.MeshSizeMin=15;
Mesh.MeshSizeMax=15;

Mesh.SubdivisionAlgorithm=2;

Mesh 3;


lc = 0.3;

top=146.6;

side=230.4/2;

side2=side+10;


Point(1) = {-side+0.4, -side-0.1, 0.0, lc};

Point(2) = {side-0.7, -side-0.3, 0.0, lc};

Point(3) = {side+0.5, side+0.1, 0.0, lc};

Point(4) = {-side-0.55, side-0.99, 0.0, lc};

Point(5) = {0.0, 0.0, top, lc};


Line(1) = {1, 2};

Line(2) = {2, 3};

Line(3) = {3, 4};

Line(4) = {4, 1};


Line(5) = {5, 1};

Line(6) = {5, 2};

Line(7) = {5, 3};

Line(8) = {5, 4};

Curve Loop(1) = {1, 2, 3, 4};
Curve Loop(2) = {1, 6, 5};
Curve Loop(3) = {2, 7, 6};
Curve Loop(4) = {3, 8, 7};
Curve Loop(5) = {4, 5, 8};



Plane Surface(1) = {1};
Plane Surface(2) = {2};
Plane Surface(3) = {3};
Plane Surface(4) = {4};
Plane Surface(5) = {5};

Surface Loop(1)={1,2,3,4,5};

Volume(1)={1};

Box(20) = {-side2,-side2,-20, 2*side2,2*side2,20};

BooleanUnion(23)={ Volume{1};  Delete; }{Volume{20};  Delete;};

Box(25) = {-side2,-side2,top-1, 2*side2,2*side2,20};

BooleanDifference(26) = { Volume{23}; Delete; }{ Volume{25}; Delete; };



