// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");

//Mesh.SubdivisionAlgorithm=1;
Mesh.MeshSizeMin=0.02;
Mesh.MeshSizeMax=0.02;


// Volumes can be constructed from (closed) curve loops thanks to the
// ‘ThruSections’ command
Disk(1) = {0.1,0,0, 1.0};  
Disk(2) = {3.0,0.0,0.0, 2.0}; 

BooleanIntersection(3)={Surface{1}; Delete;}{Surface{2}; Delete;};


Rectangle(4)={-2,-2,0,6,4};

BooleanFragments{ Surface{3,4}; Delete; }{}

