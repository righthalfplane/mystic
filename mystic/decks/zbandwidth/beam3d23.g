main()
{
	fmain();
}
end
setup,1,6,3,3
block,1,  1,1,1,  6,3,3
0,-5,-8,       35,-5,-8,      35,5,-8,      0,5,-8/
0,-5,8,        35,-5,8,       35,5,8,       0,5,8
end,grid
w
beam3d23.g
poff stre 
pon plot dof node equl
poff elem  comp
pnt,50,1,1
19,19,1
end
exit
nodes3d
7,  1,1,1,    1,3,3
end
mat,1
0 2 1 1 1 1.9e12 0.3 0. 7.85 $steel cgs
end
elemm3
brick,1,  1,1,1
1,1,1, 5,2,2
end
w
tloa 4 4 1
nfun 1 4
0. 0.0032 0.00321 1e3
-2500. -2500. 0. 0.
nfun 2 4
0. 0.0032 0.00321 1e3
-5000. -5000. 0. 0.
nfun 3 4
0. 0.0032 0.00321 1e3
-10000. -10000. 0. 0.
nfun 4 4
0. 0.0032 0.00321 1e3
-10000. -10000. 0. 0.
exit
w
form
exit
load3
0.,0, 4.,  6,2,2,    6,2,2
end
w
solv,stre,1,end
stop
exit
stop

