double atan2(),sqrt(),sin(),cos(),atan(),fabs();
main()
{

	fmain();

}
grid(x,y,z,n)
float *x,*y,*z;
long *n;
{
	double r,sr,xx,yy,zz,xc,yc,zc,ss,rc,rcos,rsin,zr,depth,yp1;
	double pi,theta1,dtorad,rx1,ry1,rr1,rz1,rxx,rzz,r2r,r2x,r2y,r2h,ts1,te1,rk;
	double ts2,te2,rr2,rx2,ry2,theta2,rz2,yp2;
	double zp,yp;
	long i,j,k,m,mstep,step,nx,ny,nt,kk;


	pi=4*atan(1.0);
	dtorad=pi/180;

	ts1=57.09;
	te1=90.0;

	rr1=11.263*2.54;
	rx1=6.119*2.54;
	ry1= -9.456*2.54;
 
	ts2=52.14;
	te2=90.0;

	rr2=7.496*2.54;
	rx2=6.184*2.54;
	ry2= -5.814*2.54;
 
	nx=30;
	ny=30;

	step=nx*ny;
        for(k=16;k<20;++k){
            kk=k-16;
	    mstep=step*k;
            for(j=0;j<15;++j){
                for(i=0;i<29;++i){
		    m=i+nx*j+mstep;
		    if(x[m] == -1.23e-21)continue;
	    	    xx=x[m];
	            yy=y[m];
	            zz=z[m];
	            r=sqrt(xx*xx+yy*yy);
	            if(r < 1.0e-3){
	               r=0;
	            }
		    sr=r/(1.807*2.54);
	            theta1=(ts1+sr*(te1-ts1));
		    rz1=rx1-rr1*cos(theta1*dtorad);
		    yp1=rr1*sin(theta1*dtorad)+ry1;
	            theta2=(ts2+sr*(te2-ts2));
	            if(theta2 < 69.089){
		        rz2=rx2-rr2*cos(theta2*dtorad);
		        rz2=3.169*2.54;
		        yp2=rr2*sin(theta2*dtorad)+ry2;
	            }else{
		        rz2=rx2-rr2*cos(theta2*dtorad);
		        yp2=rr2*sin(theta2*dtorad)+ry2;
		    }
		    zp=rz1+kk*(rz2-rz1)/3;
		    yp=yp1+kk*(yp2-yp1)/3;
			if(r < 1.0e-3){
			   if(kk == 0){
		                x[m]= 0;
		                y[m]= 0;
			        z[m]= 0;
			   }else{
		                x[m]= 0;
		                y[m]= 0;
		                z[m]= zp;
			   }
			}else{
		            x[m]= yp*xx/r;
		            y[m]= yp*yy/r;
		            z[m]= zp;
			}
	        }
	    }
	}
/*
        for(k=0;k<26;++k){
	    mstep=step*k;
            for(j=0;j<30;++j){
                for(i=0;i<30;++i){
		    m=i+nx*j+mstep;
		    if(y[m] < 0 && y[m] != -1.23e-21)y[m]=0;
                }
            }
	}
*/ 
}
end
material,1,7.8920
material,2,2.25
material,3,7.8920,.15875
material,4,7.8920,.3175
setup,1,30,30,27
block,2,  1,1,1,  30,30,16,  9,2, 10,2,  11,2,  12,2                       $concrete cylinder
.508,0,-121.92,  91.44,0,-121.92,  91.44,180,-121.92,  .508,180,-121.92/
.508,0,0,  91.44,0,0,  91.44,180,0,  .508,180,0
block,2,  15,1,17,  29,8,20   $block4
0,0,0,    4.5898,0,0,    4.5898,45,0,    0,45,0/
0,0,.635,  4.5898,0,.635,  4.5898,45,.635,  0,45,.635
block,2,  15,1,17,  22,15,20  $block6
0,90,0,    0,45,0,    4.5898,45,0,    4.5898,90,0/
0,90,.635,  0,45,.635,  4.5898,45,.635,  4.5898,90,.635
block,1,  15,1,17,  22,8,20   $block3
0,0,0,    2.21615,0,0,    1.5659,1.5659,0,    0,2.21615,0/
0,0,.635,  2.21615,0,.635,  1.5659,1.5659,.635,  0,2.21615,.635
connect,1,  22,1,17,  29,1,17,  0,  1,8,4  $block3-block4
connect,1,  15,8,17,  15,15,17,  0, 8,1,4  $block3-block6
kill,1, 23,9,17,  30,30,20
block,2,  1,1,17,  15,8,20     $block1
4.5898,180,0,    0,180,0,    0,135,0,    4.5898,135,0/
4.5898,180,.635, 0,180,.635, 0,135,.635, 4.5898,135,.635
block,2,  8,1,17,  15,15,20    $block2
0,135,0,    0,90,0,     4.5898,90,0,    4.5898,135,0/
0,135,.635, 0,90,.635,  4.5898,90,.635, 4.5898,135,.635
block,1,  8,1,17,  15,8,20     $block5
-2.21615,0,0,    0,0,0,    0,2.21615,0,    -1.5659,1.5659,0/
-2.21615,0,.635, 0,0,.635, 0,2.21615,.635,  -1.5659,1.5659,.635, 
connect,1,  1,1,17,  8,1,17,  0,  1,8,4
connect,1,  8,8,17,  8,15,17,  0,  8,1,4
kill,1, 1,9,17,  7,30,20
block,2,  16,1,21,  30,8,24    $ Block 10
4.5898,0,18.59,  4.5898,0,61.29,  4.5898,45,61.29,  4.5898,45,18.59/
4.2722,0,18.59,  4.2722,0,61.29,  4.2722,45,61.29,  4.2722,45,18.59
block,2,  15,16,21,  22,30,24    $ Block 9
4.5898,90,18.59,  4.5898,45,18.59,  4.5898,45,61.29,  4.5898,90,61.29/
4.2722,90,18.59,  4.2722,45,18.59,  4.2722,45,61.29,  4.2722,90,61.29
block,2,  8,16,21,  15,30,24    $ Block 8
4.5898,135,18.59, 4.5898,90,18.59, 4.5898,90,61.29, 4.5898,135,61.29/
4.2722,135,18.59,  4.2722,90,18.59,  4.2722,90,61.29,  4.2722,135,61.29
block,2,  1,1,21,  15,8,24    $ Block 7
4.5898,180,61.29, 4.5898,180,18.59,   4.5898,135,18.59, 4.5898,135,61.29/
4.2722,180,61.29,  4.2722,180,18.59,    4.2722,135,18.59,  4.2722,135,61.29   
block,2,  29,1,21,  30,8,24  $adjust block10
5.0889,0,59.73,  5.0889,0,61.29,  5.0889,45,61.29,  5.0889,45,59.73/
4.2722,0,59.73,  4.2722,0,61.29,  4.2722,45,61.29,  4.2722,45,59.73
block,2,  15,29,21,  22,30,24  $adjust block9
5.0889,90,59.73, 5.0889,45,59.73,  5.0889,45,61.29,  5.0889,90,61.29/
4.2722,90,59.73,  4.2722,45,59.73,  4.2722,45,61.29,  4.2722,90,61.29
block,2,  8,29,21,  15,30,24  $adjust block8
5.0889,135,59.73, 5.0889,90,59.73, 5.0889,90,61.29, 5.0889,135,61.29/
4.2722,135,59.73,  4.2722,90,59.73,  4.2722,90,61.29,  4.2722,135,61.29
block,2,  1,1,21,  2,8,24  $adjust block7
5.0889,180,61.29, 5.0889,180,59.73,  5.0889,135,59.73, 5.0889,135,61.29/
4.2722,180,61.29,  4.2722,180,59.73,   4.2722,135,59.73,   4.2722,135,61.29
block,2,  15,1,25,  29,8,26   $block14
0,0,59.73,   4.2722,0,59.73,   4.2722,45,59.73,   0,45,59.73/
0,0,61.290,  4.2722,0,61.290,  4.2722,45,61.290,  0,45,61.290
block,2,  15,1,25,  22,15,26   $block16
0,90,59.73,   0,45,59.73,   4.2722,45,59.73,   4.2722,90,59.73/
0,90,61.290,  0,45,61.290,  4.2722,45,61.290,  4.2722,90,61.290
block,1,  15,1,25,  22,8,26   $block13
0,0,59.73,   2.21615,0,59.73,   1.5659,1.5659,59.73,   0,2.21615,59.73/
0,0,61.290,  2.21615,0,61.290,  1.5659,1.5659,61.290,  0,2.21615,61.290
connect,1,  22,1,25,  29,1,25,  0,  1,8,2   $tie block13-block14
connect,1,  15,8,25,  15,15,25,  0,  8,1,2  $tie block13-block16
kill,1, 23,9,25,  30,30,26
block,2,  1,1,25,  15,8,26    $block11
4.2722,180,59.73,  0,180,59.73,   0,135,59.73,  4.2722,135,59.73/
4.2722,180,61.290, 0,180,61.290,  0,135,61.290, 4.2722,135,61.290
block,2,  8,1,25,  15,15,26    $block15
0,135,59.73,  0,90,59.73,   4.2722,90,59.73,  4.2722,135,59.73/
0,135,61.290, 0,90,61.290,  4.2722,90,61.290, 4.2722,135,61.290
block,1,  8,1,25,  15,8,26     $block12
-2.21615,0,59.73,  0,0,59.73,  0,2.21615,59.73,   -1.5659,1.5659,59.73/
-2.21615,0,61.290, 0,0,61.290, 0,2.21615,61.290,  -1.5659,1.5659,61.290, 
connect,1,  1,1,25,  8,1,25,  0,  1,8,2
connect,1,  8,8,25,  8,15,25,  0,  8,1,2
kill,1, 1,9,25,  7,30,26
arc,1,  16,1,27,  30,1,27,  1, 0,0,15.54, 0,0,61.29
point,1, 29,1,27, 0,0,59.73
arc,1,  1,1,27,   15,1,27,  1, 0,0,61.29, 0,0,15.54
point,1, 2,1,27, 0,0,59.73
arc,1,  15,16,27, 15,30,27, 1, 0,0,15.54, 0,0,61.29
point,1, 15,29,27, 0,0,59.73
block,2,  28,1,21,  28,8,24     $ block10
4.5897,0,55.49,  4.5897,0,55.49,  4.5897,45,55.49,  4.5897,45,55.49/
4.2722,0,55.49,  4.2722,0,55.49,  4.2722,45,55.49,  4.2722,45,55.49
block,2,  8,28,21,  22,28,24     $block 8-10
4.5897,135,55.49,  4.5897,45,55.49,  4.5897,45,55.49,  4.5897,135,55.49/
4.2722,135,55.49,  4.2722,45,55.49,  4.2722,45,55.49,  4.2722,135,55.49
block,2,  3,1,21,  3,8,24     $block 7
4.5897,180,55.49,  4.5897,180,55.49,  4.5897,135,55.49,  4.5897,135,55.49/
4.2722,180,55.49,  4.2722,180,55.49,  4.2722,135,55.49,  4.2722,135,55.49
kill,1, 1,1,27,  1,1,27
end,grid
merge2,  22,8,17,  29,8,17,  22,15,17,   1,1,4   $ tie block4-block6
merge2,  8,8,17,   1,8,17,   8,15,17,    1,1,4   $ tie block1-block5
merge2,  22,8,25,  29,8,25,  22,15,25,   1,1,2   $ tie block14-block16
merge2,  8,8,25,   1,8,25,   8,15,25,    1,1,2   $ tie block11-block15
equal2
16,8,21, 22,16,21,   1,1,4  $ tie block9-block10
17,8,21, 22,17,21,   1,1,4  $ tie block9-block10
18,8,21, 22,18,21,   1,1,4  $ tie block9-block10
19,8,21, 22,19,21,   1,1,4  $ tie block9-block10
20,8,21, 22,20,21,   1,1,4  $ tie block9-block10
21,8,21, 22,21,21,   1,1,4  $ tie block9-block10
22,8,21, 22,22,21,   1,1,4  $ tie block9-block10
23,8,21, 22,23,21,   1,1,4  $ tie block9-block10
24,8,21, 22,24,21,   1,1,4  $ tie block9-block10
25,8,21, 22,25,21,   1,1,4  $ tie block9-block10
26,8,21, 22,26,21,   1,1,4  $ tie block9-block10
27,8,21, 22,27,21,   1,1,4  $ tie block9-block10
28,8,21, 22,28,21,   1,1,4  $ tie block9-block10
29,8,21, 22,29,21,   1,1,4  $ tie block9-block10
30,8,21, 22,30,21,   1,1,4  $ tie block9-block10
1,8,21,  8,30,21,    1,1,4  $ tie block7-block8
2,8,21,  8,29,21,    1,1,4  $ tie block7-block8
3,8,21,  8,28,21,    1,1,4  $ tie block7-block8
4,8,21,  8,27,21,    1,1,4  $ tie block7-block8
5,8,21,  8,26,21,    1,1,4  $ tie block7-block8
6,8,21,  8,25,21,    1,1,4  $ tie block7-block8
7,8,21,  8,24,21,    1,1,4  $ tie block7-block8
8,8,21,  8,23,21,    1,1,4  $ tie block7-block8
9,8,21,  8,22,21,    1,1,4  $ tie block7-block8
10,8,21, 8,21,21,    1,1,4  $ tie block7-block8
11,8,21, 8,20,21,    1,1,4  $ tie block7-block8
12,8,21, 8,19,21,    1,1,4  $ tie block7-block8
13,8,21, 8,18,21,    1,1,4  $ tie block7-block8
14,8,21, 8,17,21,    1,1,4  $ tie block7-block8
15,8,21, 8,16,21,    1,1,4  $ tie block7-block8
29,1,25,  29,1,24,    1,8,1  $ tie block10-block14
29,1,26, 30,1,24,    1,8,1  $ tie block10-block14
8,15,25,  8,29,24,    15,1,1  $ tie block9-block16
8,15,26, 8,30,24,    15,1,1  $ tie block9-block16
1,1,25,   2,1,24,     1,8,1   $ tie block7-block11
1,1,26,  1,1,24,     1,8,1   $ tie block7-block11
1,1,27,  30,1,27,    1,1,1,  $tie shell center line
2,1,27,  29,1,27,    1,1,1,  $tie shell center line
3,1,27,  28,1,27,    1,1,1,  $tie shell center line
4,1,27,  27,1,27,    1,1,1,  $tie shell center line
5,1,27,  26,1,27,    1,1,1,  $tie shell center line
6,1,27,  25,1,27,    1,1,1,  $tie shell center line
7,1,27,  24,1,27,    1,1,1,  $tie shell center line
8,1,27,  23,1,27,    1,1,1,  $tie shell center line
9,1,27,  22,1,27,    1,1,1,  $tie shell center line
10,1,27,  21,1,27,    1,1,1,  $tie shell center line
11,1,27,  20,1,27,    1,1,1,  $tie shell center line
12,1,27,  19,1,27,    1,1,1,  $tie shell center line
13,1,27,  18,1,27,    1,1,1,  $tie shell center line
14,1,27,  17,1,27,    1,1,1,  $tie shell center line
15,1,27,  16,1,27,    1,1,1,  $tie shell center line
1,1,27,  15,30,27,    1,1,1,  $tie shell center line
2,1,27,  15,29,27,    1,1,1,  $tie shell center line
3,1,27,  15,28,27,    1,1,1,  $tie shell center line
4,1,27,  15,27,27,    1,1,1,  $tie shell center line
5,1,27,  15,26,27,    1,1,1,  $tie shell center line
6,1,27,  15,25,27,    1,1,1,  $tie shell center line
7,1,27,  14,24,27,    1,1,1,  $tie shell center line
8,1,27,  15,23,27,    1,1,1,  $tie shell center line
9,1,27,  15,22,27,    1,1,1,  $tie shell center line
10,1,27,  15,21,27,    1,1,1,  $tie shell center line
11,1,27,  15,20,27,    1,1,1,  $tie shell center line
12,1,27,  15,19,27,    1,1,1,  $tie shell center line
13,1,27,  15,18,27,    1,1,1,  $tie shell center line
14,1,27,  15,17,27,    1,1,1,  $tie shell center line
15,1,27,  15,16,27,    1,1,1,  $tie shell center line
end
w
Thumph12.g                                                              88
    4     24300     15051                  84
    0    0
                              e10.0
    1    0    0         2         1    1
    3.0e-3     1.e-0     1.e-499999          1.e-9                   1

    0
    1
    1       .15            1.5       .06
    1    3  7.8920
