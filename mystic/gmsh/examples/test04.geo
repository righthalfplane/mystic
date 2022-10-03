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
Point(15) = {-1, -1, 0, 1.0};
//+
Point(16) = {1, -1, 0, 1.0};
//+
Point(17) = {1.0, -9, 0.0, 1.0};
//+
Point(18) = {-1, -9, 0, 1.0};
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

Point(19) = {-1, 0, 0, 1.0};
//+
Point(20) = {1.0, -0, 0, 1.0};
//+
Point(21) = {-1.0, -10, 0, 1.0};
//+
Point(22) = {1, -10, 0, 1.0};
//+
Line(14) = {15, 16};
//+
Line(15) = {15, 19};
//+
Line(16) = {16, 20};
//+
Line(17) = {18, 21};
//+
Line(18) = {17, 22};
//+
Line(19) = {18, 17};
//+
Recursive Delete {
  Curve{10}; 
}
//+
Recursive Delete {
  Curve{4}; 
}
//+
Line(20) = {1, 19};
//+
Line(21) = {19, 20};
//+
Line(22) = {20, 14};
//+
Line(23) = {8, 21};
//+
Line(24) = {22, 21};
//+
Line(25) = {22, 9};
//+
Curve Loop(1) = {2, 15, -20, 1};
//+
Plane Surface(1) = {1};
//+
Curve Loop(2) = {14, 16, -21, -15};
//+
Plane Surface(2) = {2};
//+
Curve Loop(3) = {16, 22, -9, -8};
//+
Plane Surface(3) = {3};
//+
Curve Loop(4) = {14, -7, -19, -11};
//+
Plane Surface(4) = {4};
//+
Curve Loop(5) = {3, 12, 23, -17};
//+
Plane Surface(5) = {5};
//+
Curve Loop(6) = {19, 18, 24, -17};
//+
Plane Surface(6) = {6};
//+
Curve Loop(7) = {18, 25, 13, 5};
//+
Plane Surface(7) = {7};
//+
Extrude {0, 0, 20} {
  Curve{20}; Curve{21}; Curve{22}; Curve{16}; Curve{8}; Curve{9}; Curve{15}; Curve{1}; Curve{2}; Curve{14}; Curve{11}; Curve{7}; Curve{3}; Curve{12}; Curve{17}; Curve{23}; Curve{19}; Curve{24}; Curve{18}; Curve{5}; Curve{13}; Curve{25}; Layers {21}; Recombine;
}
//+
Transfinite Curve {56, 27, 51, 28, 39, 32, 44, 36, 80, 76, 84, 68, 95, 71, 107, 103} = 21 Using Progression 1;
//+
Transfinite Curve {26, 58, 2, 20, 42, 34, 8, 22, 23, 3, 74, 86, 102, 110, 25, 5} = 4 Using Progression 1;
//+
Transfinite Curve {94, 90, 24, 19, 62, 30, 14, 21} = 3 Using Progression 1;
//+
Transfinite Curve {78, 82, 98, 106, 12, 17, 18, 13, 54, 50, 38, 46, 1, 15, 16, 9} = 3 Using Progression 1;
//+
Curve Loop(8) = {78, 86, -82, 74};
//+
Plane Surface(114) = {8};
//+
Curve Loop(9) = {66, 90, 70, -62};
//+
Plane Surface(115) = {9};
//+
Curve Loop(10) = {82, -94, -98, -90};
//+
Plane Surface(116) = {10};
//+
Curve Loop(11) = {102, 98, 110, 106};
//+
Plane Surface(117) = {11};
//+
Curve Loop(12) = {58, 50, -26, 54};
//+
Plane Surface(118) = {12};
//+
Curve Loop(13) = {30, -38, -62, 50};
//+
Plane Surface(119) = {13};
//+
Curve Loop(14) = {34, -46, -42, 38};
//+
Plane Surface(120) = {14};
