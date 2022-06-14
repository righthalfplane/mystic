main()
{
	fmain();
}
end
setup,1,5,4,4
block,1,  1,1,1,  5,4,4
0,0,0,        5,0,0,       5,4,0,       0,4,0/
0,0,4,        5,0,4,       5,4,4,      0,4,4
end,grid
w
beam3d01.g
poff stre 
pon plot dof node equl
poff elem  comp
exit
nodes3d
7,  1,1,1,  1,4,4
end
mat,1
0 2 1 1 1 8.00e10 .2 0. 2.0
end
elemm3
brick,1,  1,1,1
1,1,1, 4,3,3
end
w
tloa 2 2 1
nfun 1 2
0. 1e3
-500. -500.
nfun 2 2
0. 1e3
-1000. -1000.
exit
w
form,1,0,2,2
exit
load3
-1., 0., 0., 5,1,1, 5,4,4
end
w
mar 100 8e-6 1
end
end
end
stop
exit
stop
