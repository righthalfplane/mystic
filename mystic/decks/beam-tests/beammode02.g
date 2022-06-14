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
beammode01.g
poff stre 
pon plot dof node equl
poff elem  comp
pnt,50,1,1
19110,19125,1
end
exit
nodes3d
7,  1,1,1,    15,15,1
2,  1,1,1,    15,1,85
2,  1,15,1,   15,15,85
end
mat,1
2 2 1 1 15 1.9e12 0.3 0. 7.85 $steel cgs
end
elemm3
brick,1,  1,1,1
1,1,1, 14,14,84
end
w
mode 3 0 0 3
stop
exit
stop


w
stop
exit
stop


