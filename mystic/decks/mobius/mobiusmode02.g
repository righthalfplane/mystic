double atan2(),sqrt(),sin(),cos(),atan(),fabs();
main()
{
	fmain();
}
grid(x,y,z,n)
float *x,*y,*z;
long *n;
{
     int nx,ny,nz,ni,nj,nk;

     nx=5;
     ny=5;
     nz=85;
     for(nk=0;nk<nz;++nk){
        for(nj=0;nj<ny;++nj){
            for(ni=0;ni<nx;++ni){
            /*   z[ni+nj*nx+nk*nx*ny]=z[ni+nj*nx+nk*nx*ny]+10.0; */
              z[ni+nj*nx+nk*nx*ny]=10.0;
               printf("nk %ld z = %f\n",nk,z[ni+nj*nx+nk*nx*ny]);
            }
        }
    }
}
end
setup,1,5,5,85
block,1,  1,1,1,  5,5,85
0,0,0,         15,0,0,        15,5,0,        0,5,0/
0,0,85,        15,0,85,       15,5,85,       0,5,85
end,grid
w
mobiusmode01.g
poff stre 
pon plot dof node equl
poff elem  comp
pnt,50,1,1
191,191,1
end
exit
nodes3d
7,  1,1,1,    5,5,1
end
mat,1
2 2 1 1 15 1.9e12 0.3 0. 7.85 $steel cgs
end
elemm3
brick,1,  1,1,1
1,1,1, 4,4,84
end
w
mode 5 0 0 5
stop
exit
stop


w
stop
exit
stop