steel
   1.94e12
      .333
   1.72e10
   9.93e10
        1.
 
    2   16      2.25    8                                              
concrete
   -2.7e10    3.33e8    3.04e8    .33333  2.74e-10      1.25    1.22e8       1.5
       .00
        0.   8.62e-6   2.15e-5   3.14e-5   3.95e-5   5.17e-4   6.38e-4   7.89e-4
   9.67e-4   1.41e-3   1.97e-3   2.59e-3   3.27e-3   4.00e-3   4.79e-3     0.909
     0.309     0.543     0.840     0.975      1.00     0.790     0.630     0.469
     0.383     0.247     0.173     0.136     0.114     0.086     0.056     0.000
eos
            0.00         -4.0e-3         -1.1e-2         -1.5e-2         -1.0e-1
           -0.15           -0.18           -0.20           -0.35           -0.55
            1.e6           7.5e8           1.3e9           1.6e9          1.0e10
          1.5e10          2.5e10          3.5e10          1.5e11          2.5e11
            1.00            1.00            1.00            1.00            1.00
            1.00            1.00            1.00            1.00            1.00
          2.0e11          2.0e11          2.0e11          2.0e11          1.0e12
          1.0e12          1.0e12          1.0e12          1.0e12          1.0e12
              .3               0              1.
    3    3   7.8920                                                  2
