main()
{
	fmain();
}
end
setup,1,85,11,1
face,1,  1,1,1,  85,11,1,  $ kill graidents 1,4,1,4
0,0,0,       85,0,0,       85,10,0,       0,10,0
end,grid
w
beam01.g
poff stre 
pon plot dof node equl
poff elem  comp
exit
nodes
2,  1,1,  85,1
2,  1,11, 85,11
3,  1,1, 1,11
end
mat,1
0 2 1 1 1 1. 8.00e10 .2 0. 2.0
end
elemm
quad,1,  1,1
84,10
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
load
2. 0. 85,2,  85,10
1. 0. 85,1,  85,1
1. 0. 85,11, 85,11
end
w
mar 100 8e-6 1
end
end
end
stop
exit
stop
