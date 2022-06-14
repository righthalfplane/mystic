#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "showlib.h"

#ifdef CRAY
#define esbgn ESBGN
#define esend ESEND
#define eselm ESELM
#define eselm2 ESELM2
#define esdis ESDIS
#define esnod ESNOD
#define eshead ESHEAD
#define esstr ESSTR
#define esnodes ESNODES
#define esmode ESMODE
#define estime ESTIME
#define INC	1
#else
#define esbgn esbgn_
#define esend esend_
#define eselm eselm_
#define eselm2 eselm2_
#define esdis esdis_
#define esnod esnod_
#define eshead eshead_
#define esstr esstr_
#define esnodes esnodes_
#define esmode esmode_
#define estime estime_

#define INC	1
#endif

static char name[256];
static long plot=0;
static FILE *out;

int eshead(char *name,long long *value);

#define EMAX4 800000
#define EMAX8 900000

static long *elem4;
static double *elem4value;
static double *elem4nodes;
static double *elem4dis;
static long nelem4;
static long nelem4nodes;
static long nelem4value;
static long *elem8;
static double *elem8value;
static long nelem8;
static long nelem8value;
static long nelem4dis;
static double *elem8nodes;
static long nelem8nodes;
static double *elem8dis;
static long nelem8dis;

static double *modex;
static double *modey;
static double *modez;

static double *nodesx;
static double *nodesy;
static double *nodesz;

static showPtr show;

int esbgn();

int neselm2;
int nemode;
long modeCount;
long nodeCount;

int estime(double *time)
{
	if(!show)return 1;

	show->time=*time;
	
	return 0;
}
int esmode(double *x,long long *nodeCounti)
{
	long nodeC;
	
	if(!x || !nodeCounti)return 1;
	
	nodeC=*nodeCounti;
	
	nemode=1;
	
	modeCount=nodeCount;

	
//	fprintf(stderr,"esmode nodeC %ld modeCount %ld nemode %d\n",nodeC,modeCount,nemode);
	
	if(!modex){
		modex=(double *)calloc(modeCount*sizeof(double),1);
		modey=(double *)calloc(modeCount*sizeof(double),1);
		modez=(double *)calloc(modeCount*sizeof(double),1);
		if(!modex || !modey || !modez)return 1;
	}
	if(nodeC <= modeCount){
	   modex[nodeC-1]=x[0];
	   modey[nodeC-1]=x[1];
	   modez[nodeC-1]=x[2];
	}
	
	return 0;
	
}
int eselm2(long long *mat,long long *ip,long long *kni)
{
	static int error;
	long kn,n;

	if(!show)return 1;

	if(!mat || !ip || !kni)return 1;
	
	neselm2=1;

	kn = *kni;

	if(kn == 3)kn=4;

	if(kn == 4){
	    if((nelem4+5) >= EMAX4-1)return 0;
/*
	        if(ip[0] < 0 || ip[1*INC] < 0 || ip[2*INC] < 0 || ip[3*INC] < 0 ){
		        ip += 4*INC;
		        return 0;
	        }
*/
	        elem4[nelem4++]= *mat;
            elem4[nelem4++]= (*ip)-1;
	        ip += INC;
            elem4[nelem4++]= (*ip)-1;
	        ip += INC;
            elem4[nelem4++]= (*ip)-1;
 	        ip += INC;
            elem4[nelem4++]= (*ip)-1;
	        ip += INC;
	}else if(kn == 8){
	    if((nelem8+9) >= EMAX8-1)return 0;
/*
	        if(ip[0] < 0 || ip[1*INC] < 0 || ip[2*INC] < 0 || ip[3*INC] < 0 ||
	           ip[4*INC] < 0 || ip[5*INC] < 0 || ip[6*INC] < 0 || ip[7*INC] < 0 ){
		    ip += 8*INC;
		    return 0;
	        }
*/
	        elem8[nelem8++]= *mat;
            elem8[nelem8++]= (*ip)-1;
	        ip += INC;
            elem8[nelem8++]= (*ip)-1;
	        ip += INC;
            elem8[nelem8++]= (*ip)-1;
 	        ip += INC;
            elem8[nelem8++]= (*ip)-1;
	        ip += INC;
            elem8[nelem8++]= (*ip)-1;
	        ip += INC;
            elem8[nelem8++]= (*ip)-1;
	        ip += INC;
            elem8[nelem8++]= (*ip)-1;
 	        ip += INC;
            elem8[nelem8++]= (*ip)-1;
	        ip += INC;
	}else{
	   if(error == 0)printf("eselm Skipped Node Count %ld\n",kn);
	   error = 1;
	}


	return 0;



}
int esnodes(double *x,double *y,double *z,long long *nodeCounti,long long *ndimi)
{
	long ndim;
	
	if(!x || !y || !z || !nodeCounti || !ndimi)return 1;
	
	nodeCount=*nodeCounti;
	
	
	ndim=*ndimi;

//	fprintf(stderr,"esnodes nodeCount %ld\n",nodeCount);
	
	
	if(!nodesx){
		nodesx=(double *)calloc(nodeCount*sizeof(double),1);
		nodesy=(double *)calloc(nodeCount*sizeof(double),1);
		nodesz=(double *)calloc(nodeCount*sizeof(double),1);
		if(!nodesx || !nodesy || !nodesz)return 1;
	}


	for(long k=0;k<nodeCount;++k){
		if(ndim == 1){
			nodesx[k]=x[k];
		}else if(ndim == 2){
			nodesx[k]=x[k];
			nodesy[k]=y[k];
		}else{
			nodesx[k]=x[k];
			nodesy[k]=y[k];
			nodesz[k]=z[k];
		}
	}
		
	return 0;
}

