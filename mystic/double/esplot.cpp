#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "showlib.h"
#include <vector>

using namespace std;

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
#define esvel esvel_
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


#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */


int eshead(char *name,long long *value);
int esdis(double *x,long long *nodeCounti);
int esvel(double *x,long long *nodeCounti);
int estime(double *time);
int eselm2(long long *mat,long long *ip,long long *kni);
int esnodes(double *x,double *y,double *z,long long *nodeCounti,long long *ndimi);
int esbgn();
int esstr(double *x,long long *nsi);
int esnod(double *x,long long *n1i,long long *n2i);
int eselm(long long *mat,long long *kni);
int esend();

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

vector<double> xloc;
vector<double> yloc;
vector<double> zloc;

vector<double> xdis;
vector<double> ydis;
vector<double> zdis;

vector<double> xvel;
vector<double> yvel;
vector<double> zvel;


struct triStruct{
    long  mat;
    long  n1;
    long  n2;
    long  n3;
};

vector<triStruct> Tri;

struct quadStruct{
    long  mat;
    long  n1;
    long  n2;
    long  n3;
    long  n4;
};

vector<quadStruct> Quads;

struct hex8Struct{
    long mat;
    long n1;
    long n2;
    long n3;
    long n4;
    long n5;
    long n6;
    long n7;
    long n8;
};

vector<hex8Struct> Hex8;

struct str3truct{
	double s1;
	double s2;
	double s3;
};

vector<str3truct> Str3;

struct str4truct{
	double s1;
	double s2;
	double s3;
	double s4;
};

vector<str4truct> Str4;

struct str6truct{
	double s1;
	double s2;
	double s3;
	double s4;
	double s5;
	double s6;
};

