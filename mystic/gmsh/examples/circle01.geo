// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

// Volumes can be constructed from (closed) curve loops thanks to the
// ‘ThruSections’ command
Circle(1) = {0.1,0,0, 1.0};  
Circle(2) = {3.0,0.0,0.0, 2.0}; 

Box(3) = {0,0,0, 5,5,1};


Mesh.MeshSizeMin=0.5;
Mesh.MeshSizeMax=0.5;

Point(14)=(2,0,0);
Point(15)=(0,0,0);
Point(16)=(-2.0,0.0,0);
Circle(17)={14,15,16};

Plane Surface(19)={1};

Disk(25)={4.0,0,0,3.0};

Disk(27)={6.0,0,0,3.0};

BooleanIntersection(30)={Surface{25}; Delete;}{Surface{27}; Delete;};
