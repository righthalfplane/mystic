main()
{
	fmain();
}
end
setup,1,6,3,1
face,1,  1,1,1,  6,3,1,  $ kill graidents 1,4,1,4
0,0,0,       6,0,0,       6,3,0,       0,3,0
end,grid
w
beam02.g
poff stre 
pon plot dof node equl
poff elem  comp
exit
nodes
3,  1,1, 1,3
end
mat,1
0 2 1 1 1 1. 8.00e10 .2 0. 2.0
end
elemm
quad,1,  1,1
5,2
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
2. 0. 6,1,  6,3
end
w
mar 10 8e-6 1
end
end
end
stop
exit
stop
