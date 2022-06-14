#include "showlib.h"
#include <stdlib.h>

char *cMalloc(unsigned long size);

int cFree(char *pointer);

int zerol(char *pointer,unsigned long length);

int Warning(char *message);

int showWrite(showPtr s,void *d,unsigned long n);
int showLong(showPtr s,long *nodeCount,unsigned long n);
int showFloat(showPtr s,double *x,unsigned long n);

showPtr showStart(char *name,char *mode)
{
	char buff[256];
	showPtr s;

	if(!name)return NULL;

	s=(showPtr)cMalloc(sizeof(struct showdata));

	if(!s){
	    sprintf(buff,"showStart Out of Memory");
	    Warning(buff);
	    return NULL;
	}
	zerol((char *)s,sizeof(struct showdata));

	{
		union test{
		    unsigned int n;
		    unsigned char cn[4];
		}t;
		t.n=1;
		s->pc=t.cn[0];
	}

	if(*mode == 'w'){
	    s->out=fopen(name,"w");
	    if(!s->out){
	        sprintf(buff,"showStart Could Not Open %s to Write",name);
	        Warning(buff);
	        goto ErrorOut;
	    }
	    if(showWrite(s,"SHOW",4L))goto ErrorOut;
	}else{
	    s->out=fopen(name,"r");
	    if(!s->out){
	        sprintf(buff,"showStart Could Not Open %s to Read",name);
	        Warning(buff);
	        goto ErrorOut;
	    }
	}
	return s;
ErrorOut:
	if(s){
	    if(s->out)fclose(s->out);
	}
	cFree((char *)s);
	return NULL;
}
int showEnd(showPtr s)
{
	if(!s)return 1;
	if(showWrite(s,"BLK1",4L))goto ErrorOut;

	if(showLong(s,&s->nodeCount,1L))goto ErrorOut;
	if(showLong(s,&s->valueCount,1L))goto ErrorOut;
	if(showLong(s,&s->normalCount,1L))goto ErrorOut;
	if(showLong(s,&s->dxyzCount,1L))goto ErrorOut;
	if(showLong(s,&s->rsCount,1L))goto ErrorOut;
	if(showLong(s,&s->elementCount,1L))goto ErrorOut;
	if(showLong(s,&s->nodesMax,1L))goto ErrorOut;
	if(showLong(s,&s->localSystem,1L))goto ErrorOut;
	if(showLong(s,&s->stressCount,1L))goto ErrorOut;

	if(s->nodeCount){
	    if(showFloat(s,s->x,s->nodeCount))goto ErrorOut;
	    if(showFloat(s,s->y,s->nodeCount))goto ErrorOut;
	    if(showFloat(s,s->z,s->nodeCount))goto ErrorOut;
	}

	if(s->valueCount){
	    if(showFloat(s,s->v,s->valueCount))goto ErrorOut;
	}

	if(s->normalCount){
	    if(showFloat(s,s->nx,s->normalCount))goto ErrorOut;
	    if(showFloat(s,s->ny,s->normalCount))goto ErrorOut;
	    if(showFloat(s,s->nz,s->normalCount))goto ErrorOut;
	}

	if(s->dxyzCount){
	    if(showFloat(s,s->dx,s->dxyzCount))goto ErrorOut;
	    if(showFloat(s,s->dy,s->dxyzCount))goto ErrorOut;
	    if(showFloat(s,s->dz,s->dxyzCount))goto ErrorOut;
	}

	if(s->rsCount){
	    if(showFloat(s,s->r,s->rsCount))goto ErrorOut;
	    if(showFloat(s,s->s,s->rsCount))goto ErrorOut;
	}

	if(s->elementCount){
	    if(showLong(s,s->elements,s->elementCount*(s->nodesMax+1)))goto ErrorOut;
	}

	if(s->stressCount){
	    if(showFloat(s,s->stress,s->stressCount))goto ErrorOut;
	}

	if(showWrite(s,"END1",4L))goto ErrorOut;
	return 0;
ErrorOut:
	return 1;
}
int showLong(showPtr s,long *node,unsigned long nn)
{
	unsigned char c[4];
	unsigned char *lp;
	unsigned long n;


	if(!s || !s->out || !node || !nn)return 1;


	if(s->pc){
	    for(n=0;n<nn;++n){
	         lp= (unsigned char *)&node[n];
	         c[0]=lp[3];
	         c[1]=lp[2];
	         c[2]=lp[1];
	         c[3]=lp[0];
	         if(showWrite(s,(unsigned char *)c,4L))return 1;
	    }
	}else{
	     if(showWrite(s,(unsigned char *)node,nn*sizeof(long)))return 1;
	}

	return 0;
}
int showFloat(showPtr s,double *x,unsigned long nn)
{
	unsigned char c[4];
	unsigned char *lp;
	unsigned long n;

	if(!s || !x || !nn)return 1;

	if(s->pc){
	    for(n=0;n<nn;++n){
	         lp= (unsigned char *)&x[n];
	         c[0]=lp[3];
	         c[1]=lp[2];
	         c[2]=lp[1];
	         c[3]=lp[0];
	         if(showWrite(s,(unsigned char *)c,4L))return 1;
	    }
	}else{
	    if(showWrite(s,(unsigned char *)x,nn*sizeof(double)))return 1;
	}

	return 0;
}
int showDone(showPtr s)
{
	if(!s)return 1;

	showWrite(s,"DONE",4L);

	if(s->out)fclose(s->out);
	s->out=NULL;

	s->nodeCount=0;

	s->valueCount=0;

	s->normalCount=0;

	s->dxyzCount=0;

	s->rsCount=0;

	s->localSystem=0;

	s->stressCount=0;

	s->elementCount=0;

	if(s->x)cFree((char *)s->x);
	s->x=NULL;

	if(s->y)cFree((char *)s->y);
	s->y=NULL;

	if(s->z)cFree((char *)s->z);
	s->z=NULL;

	if(s->v)cFree((char *)s->v);
	s->v=NULL;

	if(s->nx)cFree((char *)s->nx);
	s->nx=NULL;

	if(s->ny)cFree((char *)s->ny);
	s->ny=NULL;

	if(s->nz)cFree((char *)s->nz);
	s->nz=NULL;

	if(s->dx)cFree((char *)s->dx);
	s->dx=NULL;

	if(s->dy)cFree((char *)s->dy);
	s->dy=NULL;

	if(s->dz)cFree((char *)s->dz);
	s->dz=NULL;

	if(s->r)cFree((char *)s->r);
	s->r=NULL;

	if(s->s)cFree((char *)s->s);
	s->s=NULL;

	if(s->elements)cFree((char *)s->elements);
	s->elements=NULL;

	if(s->stress)cFree((char *)s->stress);
	s->stress=NULL;

	cFree((char *)s);
	return 0;
}
int showWrite(showPtr s,void *data,unsigned long length)
{
	if(!s || !s->out || !data || !length)return 1;

	 if(fwrite(data,1,length,s->out) != length){
		return 1;
	}
	
	return 0;
}
int showNodes(showPtr s,long nodeCount,double *x,double *y,double *z)
{
	double *sx,*sy,*sz;
	char buff[256];
	long n;

	if(!s)return 1;

	if(s->x)cFree((char *)s->x);
	s->x=NULL;

	if(s->y)cFree((char *)s->y);
	s->y=NULL;

	if(s->z)cFree((char *)s->z);
	s->z=NULL;


	s->nodeCount=0;

	if(nodeCount <= 0)nodeCount=0;

	if(nodeCount == 0)return 0;

	if(!x || !y || !z)return 1;

	sx=(double *)cMalloc(nodeCount*sizeof(double));
	sy=(double *)cMalloc(nodeCount*sizeof(double));
	sz=(double *)cMalloc(nodeCount*sizeof(double));

	if(!sx || !sy || !sz){
	    if(sx)cFree((char *)sx);
	    if(sy)cFree((char *)sy);
	    if(sz)cFree((char *)sz);

	    sprintf(buff,"showNodes Out Of Memory (Requested %ld)\n",
	           nodeCount*sizeof(double)*3);
	    Warning(buff);
	    return 1;
	}

	for(n=0;n<nodeCount;++n){
	    sx[n]=x[n];
	    sy[n]=y[n];
	    sz[n]=z[n];
	}

	s->x=sx;
	s->y=sy;
	s->z=sz;
	s->nodeCount=nodeCount;

	return 0;
}
int showNormals(showPtr s,long nodeCount,double *nx,double *ny,double *nz)
{
	double *sx,*sy,*sz;
	char buff[256];
	long n;

	if(!s)return 1;

	if(s->nx)cFree((char *)s->nx);
	s->nx=NULL;

	if(s->ny)cFree((char *)s->ny);
	s->ny=NULL;

	if(s->nz)cFree((char *)s->nz);
	s->nz=NULL;

	s->normalCount=0;

	if(nodeCount <= 0)nodeCount=0;

	if(nodeCount == 0)return 0;

	if(!nx || !ny || !nz)return 1;

	sx=(double *)cMalloc(nodeCount*sizeof(double));
	sy=(double *)cMalloc(nodeCount*sizeof(double));
	sz=(double *)cMalloc(nodeCount*sizeof(double));

	if(!sx || !sy || !sz){
	    if(sx)cFree((char *)sx);
	    if(sy)cFree((char *)sy);
	    if(sz)cFree((char *)sz);

	    sprintf(buff,"showNormals Out Of Memory (Requested %ld)",
	           nodeCount*sizeof(double)*3);
	    Warning(buff);
	    return 1;
	}

	for(n=0;n<nodeCount;++n){
	    sx[n]=nx[n];
	    sy[n]=ny[n];
	    sz[n]=nz[n];
	}

	s->nx=sx;
	s->ny=sy;
	s->nz=sz;
	s->normalCount=nodeCount;

	return 0;
}
int showDisplacements(showPtr s,long nodeCount,double *dx,double *dy,double *dz)
{
	double *sx,*sy,*sz;
	char buff[256];
	long n;

	if(!s)return 1;

	if(s->dx)cFree((char *)s->dx);
	s->dx=NULL;

	if(s->dy)cFree((char *)s->dy);
	s->dy=NULL;

	if(s->dz)cFree((char *)s->dz);
	s->dz=NULL;

	s->dxyzCount=0;

	if(nodeCount <= 0)nodeCount=0;

	if(nodeCount == 0)return 0;

	if(!dx || !dy || !dz)return 1;

	sx=(double *)cMalloc(nodeCount*sizeof(double));
	sy=(double *)cMalloc(nodeCount*sizeof(double));
	sz=(double *)cMalloc(nodeCount*sizeof(double));

	if(!sx || !sy || !sz){
	    if(sx)cFree((char *)sx);
	    if(sy)cFree((char *)sy);
	    if(sz)cFree((char *)sz);

	    sprintf(buff,"showDisplacements Out Of Memory (Requested %ld)",
	           nodeCount*sizeof(double)*3);
	    Warning(buff);
	    return 1;
	}

	for(n=0;n<nodeCount;++n){
	    sx[n]=dx[n];
	    sy[n]=dy[n];
	    sz[n]=dz[n];
	}

	s->dx=sx;
	s->dy=sy;
	s->dz=sz;
	s->dxyzCount=nodeCount;

	return 0;
}
int showRS(showPtr s,long nodeCount,double *rr,double *ss)
{
	double *sx,*sy;
	char buff[256];
	long n;

	if(!s)return 1;

	if(s->r)cFree((char *)s->r);
	s->r=NULL;

	if(s->s)cFree((char *)s->s);
	s->s=NULL;


	s->rsCount=0;

	if(nodeCount <= 0)nodeCount=0;

	if(nodeCount == 0)return 0;

	if(!rr || !ss)return 1;

	sx=(double *)cMalloc(nodeCount*sizeof(double));
	sy=(double *)cMalloc(nodeCount*sizeof(double));

	if(!sx || !sy){
	    if(sx)cFree((char *)sx);
	    if(sy)cFree((char *)sy);

	    sprintf(buff,"showRS Out Of Memory (Requested %ld)",
	           nodeCount*sizeof(double)*3);
	    Warning(buff);
	    return 1;
	}

	for(n=0;n<nodeCount;++n){
	    sx[n]=rr[n];
	    sy[n]=ss[n];
	}

	s->r=sx;
	s->s=sy;
	s->rsCount=nodeCount;

	return 0;
}
int showElements(showPtr s,long elementCount,long nodesMax,long *elements)
{
	char buff[256];
	long *sn;
	long n;

	if(!s)return 1;

	if(s->elements)cFree((char *)s->elements);
	s->elements=NULL;

	s->elementCount=0;

	if(elementCount <= 0)elementCount=0;

	if(elementCount == 0)return 0;

	if(!elements)return 1;

	sn=(long *)cMalloc(elementCount*(nodesMax+1)*sizeof(long));

	if(!sn){
	    if(sn)cFree((char *)sn);

	    sprintf(buff,"showElements Out Of Memory (Requested %ld)",
	           elementCount*sizeof(long)*(nodesMax+1));
	    Warning(buff);
	    return 1;
	}

	for(n=0;n<elementCount*(nodesMax+1);++n){
	    sn[n]=elements[n];
	}

	s->elements=sn;

	s->elementCount=elementCount;

	s->nodesMax=nodesMax;

	return 0;
}
int showValues(showPtr s,long nodeCount,double *v)
{
	double *sx;
	char buff[256];
	long n;

	if(!s)return 1;

	if(s->v)cFree((char *)s->v);
	s->v=NULL;

	s->valueCount=0;

	if(nodeCount <= 0)nodeCount=0;

	if(nodeCount == 0)return 0;

	if(!v)return 1;

	sx=(double *)cMalloc(nodeCount*sizeof(double));

	if(!sx){

	    sprintf(buff,"showDisplacements Out Of Memory (Requested %ld)",
	            nodeCount*sizeof(double)*3);
	    Warning(buff);
	    return 1;
	}

	for(n=0;n<nodeCount;++n){
	    sx[n]=v[n];
	}

	s->v=sx;
	s->valueCount=nodeCount;
	return 0;
}
int showStressFloat(showPtr s,long nodeCount,float *stress,long ibq)
{
	double *sx;
	char buff[256];
	long n;

	if(!s)return 1;

	if(s->stress)cFree((char *)s->stress);
	s->stress=NULL;

	s->stressCount=0;

	if(nodeCount <= 0)nodeCount=0;

	if(nodeCount == 0)return 0;

	if(!stress)return 1;

	sx=(double *)cMalloc(nodeCount*sizeof(double));

	if(!sx){

	    sprintf(buff,"showDisplacements Out Of Memory (Requested %ld)",
	            nodeCount*sizeof(double)*3);
	    Warning(buff);
	    return 1;
	}
	
	
    if(ibq == 1){
	    for(n=0;n<nodeCount;++n){
	        sx[n]=stress[n];
	    }
    }else{
	    for(n=0;n<nodeCount;++n){
	        sx[n]=.25*(stress[4*n]+stress[4*n+1]+stress[4*n+2]+stress[4*n+3]);
	    }
    }
	
	s->stress=sx;
	s->stressCount=nodeCount;
	return 0;
}
int showStress(showPtr s,long nodeCount,double *stress)
{
	double *sx;
	char buff[256];
	long n;

	if(!s)return 1;

	if(s->stress)cFree((char *)s->stress);
	s->stress=NULL;

	s->stressCount=0;

	if(nodeCount <= 0)nodeCount=0;

	if(nodeCount == 0)return 0;

	if(!stress)return 1;

	sx=(double *)cMalloc(nodeCount*sizeof(double));

	if(!sx){

	    sprintf(buff,"showDisplacements Out Of Memory (Requested %ld)",
	            nodeCount*sizeof(double)*3);
	    Warning(buff);
	    return 1;
	}

	for(n=0;n<nodeCount;++n){
	    sx[n]=stress[n];
	}

	s->stress=sx;
	s->stressCount=nodeCount;
	return 0;
}
char *cMalloc(unsigned long length)
{
/*      extern char *malloc(long); */
        char buff[256];
        char *ret;

        ret=(char *)malloc(length+4L);
        if(ret == NULL){
            sprintf(buff,"cMalloc Out of Memory Requested (%ld)",length);
            Warning(buff);
            return (char *)NULL;
        }
        return ret;
}
int cFree(char *message)
{
	if(!message)return 1;

	free((char *)message);

	return 0;
}
int Warning(char *message)
{
	if(!message)return 1;

        fprintf(stderr,"%s\n",message);

        return 0;
}
/*
int zerol(char *s,unsigned long n)
{
        unsigned long count;

        if(s == NULL)return 1;
        count=0;
        while(count++ < n)*s++ = 0;
        return 0;
}
*/
