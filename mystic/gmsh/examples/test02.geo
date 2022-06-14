// Gmsh project created on Fri May 28 16:21:57 2021
//+
Point(1) = {-4.0, 0.0, 0, 1.0};
//+
Point(2) = {-4.0, -1.0, 0, 1.0};
//+
Point(3) = {-4.0, 0, 0, 1.0};
//+
Point(4) = {-4.0, 0, 0, 1.0};
//+
Point(5) = {-3.0, -1.0, 0, 1.0};
//+
Point(6) = {-3.0, -9.0, 0, 1.0};
//+
Point(7) = {-4.0, -9.0, 0, 1.0};
//+
Point(8) = {-4.0, -10.0, 0, 1.0};
//+
Point(9) = {4.0, -10.0, 0, 1.0};
//+
Point(10) = {4.0, -9.0, 0, 1.0};
//+
Point(11) = {3.0, -9.0, 0, 1.0};
//+
Point(12) = {3.0, -1.0, 0, 1.0};
//+
Point(13) = {4.0, -1.0, 0, 1.0};
//+
Point(14) = {4.0, 0.0, 0, 1.0};
//+
Point(15) = {-0.5, -1, 0, 1.0};
//+
Point(16) = {0.5, -1, 0, 1.0};
//+
Point(17) = {0.5, -9, 0.0, 1.0};
//+
Point(18) = {-0.5, -9, 0, 1.0};
//+
Line(1) = {1, 2};
//+
Line(2) = {2, 15};
//+
Line(3) = {18, 7};
//+
Line(4) = {8, 9};
//+
Line(5) = {10, 17};
//+
Line(6) = {16, 16};
//+
Line(7) = {17, 16};
//+
Line(8) = {16, 13};
//+
Line(9) = {13, 14};
//+
Line(10) = {14, 1};
//+
Line(11) = {15, 18};
//+
Line(12) = {7, 8};
//+
Line(13) = {9, 10};
//+
Recursive Delete {
  Point{5}; 
}
//+
Recursive Delete {
  Point{12}; 
}
//+
Recursive Delete {
  Point{6}; 
}
//+
Recursive Delete {
  Point{11}; 
}
//+
Curve Loop(1) = {1, 2, 11, 3, 12, 4, 13, 5, 7, 8, 9, 10};
//+
Plane Surface(1) = {1};

Physical Surface("clamped") = {1};

//+
Extrude {0, 0, 80} {
  Surface{1}; 
}

Mesh.MeshSizeMin=2.0;
Mesh.MeshSizeMax=2.0;

//+
Surface Loop(1) = {46, 1, 30, 34, 38, 42, 75, 50, 54, 58, 62, 66, 70, 74};
//+
Volume(2) = {1};

Mesh.SubdivisionAlgorithm=2;