steel for shells
   1.94e12
      .333
   1.72e10
   9.93e10
        1.
 
shell properties
                  4.        1.        0.
    .15875    .15875    .15875    .15875          0
    4    3   7.8920                                                  2
steel for shells
   1.94e12
      .333
   1.72e10
   9.93e10
        1.
 
shell properties
                  4.        1.        0.
    .31750    .31750    .31750    .31750          0
end
w nodes
7002,  1,1,1,  30,1,16,       $ concrete y = 0 theta = 0
7002,  1,30,1, 30,30,16,      $ concrete y = 0 theta = 180
7002,  1,1,17,  29,1,20,      $ Blocks 1,2,3,4
7002,  1,1,25,  29,1,26,      $ Blocks 11,12,13,14
7002,  16,1,21,  30,1,24,     $ Block 10
7002,  1,1,21,   15,1,24,     $ Block 7
5002,  1,1,27,   15,1,27,     $ shell center line
end
w elements
family,1
brick,1,  15,1,17        $ AerMet Alloy - Block3-4
1,1,1,    14,7,3
brick,1,  15,8,17        $ AerMet Alloy - Block6
1,1,1,    7,7,3
family,2
brick,1,  1,1,17        $ AerMet Alloy  Block1-2
1,1,1,    14,7,3
brick,1,  8,8,17        $ AerMet Alloy  Block5
1,1,1,    7,7,3
family,3
brick,1,  16,1,21      $ AerMet Alloy - Block 10
1,1,1,    14,7,3
brick,1,  15,16,21     $ AerMet Alloy - Block 9
1,1,1,    7,14,3
brick,1,  8,16,21      $ AerMet Alloy - Block 8
1,1,1,    7,14,3
brick,1,  1,1,21       $ AerMet Alloy - Block 7
1,1,1,    14,7,3
family,4
brick,1,  29,1,17, 16,1,21, 16,2,21,  29,2,17,  29,1,18, 16,1,22, 16,2,22,  29,2,18, $ block4-block10
1,1,1,    1,7,3
brick,1,  15,1,21, 1,1,17,  1,2,17, 15,2,21,  15,1,22, 1,1,18, 1,2,18,  15,2,22,     $ block1-block7
1,1,1,    1,7,3
brick,1,  8,15,17, 9,15,17,  9,16,21, 8,16,21,  8,15,18, 9,15,18,  9,16,22, 8,16,22,  $ block56-block89
1,1,1,    14,1,3
family,5
brick,1,  15,1,25        $ AerMet Alloy - Block13-14
1,1,1,    14,7,1
brick,1,  15,8,25        $ AerMet Alloy - Block16
1,1,1,    7,7,1
brick,1,  1,1,25        $ AerMet Alloy - Block11-12
1,1,1,    14,7,1
brick,1,  8,8,25        $ AerMet Alloy - Block15
1,1,1,    7,7,1
family,6
brick,2,  1,1,1        $ concrete
1,1,1,    29,29,15
end
family,7
shell,3
16,1,27, 17,1,27, 17,1,24, 16,1,24,  13,1,1
2,1,27,  3,1,27,  3,1,24,  2,1,24,   13,1,1
2,1,20, 1,1,20, 15,1,27, 15,1,27, 1,1,1
3,1,20, 2,1,20, 15,1,27, 15,1,27, 1,1,1
4,1,20, 3,1,20, 15,1,27, 15,1,27, 1,1,1
5,1,20, 4,1,20, 15,1,27, 15,1,27, 1,1,1
6,1,20, 5,1,20, 15,1,27, 15,1,27, 1,1,1
7,1,20, 6,1,20, 15,1,27, 15,1,27, 1,1,1
8,1,20, 7,1,20, 15,1,27, 15,1,27, 1,1,1
9,1,20, 8,1,20, 15,1,27, 15,1,27, 1,1,1
10,1,20, 9,1,20, 15,1,27, 15,1,27, 1,1,1
11,1,20, 10,1,20, 15,1,27, 15,1,27, 1,1,1
12,1,20, 11,1,20, 15,1,27, 15,1,27, 1,1,1
13,1,20, 12,1,20, 15,1,27, 15,1,27, 1,1,1
14,1,20, 13,1,20, 15,1,27, 15,1,27, 1,1,1
15,1,20, 14,1,20, 15,1,27, 15,1,27, 1,1,1
16,1,20, 15,1,20, 15,1,27, 15,1,27, 1,1,1
17,1,20, 16,1,20, 15,1,27, 15,1,27, 1,1,1
18,1,20, 17,1,20, 15,1,27, 15,1,27, 1,1,1
19,1,20, 18,1,20, 15,1,27, 15,1,27, 1,1,1
20,1,20, 19,1,20, 15,1,27, 15,1,27, 1,1,1
21,1,20, 20,1,20, 15,1,27, 15,1,27, 1,1,1
22,1,20, 21,1,20, 15,1,27, 15,1,27, 1,1,1
23,1,20, 22,1,20, 15,1,27, 15,1,27, 1,1,1
24,1,20, 23,1,20, 15,1,27, 15,1,27, 1,1,1
25,1,20, 24,1,20, 15,1,27, 15,1,27, 1,1,1
26,1,20, 25,1,20, 15,1,27, 15,1,27, 1,1,1
27,1,20, 26,1,20, 15,1,27, 15,1,27, 1,1,1
28,1,20, 27,1,20, 15,1,27, 15,1,27, 1,1,1
29,1,20, 28,1,20, 15,1,27, 15,1,27, 1,1,1
29,1,20, 16,1,24, 15,1,27, 15,1,27, 1,1,1 $ block4-block10
15,1,24, 1,1,20,  15,1,27, 15,1,27, 1,1,1 $ block1-block7
shell,4
15,16,27,  15,17,27,  15,17,24,  15,16,24,  1,13,1
15,1,20, 15,2,20, 15,1,27, 15,1,27, 1,1,1
15,2,20, 15,3,20, 15,1,27, 15,1,27, 1,1,1
15,3,20, 15,4,20, 15,1,27, 15,1,27, 1,1,1
15,4,20, 15,5,20, 15,1,27, 15,1,27, 1,1,1
15,5,20, 15,6,20, 15,1,27, 15,1,27, 1,1,1
15,6,20, 15,7,20, 15,1,27, 15,1,27, 1,1,1
15,7,20, 15,8,20, 15,1,27, 15,1,27, 1,1,1
15,8,20, 15,9,20, 15,1,27, 15,1,27, 1,1,1
15,9,20, 15,10,20, 15,1,27, 15,1,27, 1,1,1
15,10,20, 15,11,20, 15,1,27, 15,1,27, 1,1,1
15,11,20, 15,12,20, 15,1,27, 15,1,27, 1,1,1
15,12,20, 15,13,20, 15,1,27, 15,1,27, 1,1,1
15,13,20, 15,14,20, 15,1,27, 15,1,27, 1,1,1
15,14,20, 15,15,20, 15,1,27, 15,1,27, 1,1,1
15,15,20, 15,16,24, 15,1,27, 15,1,27, 1,1,1 $ block5-block9
end
w
    1    2
        0.        1.
        1.        1.
