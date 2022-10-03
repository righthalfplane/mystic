
SetFactory("OpenCASCADE");

Sphere(1) = {0.75, 0, 0, 0.5, -Pi/2, Pi/2, 2*Pi};
Sphere(2) = {0, 0, 0, 1, -Pi/2, Pi/2, 2*Pi};

BooleanDifference(3) = { Volume{2}; Delete; }{ Volume{1}; Delete; };

Characteristic Length{ PointsOf{ Volume{3}; } } = 0.05;