#include <stdio.h>
#include "showlib.h"

static showPtr show;

#define EMAX4 500000
static long *elem4;
static long nelem4;
#define Max_Nodes 1000000
static double *x,*y,*z,*v;
static double *nx,*ny,*nz;
static long nn,ne;
static long nodes,triangles;
int main()
{
	int ret;

	ret=1;
	elem4=(long *)malloc(EMAX4*sizeof(long));
	if(!elem4){
	   printf("No Element Memory\n");
	   return 1;
	}
	
	x=(double *)malloc(Max_Nodes*sizeof(double));
	if(!x){
	   printf("No x Memory\n");
	   return 1;
	}
	
	y=(double *)malloc(Max_Nodes*sizeof(double));
	if(!y){
	   printf("No y Memory\n");
	   return 1;
	}
	
	z=(double *)malloc(Max_Nodes*sizeof(double));
	if(!z){
	   printf("No z Memory\n");
	   return 1;
	}
	
	
	v=(double *)malloc(Max_Nodes*sizeof(double));
	if(!v){
	   printf("No v Memory\n");
	   return 1;
	}
	
	nx=(double *)malloc(Max_Nodes*sizeof(double));
	if(!nx){
	   printf("No nx Memory\n");
	   return 1;
	}
	
	ny=(double *)malloc(Max_Nodes*sizeof(double));
	if(!ny){
	   printf("No ny Memory\n");
	   return 1;
	}
	
	nz=(double *)malloc(Max_Nodes*sizeof(double));
	if(!nz){
	   printf("No nz Memory\n");
	   return 1;
	}
	
	
	show=showStart("Image2.shw","w");
	if(!show)return 1;
	
	triangles = 0;
	nodes = 0;
	nelem4=0;
	nn=0;
	ne=0;
	
	if(Translate("test.stl"))goto ErrorOut;
	
	printf("nodes %ld triangles %ld\n",nodes,triangles);
	
	if(nn > 0 && ne > 0){
	
	    ret=showNodes(show,nn,x,y,z);
	    if(ret)goto ErrorOut;
	    
	    ret=showNormals(show,nn,nx,ny,nz);
	    if(ret)goto ErrorOut;
/*
	    ret=showValues(show,nn,v);
	    if(ret)goto ErrorOut;
*/
        ret=showElements(show,ne,4L,elem4);
	    if(ret)goto ErrorOut;
	
	    ret=showEnd(show);
	    if(ret)goto ErrorOut;
	
	}
	ret=0;
	
ErrorOut:

	
	showDone(show);
	
	return ret;
		
}
int Translate(char *name)
{
	double xx,yy,zz;
	double nxx,nyy,nzz;
	char buff[256];
	FILE *out;
	double xmin = 1e30,xmax = -1e30;
	double ymin = 1e30,ymax = -1e30;
	double zmin = 1e30,zmax = -1e30;
	long count;
	int ojbect;
	int ret;
	int nc;
	int c;
	int n;
	
	
	ret = 1;
	
	out=fopen(name,"r");
	if(!out){
	    printf("Could not open %s to read \n",name);
	    goto ErrorOut;
	}
	
	ojbect=0;
	
	count=0;
	
	
	nxx=0;
	nyy=0;
	nzz=1;
	nc = 0;
	while(fgets(buff,256,out)){
		count++;
		for(n=0;n<256;++n){
		    c=buff[n];
		    if(c == 'v' && buff[n+1] == 'e' && buff[n+2] == 'r'){
		        sscanf(&buff[n]+6,"%lf %lf %lf",&xx,&yy,&zz);
		        if(nn+1 < Max_Nodes){
		            if(xx > xmax)xmax = xx;
		            if(yy > ymax)ymax = yy;
		            if(zz > zmax)zmax = zz;
		            if(xx < xmin)xmin = xx;
		            if(yy < ymin)ymin = yy;
		            if(zz < zmin)zmin = zz;
		            x[nn]=xx;
		            y[nn]=yy;
		            z[nn]=zz;		
		            nx[nn]=nxx;
		            ny[nn]=nyy;
		            nz[nn]=nzz;		
		            v[nn++]=ojbect;		
					++nodes;
		        }else{
		            printf("Too many Nodes %ld\n",nn);
		            return 0;
		        }
		        if(++nc == 3){
		            nc = 0;
		            if(nelem4+6 < EMAX4){
		                elem4[nelem4++] = 1;
		                elem4[nelem4++] = nn-3;
		                elem4[nelem4++] = nn-2;
		                elem4[nelem4++] = nn-1;		            
		                elem4[nelem4++] = -1;	            
		                ++ne;
						++triangles;
		            }else{
		                printf("Too many Elements %ld\n",nelem4);
		                return 0;
		            }
		            
		        }   
		        break;
		    }else if(c == 's'  && buff[n+1] == 'o' && buff[n+2] == 'l'){
		        ojbect++;
/*		        
		        if(nn > 0){
					ret=showNodes(show,nn,x,y,z);
					if(ret)goto ErrorOut;
			
					ret=showValues(show,nn,v);
					if(ret)goto ErrorOut;
			
					ret=showElements(show,ne,4L,elem4);
					if(ret)goto ErrorOut;
				
					ret=showEnd(show);
					if(ret)goto ErrorOut;
					
					nc = 0;
					nn = 0;
					ne = 0;
					nelem4 = 0;
					
					printf("nodes %ld triangles %ld\n",nodes,triangles);
		        
		        }
*/		        
		        break;
		    }else if(n == 2 && c == 'f' && buff[n+1] == 'a' && buff[n+2] == 'c'){
		        sscanf(&buff[n]+12,"%lf %lf %lf",&nxx,&nyy,&nzz);
		        /*
		        printf("nxx %f %f %f\n",nxx,nyy,nzz);
		        */
		        break;
		    }
		}
	}
	printf("count %ld ojbect %d\n",count,ojbect);
	printf("xmin %g xmax %g\n",xmin,xmax);
	printf("ymin %g ymax %g\n",ymin,ymax);
	printf("zmin %g zmax %g\n",zmin,zmax);
	
	ret = 0;
	
ErrorOut:
	
	return ret;	
}
