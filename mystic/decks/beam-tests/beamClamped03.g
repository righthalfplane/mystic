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
beamClamped03.g
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
0., -1.,0,  1,1,1, 1,1,85
0., -1.,0,  15,1,1, 15,1,85
0., -2.,0,  2,1,1, 14,1,85
end
w
solv,stre,1,end
stop
exit
stop
