// Gmsh project created on Fri May 28 11:18:05 2021
SetFactory("OpenCASCADE");



Box(2) = {0,0,0, 160,70,40};

Mesh.CharacteristicLengthMax = 2.0;
Mesh.CharacteristicLengthMin = 2.0;

//Mesh.SubdivisionAlgorithm=2;

Box(3) = {3,3,3, 154,64,40};

BooleanDifference(4)={Volume{2}; Delete;}{Volume{3}; Delete;};

//BooleanDifference(5)={Volume{4}; Delete;}{Volume{1}; Delete;};

//Merge "Olivia.stl";

//Physical Surface("Box") = {1};

//Surface Loop(1) = {1};

//Volume(8)={1};

//BooleanDifference(5)={Volume{4}; Delete;}{"Olivia.stl"; Delete;};
