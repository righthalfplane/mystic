double atan2(),sqrt(),sin(),cos(),atan(),fabs();
main()
{
	fmain();
}
grid(x,y,z,n)
float *x,*y,*z;
long *n;
{
     int nx,ny,nz,ni,nj,nk,nc;
     double xx,yy,zz;
     double r,pi,theta;
     
     r=50;
     pi=4.0*atan(1.0);
     
     
     nx=5;
     ny=5;
     nz=85;
     nc=0;
     for(nk=0;nk<nz;++nk){
        theta=2*pi*nk/(nz-1);
        zz=nk;
        for(nj=0;nj<ny;++nj){
            yy=nj*0.5;
            for(ni=0;ni<nx;++ni){
              xx=ni*5;
              x[ni+nj*nx+nk*nx*ny]=xx;
              y[ni+nj*nx+nk*nx*ny]=(r+yy)*cos(theta);
              z[ni+nj*nx+nk*nx*ny]=(r+yy)*sin(theta);
              nc++;
           }
        }
    }
    
         printf("%f %f %f %ld\n",x[0],y[0],z[0],*n);
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
7,  1,1,85,    5,5,85
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


