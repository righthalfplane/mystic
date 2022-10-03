// Gmsh project created on Sat May 29 09:17:46 2021
SetFactory("OpenCASCADE");
//+
Point(1) = {-1.0, 0.5, 0, 1.0};
//+
Point(2) = {-1.0, -0.5, 0, 1.0};
//+
Point(3) = {1, -0.5, 0, 1.0};
//+
Point(4) = {1, 0.5, 0, 1.0};
//+
Line(1) = {1, 2};
//+
Line(2) = {2, 3};
//+
Line(3) = {3, 4};
//+
Line(4) = {4, 1};
//+
Curve Loop(1) = {4, 1, 2, 3};
//+
Plane Surface(1) = {1};

newStuff[]=
Extrude {0, 0, 20} {
  Surface{1}; 
};
//+
Transfinite Curve {9, 1, 3, 12} = 4 Using Progression 1;
//+
Transfinite Curve {7, 2, 11, 4} = 5 Using Progression 1;
//+
Transfinite Curve {5, 10, 6, 8} = 20 Using Progression 1;
//+
Transfinite Surface {6};
//+
Transfinite Surface {1};
//+
Transfinite Surface {3};
//+
Transfinite Surface {5};
//+
Transfinite Surface {4};
//+
Transfinite Surface {2};
//+
Recombine Surface {3, 4, 1, 5, 2, 6};
//+
Transfinite Volume{1} = {2, 7, 6, 1, 3, 8, 5, 4};
//+
Hide "*";
//+
Show {
  Point{5}; Point{6}; Point{7}; Point{8}; Curve{7}; Curve{9}; Curve{11}; Curve{12}; Surface{6}; 
}
