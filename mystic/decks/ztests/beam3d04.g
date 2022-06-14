main()
{
	fmain();
}
end
setup,1,85,11,11
block,1,  1,1,1,  85,11,11
0,0,0,        85,0,0,       85,10,0,       0,10,0/
0,0,10,       85,0,10,      85,10,10,      0,10,10
end,grid
w
beam3d01.g
poff stre 
pon plot dof node equl
poff elem  comp
exit
nodes3d
7,  1,1,1,  1,11,11
end
mat,1
0 2 1 1 1 8.00e10 .2 0. 2.0
end
elemm3
brick,1,  1,1,1
1,1,1, 84,10,10
end
w
tloa 2 4 1
nfun 1 4
0.    0.0112  .01121    1e3
-500. -500.      0.0    0.0
nfun 2 2
0. 1e3
-1000. -1000.
exit
w
form,1,0,2,2
exit
load3
0, 0,  1.0, 85,6,6, 85,6,6
end
w
mar 100 800e-6 1
end
end
end
stop
exit
stop