int eshead(char *namei,long long *value)
{
	int nn=0;
	int nend=(int)*value;
	for(int n=0;n<nend;++n){
	    if(namei[n] == ' ')continue;
	    name[nn++]=namei[n];
	}
	name[nn++]=0;
	strncat(name,".shw",255-nn);
	return esbgn();
}
int esbgn()
{
	fprintf(stderr,"esbgn \n");
    if(!show){
		elem4=(long *)malloc(EMAX4*sizeof(long));
		if(!elem4){
		   printf("esbgn No Element Memory\n");
		   return 1;
		}
		elem8=(long *)malloc(EMAX8*sizeof(long));
		if(!elem8){
		   printf("esbgn No Element Memory\n");
		   return 1;
		}

		elem4value=(double *)malloc(6*EMAX4*sizeof(double));
		if(!elem4value){
		   printf("esbgn No Element Memory\n");
		   return 1;
		}

		elem4nodes=(double *)malloc(3*4*EMAX4*sizeof(double));
		if(!elem4nodes){
		   printf("esbgn No Element Memory\n");
		   return 1;
		}

		elem8nodes=(double *)malloc(3*8*EMAX4*sizeof(double));
		if(!elem8nodes){
		   printf("esbgn No Element Memory\n");
		   return 1;
		}

		elem4dis=(double *)malloc(3*4*EMAX4*sizeof(double));
		if(!elem4dis){
		   printf("esbgn No Element Memory\n");
		   return 1;
		}

		elem8dis=(double *)malloc(3*8*EMAX4*sizeof(double));
		if(!elem8dis){
		   printf("esbgn No Element Memory\n");
		   return 1;
		}


		elem8value=(double *)malloc(EMAX8*sizeof(double));
		if(!elem8value){
		   printf("esbgn No Element Memory\n");
		   return 1;
		}
		
		show=showStart(name,"w");
		if(!show)return 1;
    }
    
	nemode=0;
    neselm2=0;
    modeCount=0;
	nelem4=0;
	nelem4value=0;
	nelem4nodes=0;
	nelem8=0;
	nelem8value=0;
	nelem4dis=0;
	nelem8nodes=0;
	nelem8dis=0;
	
	modex=NULL;
	modey=NULL;
	modez=NULL;
	
	nodesx=NULL;
	nodesy=NULL;
	nodesz=NULL;


	return 0;
}
int esstr(double *x,long long *nsi)
{
	double *fout;
	long length,n1,n2,ns;
	int ret;
	
	if(!show)return 1;

	if(!x || !nsi)return 1;

	ns = *nsi;	
	show->stressMax=ns;
	
	for(int k=0;k<ns;++k){
		//printf("%g ",x[k]);
		//printf("\"%ld\" ",ns*nelem4value+k);
		elem4value[ns*nelem4value+k]=x[k];
	}
	++nelem4value;
	//printf(" %ld ns %ld\n",nelem4value,ns);
	

	return 0;
}
int esnod(double *x,long long *n1i,long long *n2i)
{
	double *fout;
	long length,n1,n2;
	int ret;
	
	if(!show)return 1;

	if(!x || !n1i || !n2i)return 1;

	n1 = *n1i;
	
	n2 = *n2i;
	
	
	if(n2 == 4){
		for(int k=0;k<4;++k){
		   // printf("%g %g ",x[2*k],x[2*k+1]);
			if(n1 == 2){
			//  printf("\"%ld %ld %ld\" ",3*nelem4nodes+3*k,3*nelem4nodes+3*k+1,3*nelem4nodes+3*k+2);
			   elem4nodes[3*nelem4nodes+3*k]=x[2*k];
			   elem4nodes[3*nelem4nodes+3*k+1]=x[2*k+1];
			   elem4nodes[3*nelem4nodes+3*k+2]=0;			 
			}else if(n1 == 3){
			   elem4nodes[3*nelem4nodes+3*k]=x[3*k];
			   elem4nodes[3*nelem4nodes+3*k+1]=x[3*k+1];
			   elem4nodes[3*nelem4nodes+3*k+2]=x[3*k+2];
			}else{
			   fprintf(stderr,"esnod error n1 = %ld\n",n1);
			   break;
			}
	    }
		nelem4nodes += 4;
		//printf(" %ld \n",nelem4nodes);
	}else if(n2 == 8){
		for(int k=0;k<8;++k){
		   // printf("%g %g ",x[2*k],x[2*k+1]);
			elem8nodes[3*nelem8nodes+3*k]=x[3*k];
			elem8nodes[3*nelem8nodes+3*k+1]=x[3*k+1];
			elem8nodes[3*nelem8nodes+3*k+2]=x[3*k+2];
	    }
		nelem8nodes += 8;
	
	}else{
		fprintf(stderr,"esnod error n2 = %ld\n",n2);
	}
	

	return 0;
}

