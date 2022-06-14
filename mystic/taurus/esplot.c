#include <stdio.h>
#include <stdlib.h>
#include "showlib.h"

#ifdef CRAY
#define esbgn ESBGN
#define esend ESEND
#define eselm ESELM
#define esdis ESDIS
#define esnod ESNOD
#define eshead ESHEAD
#define INC	1
#else
#define esbgn esbgn_
#define esend esend_
#define eselm eselm_
#define esdis esdis_
#define esnod esnod_
#define eshead eshead_
#define INC	1
#endif

static char name=[256];
static long plot=0;
static FILE *out;

int eshead(char *name,long long *value);

#define EMAX4 500000
#define EMAX8 900000

static long *elem4;
static double *elem4value;
static long nelem4;
static long nelem4value;
static long *elem8;
static double *elem8value;
static long nelem8;
static long nelem8value;

static showPtr show;

int eshead(char *namei,long long *value)
{
	int nn=0;
	int nend=(int)*value;
	for(int n=0;n<nend;++n){
	    name[nn++]=namei[n];
	}
	name[nn++]=0;
	esbgn();
}
int esbgn()
{

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

		elem4value=(double *)malloc(EMAX4*sizeof(double));
		if(!elem4value){
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
	nelem4=0;
	nelem4value=0;
	nelem8=0;
	nelem8value=0;

	return 0;
}

int esdis(float *x,long *nxx)
{
	double *fout;
	long length,n,n3,nx;
	int ret;

	if(!show)return 1;

	if(!x || !nxx)return 1;

	nx = *nxx;
	n3=3*(nx);
	length = (long)(n3*sizeof(double));
	if(!(fout=(double *)malloc(length))){
	    printf("node3dc Out of Memory Request (%ld)\n",length);
	    return 1;
	}
	for(n=0;n < nx;++n){
	    fout[n]=x[3*n];
	    fout[n+nx]=x[3*n+1];
	    fout[n+2*nx]=x[3*n+2];
	}

	ret=showDisplacements(show,nx,fout,fout+nx,fout+2*nx);


	if(fout)free(fout);

	return ret;
}
int esnod(float *x,long *nxx)
{
	double *fout;
	long length,n,n3,nx;
	int ret;

	if(!show)return 1;

	if(!x || !nxx)return 1;

	nx = *nxx;
	n3=3*(nx);
	length = (long)(n3*sizeof(double));
	if(!(fout=(double *)malloc(length))){
	    printf("esnod Out of Memory Request (%ld)\n",length);
	    return 1;
	}
	for(n=0;n < nx;++n){
	    fout[n]=x[3*n];
	    fout[n+nx]=x[3*n+1];
	    fout[n+2*nx]=x[3*n+2];
	}

	ret=showNodes(show,nx,fout,fout+nx,fout+2*nx);

	if(fout)free(fout);

	return ret;
}
int eselm(float *value,long *mat,long *ip,long *ki)
{
	static int error;
	long kn,n;

	if(!show)return 1;

	if(!value || !mat || !ip || !ki)return 1;

	kn = *ki;

	if(kn == 3)kn=4;

	if(kn == 4){
	    if((nelem4+5) >= EMAX4-1)return 0;
	        if(ip[0] < 0 || ip[1*INC] < 0 || ip[2*INC] < 0 || ip[3*INC] < 0 ){
		    	ip += 4*INC;
		    	return 0;
	        }
	        	elem4[nelem4++]= *mat;
                elem4[nelem4++]= (*ip)-1;
	        	ip += INC;
                elem4[nelem4++]= (*ip)-1;
	        	ip += INC;
                elem4[nelem4++]= (*ip)-1;
 	        	ip += INC;
                elem4[nelem4++]= (*ip)-1;
	        	ip += INC;
	    		elem4value[nelem4value++]= *value;
	}else if(kn == 8){
	    if((nelem8+9) >= EMAX8-1)return 0;
	        if(ip[0] < 0 || ip[1*INC] < 0 || ip[2*INC] < 0 || ip[3*INC] < 0 ||
	           ip[4*INC] < 0 || ip[5*INC] < 0 || ip[6*INC] < 0 || ip[7*INC] < 0 ){
		    	ip += 8*INC;
		    	return 0;
	        }
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
	    		elem8value[nelem8value++]= *value;
	}else{
	   if(error == 0)printf("eselm Skipped Node Count %ld\n",kn);
	   error = 1;
	}


	return 0;



}
int esend()
{
	long n;

	if(!show)return 1;

	printf("nelem8 %ld nelem8value %ld nelem4 %ld nelem4value %ld\n",nelem8,nelem8value,nelem4,nelem4value);

	if(nelem8){
	    if(nelem4){
	        for(n=0;n<nelem4value;++n){
	            elem8[nelem8++]= elem4[5*n];
	            elem8[nelem8++]= elem4[5*n+1];
	            elem8[nelem8++]= elem4[5*n+2];
	            elem8[nelem8++]= elem4[5*n+3];
	            elem8[nelem8++]= elem4[5*n+4];
	            elem8[nelem8++]= -1;
	            elem8[nelem8++]= -1;
	            elem8[nelem8++]= -1;
	            elem8[nelem8++]= -1;
	            elem8value[nelem8value++]= elem4value[n];
	        }
	    }
	    showElements(show,nelem8value,8,elem8);
	    showStress(show,nelem8value,elem8value);
	}else if(nelem4){
	    showElements(show,nelem4value,4,elem4);
		showStress(show,nelem4value,elem4value);
	}

	showEnd(show);
/*
	if(elem4)free(elem4);
	if(elem8)free(elem8);
	if(elem4value)free(elem4value);
	if(elem8value)free(elem8value);

	elem4=NULL;
	elem8=NULL;
	elem4value=NULL;
	elem8value=NULL;
*/
	nelem4=0;
	nelem8=0;
	nelem4value=0;
	nelem8value=0;
	return 0;
}
