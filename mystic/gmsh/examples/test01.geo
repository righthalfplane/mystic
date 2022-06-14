// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");
//+
Point(1) = {-4.0, -1.0, 0.0, 1.0};
//+
Point(2) = {4.0, -1.0, 0.0, 1.0};
//+
Point(3) = {4.0, 1.0, 0.0, 1.0};
//+
Point(4) = {-4.0, 1.0, 0.0, 1.0};
//+
Line(1) = {1, 2};
//+
Line(2) = {2, 3};
//+
Line(3) = {3, 4};
//+
Line(4) = {4, 1};

Curve Loop(1) = {1, 2, 3, 4};

Plane Surface(1) = {1};

Physical Surface("My surface") = {1};


newStuff[]=
Extrude {0, 0, 10.0}{ 
    Surface {1}; 
    Layers{10.0};
    Recombine;
}

Mesh.MeshSizeMin=0.2;
Mesh.MeshSizeMax=0.2;

Mesh.SubdivisionAlgorithm=2;

Mesh 3;