int esdis(double *x,long long *n1i,long long *n2i)
{
	double *fout;
	long length,n1,n2;
	int ret;
	
	if(!show)return 1;

	if(!x || !n1i || !n2i)return 1;

	n1 = *n1i;
	
	n2 = *n2i;
	
	
	if(n2 == 4){
		for(int k=0;k<4;++k){
		    //printf("%g %g ",x[2*k],x[2*k+1]);
			if(n1 == 2){
			  // printf("\"%ld %ld %ld\" ",3*nelem4dis+3*k,3*nelem4dis+3*k+1,3*nelem4dis+3*k+2);
			   elem4dis[3*nelem4dis+3*k]=x[2*k];
			   elem4dis[3*nelem4dis+3*k+1]=x[2*k+1];
			   elem4dis[3*nelem4dis+3*k+2]=0;			 
			}else if(n1 == 3){
			   elem4dis[3*nelem4dis+3*k]=x[3*k];
			   elem4dis[3*nelem4dis+3*k+1]=x[3*k+1];
			   elem4dis[3*nelem4dis+3*k+2]=x[3*k+2];
			}else{
			   fprintf(stderr,"esdis error n1 = %ld\n",n1);
			   break;
			}
	    }
		nelem4dis += 4;
		//printf(" %ld \n",nelem4dis);
	}else if(n2 == 8){
		for(int k=0;k<8;++k){
		    //printf("%g %g ",x[2*k],x[2*k+1]);
			   elem8dis[3*nelem8dis+3*k]=x[3*k];
			   elem8dis[3*nelem8dis+3*k+1]=x[3*k+1];
			   elem8dis[3*nelem8dis+3*k+2]=x[3*k+2];
	    }
		nelem8dis += 8;
	}else{
		fprintf(stderr,"esdis error n2 = %ld\n",n2);
	}

	return 0;
}