vector<str6truct> Str6;

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
int esdis(double *x,long long *nodeCounti)
{	
	if(!x || !nodeCounti)return 1;
	
	xdis.push_back(x[0]);
	ydis.push_back(x[1]);
	zdis.push_back(x[2]);
	
	return 0;
	
}
int esvel(double *x,long long *nodeCounti)
{	
	if(!x || !nodeCounti)return 1;
	
	xvel.push_back(x[0]);
	yvel.push_back(x[1]);
	zvel.push_back(x[2]);
	
	return 0;
	
}
int eselm2(long long *mat,long long *ip,long long *kni)
{
	struct quadStruct quad;
	struct hex8Struct hex8;
	struct triStruct tri;
	static int error;
	long kn,n;

	if(!show)return 1;

	if(!mat || !ip || !kni)return 1;
	
	neselm2=1;

	kn = *kni;


	if(kn == 3){
		tri.mat = *mat;
		tri.n1= (*ip)-1;
		ip += INC;
		tri.n2= (*ip)-1;
		ip += INC;
		tri.n3= (*ip)-1;
		ip += INC;
		Tri.push_back(tri);
	}else if(kn == 4){
		quad.mat = *mat;
		quad.n1= (*ip)-1;
		ip += INC;
		quad.n2= (*ip)-1;
		ip += INC;
		quad.n3= (*ip)-1;
		ip += INC;
		quad.n4= (*ip)-1;
		ip += INC;
		Quads.push_back(quad);
	}else if(kn == 8){
		hex8.mat = *mat;
		hex8.n1= (*ip)-1;
		ip += INC;
		hex8.n2= (*ip)-1;
		ip += INC;
		hex8.n3= (*ip)-1;
		ip += INC;
		hex8.n4= (*ip)-1;
		ip += INC;
		hex8.n5= (*ip)-1;
		ip += INC;
		hex8.n6= (*ip)-1;
		ip += INC;
		hex8.n7= (*ip)-1;
		ip += INC;
		hex8.n8= (*ip)-1;
		ip += INC;
		Hex8.push_back(hex8);
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
	
	for(long k=0;k<nodeCount;++k){
		if(ndim == 1){
			xloc.push_back(x[k]);
			yloc.push_back(0.0);
			zloc.push_back(0.0);
		}else if(ndim == 2){
			xloc.push_back(x[k]);
			yloc.push_back(y[k]);
			zloc.push_back(0.0);
		}else{
			xloc.push_back(x[k]);
			yloc.push_back(y[k]);
			zloc.push_back(z[k]);
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
		xvel.clear();
		yvel.clear();
		zvel.clear();
		xdis.clear();
		ydis.clear();
		zdis.clear();
	   	Str3.clear();
	   	Str4.clear();
	   	Str6.clear();
		show=showStart(name,(char *)"w");
		if(!show)return 1;
    }
    
	return 0;
}
int esstr(double *x,long long *nsi)
{
	struct str3truct str3;
	struct str4truct str4;
	struct str6truct str6;
	double *fout;
	long length,n1,n2,ns;
	int ret;
	
	if(!show)return 1;

	if(!x || !nsi)return 1;

	ns = *nsi;	
	
//	fprintf(stderr,"esstr %ld\n",ns);
	
	if(ns == 3){
	   str3.s1=x[0];
	   str3.s2=x[1];
	   str3.s3=x[2];
	   Str3.push_back(str3);
	}else if(ns == 4){
	   str4.s1=x[0];
	   str4.s2=x[1];
	   str4.s3=x[2];
	   str4.s4=x[3];
	   Str4.push_back(str4);
	}else if(ns == 6){
	   str6.s1=x[0];
	   str6.s2=x[1];
	   str6.s3=x[2];
	   str6.s4=x[3];
	   str6.s5=x[4];
	   str6.s6=x[5];
	   Str6.push_back(str6);
	}
	return 0;
}
int esend()
{
	long n;

	fprintf(stderr,"esend show %p\n",show);
	
	if(!show)return 1;


	fprintf(stderr,"x %lu y %lu z %lu ",xloc.size(),yloc.size(),zloc.size());
	
	fprintf(stderr,"xvel %lu yvel %lu zvel %lu ",xvel.size(),yvel.size(),zvel.size());

	fprintf(stderr,"Tri %lu Quads %lu Hex8 %lu xdis %lu ",Tri.size(),Quads.size(),Hex8.size(),xdis.size());

	fprintf(stderr,"Str3 %lu Str4 %lu Str6 %lu\n",Str3.size(),Str4.size(),Str6.size());

	if(xloc.size() > 0){
 		showNodes(show,xloc.size(),(double *)&xloc[0],(double *)&yloc[0],(double *)&zloc[0]);
	}else{
	    fprintf(stderr,"esend no nodes\n");
	}
	
	if(Tri.size() > 0){
	    showElements(show,Tri.size(),3,(long *)&Tri[0]);
	}
	
	if(Quads.size() > 0){
	    showElements(show,Quads.size(),4,(long *)&Quads[0]);
	}
	
	if(Hex8.size() > 0){
   		showElements(show,Hex8.size(),8,(long *)&Hex8[0]);
	}
	
	if(xdis.size() > 0){
	    showDisplacements(show,xdis.size(),(double *)&xdis[0],(double *)&ydis[0],(double *)&zdis[0]);
	}	

	if(xvel.size() > 0){
	    showVelocities(show,xvel.size(),(double *)&xvel[0],(double *)&yvel[0],(double *)&zvel[0]);
	}	

	if(Str3.size() > 0){
		show->stressMax=3;
		showStress(show,Str3.size(),(double *)&Str3[0]);
	}

	if(Str4.size() > 0){
		show->stressMax=4;
		showStress(show,Str4.size(),(double *)&Str4[0]);
	}

	if(Str6.size() > 0){
		show->stressMax=6;
		showStress(show,Str6.size(),(double *)&Str6[0]);
	}


OutOfHere:

	showEnd(show);
	
	if(nemode == 1)return 0;
	
	
OutOfHere2:
	
	showDone(show);
	
	show=NULL;
	
	return 0;
}
