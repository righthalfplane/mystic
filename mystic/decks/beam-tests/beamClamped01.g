main()
{
	fmain();
}
end
setup,1,15,15,85
block,1,  1,1,1,  15,15,85
0,0,0,         15,0,0,        15,15,0,        0,15,0/
0,0,85,        15,0,85,       15,15,85,       0,15,85
end,grid
w
beambend01.g
poff stre 
pon plot dof node equl
poff elem  comp
exit
nodes3d
7,  1,1,1,  15,15,1
7,  1,1,85,  15,15,85
end
mat,1
0 2 1 1 1 1.9e12 0.3 0. 7.85 $steel cgs
end
elemm3
brick,1,  1,1,1
1,1,1, 14,14,84
end
w
form
exit
load3
0., -1.,0,  1,1,43, 1,1,43
0., -1.,0,  15,15,43, 15,15,43
0., -1.,0,  1,15,43, 1,15,43
0., -1.,0,  15,1,43, 15,1,43
0., -2.,0,  1,2,43, 1,14,43
0., -2.,0,  15,2,43, 15,14,43
0., -2.,0,  2,1,43,  14,1,43
0., -2.,0,  2,15,43, 14,15,43
0., -4.,0,  2,2,43,  14,14,43
end
w
solv,stre,1,end
stop
exit
stop