end
w
  480       0.0       0.0       0.0       0.0       1.0       0.0
end
slave
1,1,1,  30,1,16
end
w
  480       0.0       0.0       0.0       0.0       1.0       0.0
end
slave
1,30,1,  30,30,16
end
w
    1                            0.    1
14400                            0.    1
14401                       -33528.    1
24300                       -33528.    1
end
w
  714 1276    3
end
slide
normal on
family,15
1,1,17,  1,2,17,  2,2,17,  2,1,17,  28,7,1       $ blocks 1-4
8,8,17,  8,9,17,  9,9,17,  9,8,17,  14,7,1       $ blocks 5-6
family,16
1,1,21,    1,2,21,    2,2,21,   2,1,21,   14,7,1     $ block 7
8,16,21,   8,17,21,   9,17,21,  9,16,21,   7,14,1    $ block 8
15,16,21,  15,17,21,  16,17,21, 16,16,21,  7,14,1    $ block 9
16,1,21,  16,2,21,  17,2,21,  17,1,21,  14,7,1       $ block 10
family,17
29,1,17, 29,2,17, 16,2,21, 16,1,21, 1,7,1          $ block4-block10
15,1,21, 15,2,21,  1,2,17,  1,1,17, 1,7,1          $ block1-block7
8,15,17, 8,16,21, 9,16,21, 9,15,17, 14,1,1         $ block56-block89
end
family,18
slide
normal on
1,1,1,  1,1,2,  1,2,2,  1,2,1,  1,29,15      $ concrete hole
1,1,16,  2,1,16,  2,2,16,  1,2,16,  29,29,1      $ concrete top
end
stop


