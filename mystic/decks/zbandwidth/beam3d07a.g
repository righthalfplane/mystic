main()
{
	fmain();
}
end
setup,1,5,5,85
block,1,  1,1,1,  5,5,85
0,0,0,         4,0,0,        4,4,0,        0,4,0/
0,0,85,        4,0,85,       4,4,85,       0,4,85
end,grid
w
beam3d01.g
poff stre 
pon plot dof node equl
poff elem  comp
exit
nodes3d
7,  1,1,1,  5,5,1
end
mat,1
0 2 1 1 1 8.00e10 .2 0. 2.0
end
elemm3
brick,1,  1,1,1
1,1,1, 4,4,84
end
w
mode 6 0 0 6
stop
exit
stop
