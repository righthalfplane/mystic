#define EXTERN22 extern
#include <string.h>
#include "Xappl.h"
#include "TracerDef.h"
#include "Universal.h"

int getPalette(int n,char *name,char *pal);

int fillct(struct Color *ct,unsigned char *palette);

int getPaletteReal(struct Color *ct,int n);

int findAngles(struct System *Local);
int Transform(struct P *p,struct Matrix *m);

int rotate(struct System *Local);
int SetNormalsBlock(struct Block *block);
int SetBlockRS(struct Block *block);
int getPaletteRGB(int n,unsigned char *r,unsigned char *g,unsigned char *b);

int SetBlockRS(struct Block *block)
{
	float *rdata;
	float *sdata;
 	long NodeCount;

	if(!block)return 1;
	
	if(!block->NodeCount || !block->ElementCount)return 0;
	if(!block->Elements  || !block->xdata ||
	   !block->ydata     || !block->zdata)return 0;
	   
	if((block->NodeCount <= block->RsCount) && block->rdata && block->sdata)return 0;

	rdata = NULL;
	sdata = NULL;
		
	block->RsCount = 0;
	
	NodeCount=block->NodeCount;
	if(block->rdata)cFree((char *)block->rdata);
	block->rdata = NULL;
	if(block->sdata)cFree((char *)block->sdata);
	block->sdata = NULL;
		
	if((rdata=(float *)cMalloc(sizeof(float)*NodeCount,8609)) == NULL){
	     goto OutOfHere;
	}
	zerol((char *)rdata,sizeof(float)*NodeCount);

	if((sdata=(float *)cMalloc(sizeof(float)*NodeCount,8610)) == NULL){
	     goto OutOfHere;
	}
	zerol((char *)sdata,sizeof(float)*NodeCount);

	block->rdata = rdata;
	block->sdata = sdata;

	block->RsCount=NodeCount;
	
	return 0;
OutOfHere:
    sprintf(WarningBuff,"SetBlockRS out of memory\n");
    WarningBatch(WarningBuff);
	if(rdata)cFree((char *)rdata);
	if(sdata)cFree((char *)sdata);
	return 1;
}

int SetNormalsBlock(struct Block *block)
{
	long n1,n2,n3,n4,nn,n;
	long NodesMax;
 	double *nx,*ny,*nz;
 	double len;
	struct P v1,v2,norm;
 	long *ncount;

	if(!block)return 1;
	if(!block->NodeCount || !block->ElementCount)return 0;
	if(!block->Elements  || !block->xdata ||
	   !block->ydata     || !block->zdata)return 0;

	nx = NULL;
	ny = NULL;
	nz = NULL;
	ncount = NULL;

	NodesMax=block->NodesMax;
	if(block->nx)cFree((char *)block->nx);
	block->nx = NULL;
	if(block->ny)cFree((char *)block->ny);
	block->ny = NULL;
	if(block->nz)cFree((char *)block->nz);
	block->nz = NULL;
		
 	if(NodesMax == 3  || NodesMax == 4 ){

		 if((nx=(double *)cMalloc(sizeof(double)*block->NodeCount,8611)) == NULL){
		     goto OutOfHere;
		 }
		 if((ny=(double *)cMalloc(sizeof(double)*block->NodeCount,8612)) == NULL){
		     goto OutOfHere;
		 }
		 if((nz=(double *)cMalloc(sizeof(double)*block->NodeCount,8613)) == NULL){
		     goto OutOfHere;
		 }
		 if((ncount=(long *)cMalloc(sizeof(long)*block->NodeCount,8614))  == NULL){
		     goto OutOfHere;
		 }
		 for(n=0;n<block->NodeCount;++n){
		     nx[n]=0;
		     ny[n]=0;
		     nz[n]=0;
		     ncount[n]=0;
		 }
		for(n=0;n<block->ElementCount;++n){
			nn=(NodesMax+1)*n;
			n1=block->Elements[nn+1];
			n2=block->Elements[nn+2];
			n3=block->Elements[nn+3];

			v1.x=block->xdata[n2]-block->xdata[n1];
			v1.y=block->ydata[n2]-block->ydata[n1];
			v1.z=block->zdata[n2]-block->zdata[n1];
			v2.x=block->xdata[n3]-block->xdata[n1];
			v2.y=block->ydata[n3]-block->ydata[n1];
			v2.z=block->zdata[n3]-block->zdata[n1];
			
		    norm.x =   v1.y*v2.z-v2.y*v1.z;
		    norm.y = -(v1.x*v2.z-v2.x*v1.z);
		    norm.z =   v1.x*v2.y-v2.x*v1.y;
		    
		    len=sqrt(norm.x*norm.x+norm.y*norm.y+norm.z*norm.z);
		    if(!len)continue;
		    
		    norm.x/=len;
		    norm.y/=len;
		    norm.z/=len;
				
			nx[n1] += norm.x;
			ny[n1] += norm.y;
			nz[n1] += norm.z;
			++ncount[n1];
			nx[n2] += norm.x;
			ny[n2] += norm.y;
			nz[n2] += norm.z;
			++ncount[n2];
			nx[n3] += norm.x;
			ny[n3] += norm.y;
			nz[n3] += norm.z;
			++ncount[n3];
			if(NodesMax == 4){
				n4=block->Elements[nn+4];
				nx[n4] += norm.x;
				ny[n4] += norm.y;
				nz[n4] += norm.z;
				++ncount[n4];
			}
		}
		for(n=0;n<block->NodeCount;++n){
			if(ncount[n] > 0){
			 nx[n] /= (double)ncount[n];
			 ny[n] /= (double)ncount[n];
			 nz[n] /= (double)ncount[n];
			}
		}
		if(ncount)cFree((char *)ncount);
		ncount=NULL;
		block->nx = nx;
		nx = NULL;
		block->ny = ny;
		ny = NULL;
		block->nz = nz;
		nz = NULL;
	} 
	return 0;
OutOfHere:
    sprintf(WarningBuff,"SetNormalsBlock out of memory");
    Warning(WarningBuff);
	if(nx)cFree((char *)nx);
	if(ny)cFree((char *)ny);
	if(nz)cFree((char *)nz);
	if(ncount)cFree((char *)ncount);
	return 1;
}