int eselm(long long *mat,long long *kni)
{
	static int error;
	long kn,n;

	if(!show)return 1;

	if(!mat || !kni)return 1;

	kn = *kni;

	if(kn == 4){
	    if((nelem4+5) >= EMAX4-1)return 0;
	        	elem4[nelem4++]= *mat;
                elem4[nelem4++]= nelem4nodes-4;
                elem4[nelem4++]= nelem4nodes-3;
                elem4[nelem4++]= nelem4nodes-2;
                elem4[nelem4++]= nelem4nodes-1;
                //printf("%ld %ld %ld %ld\n",elem4[nelem4-4],elem4[nelem4-3],elem4[nelem4-2],elem4[nelem4-1]);
	}else if(kn == 8){
	    if((nelem8+9) >= EMAX8-1)return 0;
	        	elem8[nelem8++]= *mat;
	
                elem8[nelem8++]= nelem8nodes-8;
                elem8[nelem8++]= nelem8nodes-7;
                elem8[nelem8++]= nelem8nodes-6;
                elem8[nelem8++]= nelem8nodes-5;
                elem8[nelem8++]= nelem8nodes-4;
                elem8[nelem8++]= nelem8nodes-3;
                elem8[nelem8++]= nelem8nodes-2;
                elem8[nelem8++]= nelem8nodes-1;
	}else{
	   if(error == 0)printf("eselm Skipped Node Count %ld\n",kn);
	   error = 1;
	}

	return 0;
}

