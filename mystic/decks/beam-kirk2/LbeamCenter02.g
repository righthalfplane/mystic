main()
{
	fmain();
}
end
setup,1,8,22,1133
block,1,  1,1,1,  4,22,1133
0,0,0,         0.18,0,0,        0.18,1.5,0,        0,1.5,0/
0,0,67.98,        0.18,0,67.98,       0.18,1.5,67.98,       0,1.5,67.98
csystm,5, 1, 0.18,0.18,0.0,  0,-1,0,  0,0,1
block,5,  5,1,1,  8,22,1133
0,0,0,         0.18,0,0,        0.18,1.32,0,        0,1.32,0/
0,0,67.98,        0.18,0,67.98,       0.18,1.32,67.98,       0,1.32,67.98
end,grid
equal2
4,4,1, 5,1,1, 1,1,1133
4,3,1, 6,1,1, 1,1,1133
4,2,1, 7,1,1, 1,1,1133
4,1,1, 8,1,1, 1,1,1133
end
w
angle01.g
poff stre 
pon plot dof node equl
poff elem  comp
exit
nodes3d
7,  1,1,1,  3,22,1
7,  5,1,1,  8,22,1
7,  1,1,1133,  3,22,1133
7,  5,1,1133,  8,22,1133
end
mat,1
0 2 1 1 1 29e6 0.3 0. 7.85 $steel cgs
end
elemm3
brick,1,  1,1,1
1,1,1, 3,21,1132
brick,1,  5,1,1
5,1,1, 7,21,1132
end
w
form
exit
load3
0., -0.29091,0,  5,1,555, 5,22,579
end
w
solv,stre,1,end
stop
exit
stop


