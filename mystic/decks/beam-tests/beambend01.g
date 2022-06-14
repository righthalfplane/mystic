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
beambend01.g
poff stre 
pon plot dof node equl
poff elem  comp
exit
nodes3d
7,  1,1,1,  5,5,1
end
mat,1
0 2 1 1 1 1.9e12 0.3 0. 7.85 $steel cgs
end
elemm3
brick,1,  1,1,1
1,1,1, 4,4,84
end
w
form
exit
load3
0., -1.,0,  1,1,85, 1,1,85
0., -1.,0,  5,5,85, 5,5,85
0., -1.,0,  1,5,85, 1,5,85
0., -1.,0,  5,1,85, 5,1,85
0., -2.,0,  1,2,85, 1,4,85
0., -2.,0,  5,2,85, 5,4,85
0., -2.,0,  2,1,85, 4,1,85
0., -2.,0,  2,5,85, 4,5,85
0., -4.,0,  2,2,85, 4,4,85
end
w
solv,stre,1,end
stop
exit
stop
