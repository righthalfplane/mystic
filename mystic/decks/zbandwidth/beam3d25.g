main()
{
	fmain();
}
end
setup,1,5,5,8
block,1,  1,1,1,  5,5,8
0,0,0,        4,0,0,       4,4,0,       0,4,0/
0,0,8,        4,0,8,       4,4,8,       0,4,8
end,grid
w
beam3d25.g
poff stre 
pon stre 
pon plot dof node equl
poff elem  comp
exit
nodes3d
1,  1,1,1,  1,5,8
1,  5,1,1,  5,5,8
2,  1,1,1,  5,1,8
2,  1,5,1,  5,5,8
3,  1,1,1,  1,1,8
3,  5,1,1,  1,5,8
3,  1,5,1,  1,5,8
3,  5,5,1,  5,5,8
7,  1,1,1,  5,5,1
end
mat,1
0 2 1 1 1 8.00e10 .2 0. 2.0
end
elemm3
brick,1,  1,1,1
1,1,1, 4,4,7
end
w
form
exit
load3
0, 0., -1., 1,1,8, 1,1,8
0, 0., -1., 5,5,8, 5,5,8
0, 0., -1., 1,5,8, 1,5,8
0, 0., -1., 5,1,8, 5,1,8
0, 0., -2., 1,2,8, 1,4,8
0, 0., -2., 5,2,8, 5,4,8
0, 0., -2., 2,1,8, 4,1,8
0, 0., -2., 2,5,8, 4,5,8
0, 0., -4., 2,2,8, 4,4,8
end
w
solv,stre,1,end
stop
exit
stop
