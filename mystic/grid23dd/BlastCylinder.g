main()
{
	fmain();
}
end
setup,1, 7,41,46
csystm,6,2,  0,0,40, 0,0,-1.0, 0,1.0,0
block,2,  1,1,1,  7,41,46, 
7.0,0,0,     10.,0,0,     10.,180,0,   7.0,180,0/
7.0,0,20,    10.,0,20,    10.,180,20,  7.0,180,20
end,grid
w nodes
0002,  1,1,1,   7,41,1           $ front edge of explosive
0002,  1,1,46,  7,41,46         $ left edge of explosive
end
w elements
brick,1,  1,1,1
1,1,1,  6,40,45
end
surface
normal on
family,5
7,1,1,  7,2,1,  7,2,2,  7,1,2,  1,40,45      $ cylinder surface
end
stop