int esend()
{
	long n;

	fprintf(stderr,"esend show %p\n",show);
	
	if(!show)return 1;

	fprintf(stderr,"nemode %ld neselm2 %ld nodeCount %ld modeCount %ld\n",(long)nemode,(long)neselm2,nodeCount,modeCount);
	
	fprintf(stderr,"nelem8 %ld nelem8value %ld nelem4 %ld nelem4value %ld nelem4nodes %ld\n",nelem8,nelem8value,nelem4,nelem4value,nelem4nodes);

	if(nemode == 1){
	    if(modeCount == 0){
	        nemode=0;
    		goto OutOfHere2;
	    }
    	if(nelem8){
 			showNodes(show,nodeCount,nodesx,nodesy,nodesz);
   			showElements(show,nelem8/9,8,elem8);
	    	showDisplacements(show,modeCount,modex,modey,modez);
    	}else if(nelem4){
			showNodes(show,nodeCount,nodesx,nodesy,nodesz);
	    	showElements(show,nelem4/5,4,elem4);
	    	showDisplacements(show,modeCount,modex,modey,modez);
    	}
    	modeCount=0;
    	goto OutOfHere;
	}

    if(neselm2 == 1){
    	if(nelem8){
 			showNodes(show,nodeCount,nodesx,nodesy,nodesz);
   			showElements(show,nelem8/9,8,elem8);
   			
 			double xmin,xmax,ymin,ymax,zmin,zmax,x,y,z;
			xmin= 1e30;
			xmax=-1e30;
			ymin= 1e30;
			ymax=-1e30;
			zmin= 1e30;
			zmax=-1e30;
			for(int n=0;n<nodeCount;++n){
				x=nodesx[n];
				y=nodesy[n];
				z=nodesz[n];
				if(x > xmax)xmax=x;
				if(x < xmin)xmin=x;
				if(y > ymax)ymax=y;
				if(y < ymin)ymin=y;
				if(z > zmax)zmax=z;
				if(z < zmin)zmin=z;
				
				if(x < -4){
				    fprintf(stderr,"%d %g %g %g\n",n,x,y,z);
				}
			}
  			
  		   fprintf(stderr,"(neselm2 == 1) xmin %f xmax %f ymin %f ymax %f zmin %f zmax %f\n",xmin,xmax,ymin,ymax,zmin,zmax);
 			
    	}else if(nelem4){
			showNodes(show,nodeCount,nodesx,nodesy,nodesz);
	    	showElements(show,nelem4/5,4,elem4);
    	}
    	goto OutOfHere;
    }

	if(nelem8){
	    long n3,length,nx;
	    double *fout;
		nx=nelem8nodes;
		n3=3*(nx);
		length = (long)(n3*sizeof(double));
		if(!(fout=(double *)malloc(length))){
	    	printf("esnod Out of Memory Request (%ld)\n",length);
	    	return 1;
		}
		double xmin,xmax,ymin,ymax,zmin,zmax,x,y,z;
		xmin= 1e30;
		xmax=-1e30;
		ymin= 1e30;
		ymax=-1e30;
		zmin= 1e30;
		zmax=-1e30;
		for(n=0;n < nelem8nodes;++n){
	    	x=fout[n]=elem8nodes[3*n];
	    	y=fout[n+nx]=elem8nodes[3*n+1];
	    	z=fout[n+2*nx]=elem8nodes[3*n+2];
	    	if(x > xmax)xmax=x;
	    	if(x < xmin)xmin=x;
	    	if(y > ymax)ymax=y;
	    	if(y < ymin)ymin=y;
	    	if(z > zmax)zmax=z;
	    	if(z < zmin)zmin=z;
	    	//printf("%ld %f %f %f\n",n,x,y,z);
		}
		fprintf(stderr,"Nodes8 xmin %f xmax %f ymin %f ymax %f zmin %f zmax %f\n",xmin,xmax,ymin,ymax,zmin,zmax);
	    showNodes(show,nelem8nodes,fout,fout+nx,fout+2*nx);
	    showElements(show,nelem8/9,8,elem8);
	    if(nelem4value > 0){
	        fprintf(stderr,"Stresses %ld elements %ld\n",nelem4value,nelem8/9);
		    showStress(show,nelem4value,elem4value);
		}
	}else if(nelem4){
	    long n3,length,nx;
	    double *fout;
		nx=nelem4nodes;
		n3=3*(nx);
		length = (long)(n3*sizeof(double));
		if(!(fout=(double *)malloc(length))){
	    	printf("esnod Out of Memory Request (%ld)\n",length);
	    	return 1;
		}
		double xmin,xmax,ymin,ymax,zmin,zmax,x,y,z;
		xmin= 1e30;
		xmax=-1e30;
		ymin= 1e30;
		ymax=-1e30;
		zmin= 1e30;
		zmax=-1e30;
		for(n=0;n < nelem4nodes;++n){
	    	x=fout[n]=elem4nodes[3*n];
	    	y=fout[n+nx]=elem4nodes[3*n+1];
	    	z=fout[n+2*nx]=elem4nodes[3*n+2];
	    	if(x > xmax)xmax=x;
	    	if(x < xmin)xmin=x;
	    	if(y > ymax)ymax=y;
	    	if(y < ymin)ymin=y;
	    	if(z > zmax)zmax=z;
	    	if(z < zmin)zmin=z;
	    	//printf("%ld %f %f %f\n",n,x,y,z);
		}
		fprintf(stderr,"Nodes xmin %f xmax %f ymin %f ymax %f zmin %f zmax %f\n",xmin,xmax,ymin,ymax,zmin,zmax);
	    showNodes(show,nelem4nodes,fout,fout+nx,fout+2*nx);
	    showElements(show,nelem4/5,4,elem4);
	    if(nelem4value > 0){
	        fprintf(stderr,"Stresses %ld elements %ld\n",nelem4value,nelem4/5);
		    showStress(show,nelem4value,elem4value);
		}
		if(fout)free(fout);

	}


	if(nelem4dis){
	    long n3,length,nx;
	    double *fout;
		nx=nelem4dis;
		n3=3*(nx);
		length = (long)(n3*sizeof(double));
		if(!(fout=(double *)malloc(length))){
	    	printf("esnod Out of Memory Request (%ld)\n",length);
	    	return 1;
		}
		double xmin,xmax,ymin,ymax,zmin,zmax,x,y,z;
		xmin= 1e30;
		xmax=-1e30;
		ymin= 1e30;
		ymax=-1e30;
		zmin= 1e30;
		zmax=-1e30;
		for(n=0;n < nelem4dis;++n){
	    	x=fout[n]=elem4dis[3*n];
	    	y=fout[n+nx]=elem4dis[3*n+1];
	    	z=fout[n+2*nx]=elem4dis[3*n+2];
	    	if(x > xmax)xmax=x;
	    	if(x < xmin)xmin=x;
	    	if(y > ymax)ymax=y;
	    	if(y < ymin)ymin=y;
	    	if(z > zmax)zmax=z;
	    	if(z < zmin)zmin=z;
	    	//printf("%ld %f %f %f\n",n,x,y,z);
		}
		fprintf(stderr,"Disp nelem4dis %ld xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g\n",nelem4dis,xmin,xmax,ymin,ymax,zmin,zmax);
	    showDisplacements(show,nelem4dis,fout,fout+nx,fout+2*nx);
		if(fout)free(fout);
	}
	if(nelem8dis){
	    long n3,length,nx;
	    double *fout;
		nx=nelem8dis;
		n3=3*(nx);
		length = (long)(n3*sizeof(double));
		if(!(fout=(double *)malloc(length))){
	    	printf("esnod Out of Memory Request (%ld)\n",length);
	    	return 1;
		}
		double xmin,xmax,ymin,ymax,zmin,zmax,x,y,z;
		xmin= 1e30;
		xmax=-1e30;
		ymin= 1e30;
		ymax=-1e30;
		zmin= 1e30;
		zmax=-1e30;
		for(n=0;n < nelem8dis;++n){
	    	x=fout[n]=elem8dis[3*n];
	    	y=fout[n+nx]=elem8dis[3*n+1];
	    	z=fout[n+2*nx]=elem8dis[3*n+2];
	    	if(x > xmax)xmax=x;
	    	if(x < xmin)xmin=x;
	    	if(y > ymax)ymax=y;
	    	if(y < ymin)ymin=y;
	    	if(z > zmax)zmax=z;
	    	if(z < zmin)zmin=z;
	    	//printf("%ld %f %f %f\n",n,x,y,z);
		}
		fprintf(stderr,"Disp nelem8dis %ld xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g\n",nelem8dis,xmin,xmax,ymin,ymax,zmin,zmax);
	    showDisplacements(show,nelem8dis,fout,fout+nx,fout+2*nx);
		if(fout)free(fout);
	}
	
	
OutOfHere:

	showEnd(show);
	
	if(nemode == 1)return 0;
	
	
OutOfHere2:
	
	showDone(show);
	
	show=NULL;
	
	neselm2 = 0;

	if(elem4)free(elem4);
	if(elem8)free(elem8);
	if(elem4value)free(elem4value);
	if(elem8value)free(elem8value);

	if(elem4nodes)free(elem4nodes);
	if(elem4dis)free(elem4dis);
	if(elem8nodes)free(elem8nodes);
	if(elem8dis)free(elem8dis);

	elem4=NULL;
	elem8=NULL;
	elem4value=NULL;
	elem8value=NULL;
	
	elem4nodes=NULL;
	elem4dis=NULL;
	elem8nodes=NULL;
	elem8dis=NULL;

	nelem4=0;
	nelem8=0;
	nelem4value=0;
	nelem8value=0;
	
	nelem4nodes=0;
	nelem4dis=0;
	nelem8nodes=0;
	nelem8dis=0;
	
	
	
	return 0;
}
