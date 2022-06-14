SetFactory('OpenCASCADE');
Mesh.CharacteristicLengthMax = 0.1;
Mesh.CharacteristicLengthMin = 0.1;
Box(1) = {0,0,0, 1, 1, 1};
Sphere(2) = {0,0,0, 0.65};
BooleanIntersection{ Volume{1}; Delete;}{ Volume{2}; Delete;}
