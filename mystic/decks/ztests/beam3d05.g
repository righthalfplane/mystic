main()
{
	fmain();
}
end
setup,1,85,5,5
block,1,  1,1,1,  85,5,5
0,0,0,        85,0,0,       85,4,0,       0,4,0/
0,0,4,       85,0,4,      85,4,4,      0,4,4
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
1,1,1, 84,4,4
end
w
tloa 3 2 1
nfun 1 2
0. 1e3
-500. -500.
nfun 2 2
0. 1e3
-1000. -1000.
nfun 3 2
0. 1e3
-250. -250.
exit
w
form,1,0,2,2
exit
load3
3, 0., 0., 85,1,1, 85,1,1
3, 0., 0., 85,5,5, 85,5,5
3, 0., 0., 85,1,5, 85,1,5
3, 0., 0., 85,5,1, 85,5,1
1, 0., 0., 85,1,2, 85,1,4
1, 0., 0., 85,5,2, 85,5,4
1, 0., 0., 85,2,1, 85,4,1
1, 0., 0., 85,2,5, 85,4,5
2, 0., 0., 85,2,2, 85,4,4
end
w
mar 200 8e-6 1
end
end
end
stop
exit
stop