int rotate(struct System *Local)
{
	static struct P v1={1.,0.,0.};
	static struct P v2={0.,1.,0.};
	static struct P v3={0.,0.,1.};
	double xx,xy,xz,yx,yy,yz,zx,zy,zz;
	double c1,c2,c3,s1,s2,s3;
	double tx,ty,tz,dtor;
	double x,y,z;
	double xo[3],yo[3],zo[3],xn[3],yn[3],zn[3];
	double rx,ry,rz;
	long n;

	rx=Local->Theta_x;
	ry=Local->Theta_y;
	rz=Local->Theta_z;

	xo[0]=v1.x;
	yo[0]=v1.y;
	zo[0]=v1.z;

	xo[1]=v2.x;
	yo[1]=v2.y;
	zo[1]=v2.z;

	xo[2]=v3.x;
	yo[2]=v3.y;
	zo[2]=v3.z;

	dtor=atan(1.0)/45.;

	tx=dtor*rx;
	ty=dtor*ry;
	tz=dtor*rz;

	c1=cos(tz);
	s1=sin(tz);
	c2=cos(ty);
	s2=sin(ty);
	c3=cos(tx);
	s3=sin(tx);

	xx=c1*c2;
	xy=s3*c1*s2-s1*c3;
	xz=s1*s3+c1*s2*c3;

	yx=s1*c2;
	yy=c1*c3+s1*s2*s3;
	yz = -s3*c1+s1*s2*c3;

	zx = -s2;
	zy = s3*c2;
	zz = c2*c3;

	for(n=0;n<3;++n){
	    x=xo[n];
	    y=yo[n];
	    z=zo[n];
	    xn[n] = xx*x+xy*y+xz*z;
	    yn[n] = yx*x+yy*y+yz*z;
	    zn[n] = zx*x+zy*y+zz*z;
	}

	Local->x=p(xn[0],yn[0],zn[0]);
	Local->y=p(xn[1],yn[1],zn[1]);
	Local->z=p(xn[2],yn[2],zn[2]);
	return 0;

}
int Transform(struct P *p,struct Matrix *m)
{
	struct P newp;

	if(!p || !m)return 1;
	newp.x=m->x[0][0]*p->x+m->x[0][1]*p->y+m->x[0][2]*p->z+m->x[0][3];
	newp.y=m->x[1][0]*p->x+m->x[1][1]*p->y+m->x[1][2]*p->z+m->x[1][3];
	newp.z=m->x[2][0]*p->x+m->x[2][1]*p->y+m->x[2][2]*p->z+m->x[2][3];
	*p = newp;
	return 0;
}
int findAngles(struct System *Local)
{
	double thx,thy,thz,RadiansToDegrees,xz,thzr;

	if(!Local)return 1;

	RadiansToDegrees = 45./atan(1.0);
	
	
	thx=RadiansToDegrees*atan2(Local->y.z,Local->z.z);
	Local->Theta_x = thx;
	
	thzr=atan2(Local->x.y,Local->x.x);
	thz=RadiansToDegrees*thzr;
	Local->Theta_z = thz;
	
	xz = -Local->x.z;	
	if(xz < -0.999999){
		xz = -.999999;
	}else if(xz > 0.999999){
		xz =  .999999;
	}
	
	thy = RadiansToDegrees*atan2(xz,sqrt(1-xz*xz));
	Local->Theta_y = thy;
	return 0;
}
struct P p(double x,double y,double z)
{
	struct P ret;
	
	ret.x=x;
	ret.y=y;
	ret.z=z;
	
	return ret;
}
struct Color c(double red,double green,double blue)
{
	struct Color ret;
	
	ret.red=red;
	ret.green=green;
	ret.blue=blue;
	
	return ret;
}

int getPaletteReal(struct Color *ct,int n)
{
	char pal[256*3],name[256];
	
	if(!ct)return 1;
	
	getPalette(n,name,pal);
	
	fillct(ct,(unsigned char *)pal);
	
	return 0;
}

int fillct(struct Color *ct,unsigned char *palette)
{	
	double base;
	int k;

	base=1.0/255.;

	for(k=1;k<255;++k){
	    ct[k].red=((double)(palette[k*3]))*base;
	    ct[k].green=((double)(palette[k*3+1]))*base;
	    ct[k].blue=((double)(palette[k*3+2]))*base;
	}
	return 0;
}
int getPaletteRGB(int n,unsigned char *r,unsigned char *g,unsigned char *b)
{
	char pal[256*3];
	int k;
	
	if(!r || !g || !b )return 1;
	
	if(getPalette(n,NULL,pal))return 1;
	
	for(k=0;k<256;++k){
	    r[k]=pal[k*3];
	    g[k]=pal[k*3+1];
	    b[k]=pal[k*3+2];
	}
	
	return 0;	
}
int getPaletteByName(char *palname,unsigned char *pal)
{
	unsigned char *palu;
	int k;
	
	if(!palname || !pal)return 1;
	
    if(!getUniversalPaletteByName(palname,&palu)){
		for(k=0;k<256*3;++k){
            pal[k]=palu[k];
        }
        return 0;
	}
	
	getPalette(12,NULL,(char *)pal);
	
	return 1;
}

