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
wall3d01.g
poff stre 
pon stre 
pon plot dof node equl
poff elem  comp
pnt,1,1,1
18900,19125,1
end
exit
nodes3d
4,  1,1,1,     15,15,1
4,  1,1,85,    15,15,85
1,  1,1,1,     1,15,85
1,  15,1,1,    15,15,85
5,  1,1,1,     1,15,1
5,  15,1,1,    15,15,1
5,  1,1,85,    1,15,85
5,  15,1,85,   15,15,85
7,  1,1,1,     15,1,85
end
mat,1
0 2 1 1 1 1.9e12 0.3 0. 7.85 $steel cgs
end
elemm3
brick,1,  1,1,1
1,1,1, 14,14,84
end
w
tloa 4 4 1
nfun 1 4
0. 0.0032 0.00321 1e3
-2500. -2500. 0. 0.
nfun 2 4
0. 0.0032 0.00321 1e3
-5000. -5000. 0. 0.
nfun 3 4
0. 0.0032 0.00321 1e3
-10000. -10000. 0. 0.
nfun 4 4
0. 0.0032 0.00321 1e3
-10000. -10000. 0. 0.
exit
w
form
exit
load3
0.,4.0, 0.,  2,15,2,     14,15,84
0.,2.0, 0.,  1,15,2,     1,15,84
0.,2.0, 0.,  15,15,2,    15,15,84
0.,2.0, 0.,  2,15,85,    14,15,85
0.,2.0, 0.,  2,15,1,     14,15,1
0.,1.0, 0.,  1,15,1,     1,15,1
0.,1.0, 0.,  1,15,85,    1,15,85
0.,1.0, 0.,  15,15,1,    15,15,1
0.,1.0, 0.,  15,15,85,   15,15,85
end
w
solv,stre,1,end
stop
exit
stop
