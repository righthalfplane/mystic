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
     double xc,yc,zc,t;
     double xn,yn,zn;
     
     r=50;
     pi=4.0*atan(1.0);
     
     
     nx=5;
     ny=5;
     nz=85;
     nc=0;

     for(nk=0;nk<nz;++nk){
        theta=2*pi*nk/(nz-1);
        ni=0;
		for(t=-2.0/10;t<=2.0/10;t+=1.0/10){
		    ++ni;
			xc=cos(theta)+t*cos(theta/2.0)*cos(theta);
			yc=sin(theta)+t*cos(theta/2.0)*sin(theta);
			zc=t*sin(theta/2);
			for(nj=0;nj<ny;++nj){
			    yy=nj*0.01-0.02;
			    /* printf("yy %f\n",yy); */
			    xn=0.5*sin(theta/2.0)*(-t*cos(theta/2.0)+2*cos(theta)+t*cos(3*theta/2));
			    yn=0.25*(t+2*cos(theta/2.0)+2*t*cos(theta)-2*cos(3*theta/2)-t*cos(2*theta));
			    zn= -cos(theta/2.0)*(1+t*cos(theta/2.0));
			    
			    x[ni+nj*nx+nk*nx*ny]=xc+xn*yy;
              	y[ni+nj*nx+nk*nx*ny]=yc+yn*yy;
              	z[ni+nj*nx+nk*nx*ny]=zc+zn*yy;
              	
              	
              	/*
			    x[ni+nj*nx+nk*nx*ny]=xc;
              	y[ni+nj*nx+nk*nx*ny]=yc;
              	z[ni+nj*nx+nk*nx*ny]=zc;
              	*/
				++nc;
				/* printf("%d %f %f %f \n",nc,xc,yc,zc); */
			}
		}
	}
	
	/* printf("nc %d\n",nc); */
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


7,  1,1,1,    5,5,1
7,  1,1,85,    5,5,85



w
stop
exit
stop


