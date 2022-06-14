#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <df.h>

/*
cc -I/usr/local/hdf/include -o writesdsforshow writesdsforshow.c -Wall -L/usr/local/hdf/lib -ldf -ljpeg -lz
*/


#ifndef FALSE
#define FALSE 0
#endif

char WarningBuff[256];

#define min(a,b)         ((a)<(b)?(a):(b))

#define max(a,b)         ((a)>(b)?(a):(b))

#define uint16 unsigned int
#define DFTAG_SDG   ((uint16)700) /* Scientific Data Group*/

FILE *nout,*in;

int DFerror;

struct SDS2Dout{
    char *path;
    char *name;
    char *pioName;
    long ixmax;
    long iymax;
    float *data;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double zmin;
    double zmax;
    double vmin;
    double vmax;
    double time;
    int n;
};
 
char *cMalloc(unsigned long length,int tag);
int zerol(char *s,unsigned long n);
int Warning(char *mess);
int cFree(char *ptr);
int writeData(char *nameFile);
int writesds2d(struct SDS2Dout *sdsout);

int main()
{
	return writeData("testwave");
}
int writeData(char *nameFile)
{
 	struct SDS2Dout sdsout;
    double xmin,xmax,ymin,ymax;
	char buff[256];
	char name[256];
	double dt,time;
	float *data;
	float value;
	float xc,yc,r,x,y;
    long ixmax;
    long iymax;
    int ret;
    int n,i,j;
 	
 	
 	zerol((char *)&sdsout,sizeof(struct SDS2Dout));
 	
 	
 	ret=1;
 	
 	data=NULL;
 	
 	r=20;
 	
 	ixmax=720;
 	iymax=480;
 	
 	xmin=0;
 	xmax=144;
 	
 	ymin=0;
 	ymax=96;
 	
 	dt=1e-6;
 	
 	time=0;
 	
	if(nameFile){
		strncpy(buff,nameFile,256);
		strncat(buff,".s2d",256);
		sdsout.path=buff;
	}else{
		sdsout.path="testfile.s2d";
	}
	
	data=(float *)cMalloc(ixmax*iymax*sizeof(float),1000);
	if(!data){
	    sprintf(WarningBuff,"writeData error Trying To allocate %ld Bytes\n",ixmax*iymax*sizeof(float));
	    Warning(WarningBuff);
		goto ErrorOut;
	}
	
	
	yc=0;
	xc=0;
	
	for(n=0;n<100;++n){
		xmin=ymin=  1e30;
		xmax=ymax= -1e30;
	    for(j=0;j<iymax;++j){
	        y=(float)j;
	        for(i=0;i<ixmax;++i){
	            x=(float)(i);
	            value=(float)sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc));
	            if(value < r)value = 1000;
	            
	            data[i+ixmax*(iymax-j-1)]=value;
	            
	            if(x < xmin)xmin=x;
	            if(x > xmax)xmax=x;
	        	if(y < ymin)ymin=y;
	        	if(y > ymax)ymax=y;
	        }
	    }
	    
	    yc += iymax/100.;
		xc += ixmax/100.;
		
	    sprintf(name,"dump.%03d",n);
	    
		sdsout.data=data;
		sdsout.name=name;
		sdsout.ixmax=ixmax;
		sdsout.iymax=iymax;
		sdsout.xmin=xmin;
		sdsout.xmax=xmax;
		sdsout.ymin=ymin;
		sdsout.ymax=ymax;
		sdsout.zmin=0;
		sdsout.zmax=0;
		sdsout.time=time;
		sdsout.n=n;
		sdsout.pioName="pres";	
		writesds2d(&sdsout);
		
		time += dt;
	
	}
	
	ret=0;
ErrorOut:
	if(data)cFree((char *)data);
	return ret;
}
 
int writesds2d(struct SDS2Dout *sdsout)
{
	extern int DFerror;
	char buff[256];
	int32 rank,size[2];
	int lastref;
	double vmin,vmax;
	double v;
	long n;

	if(!sdsout)return 1;

	rank=2;
	size[0]=(int)sdsout->iymax;
	size[1]=(int)sdsout->ixmax;

	if(sdsout->n == 0){
	    unlink((char *)sdsout->path);
	    DFSDclear();
	    DFSDrestart();
	    if(DFSDputdata((char *)sdsout->path,rank,size,(float *)sdsout->data)){
	        sprintf(WarningBuff,"writesds2d DFSDputdata error %d",DFerror);
	        Warning(WarningBuff);
	        goto OutOfHere;
	    }
	}else{
	    if(DFSDadddata((char *)sdsout->path,rank,size,(float *)sdsout->data)){
	        sprintf(WarningBuff,"writesds2d DFSDadddata error %d",DFerror);
	        Warning(WarningBuff);
	        goto OutOfHere;
	    }
	}
	
	vmin =  1e33;
	vmax = -1e33;
	for(n=0;n<sdsout->ixmax*sdsout->iymax;++n){
	    v=sdsout->data[n];
	    if(v < vmin)vmin = v;
	    if(v > vmax)vmax = v;
	}

	lastref=DFSDlastref();
	if(lastref == -1){
	    sprintf(WarningBuff,"writesds2d DFSDlastref error %d",DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}

	if(DFANputlabel(sdsout->path,DFTAG_SDG,lastref,sdsout->name) == -1){
	    sprintf(WarningBuff,"writesds2d DFANputlabel %s Name %s lastref %d error %d",
		               sdsout->path,sdsout->name,lastref,DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}
	if(sdsout->pioName){
	    sprintf(buff,"xmin %g ymin %g zmin %g xmax %g ymax %g zmax %g vmin %g vmax %g time %g pioName \"%s\" ",
	            sdsout->xmin,sdsout->ymin,sdsout->zmin,
                sdsout->xmax,sdsout->ymax,sdsout->zmax,
                vmin,vmax,sdsout->time,sdsout->pioName);
	}else{
	    sprintf(buff,"xmin %g ymin %g zmin %g xmax %g ymax %g zmax %g vmin %g vmax %g time %g",
	            sdsout->xmin,sdsout->ymin,sdsout->zmin,
                sdsout->xmax,sdsout->ymax,sdsout->zmax,
                vmin,vmax,sdsout->time);
	}
	if(DFANputdesc(sdsout->path,DFTAG_SDG,lastref,(char *)buff,strlen((char *)buff)) == -1){
	    sprintf(WarningBuff,"DFANputdesc %s Name %s lastref %d DFerror %d",
		sdsout->path,sdsout->name,lastref,DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}
	return 0;
OutOfHere:
	return 1;
}
char *cMalloc(unsigned long length,int tag)
{
/*      extern char *malloc(long); */
        char *ret;

        tag=tag;

        ret=(char *)malloc(length+4L);
        if(ret == NULL){
            fprintf(stderr,"cMalloc Out of Memory Requested (%ld)",length);
            return (char *)NULL;
        }
        return ret;
}
int cFree(char *ptr)
{
        if(ptr){
            free(ptr);
            return 0;
        }
        return 1;
}
int zerol(char *s,unsigned long n)
{
        unsigned long count;

        if(s == NULL)return 1;
        count=0;
        while(count++ < n)*s++ = 0;
        return 0;
}
int Warning(char *mess)
{
    if(!mess)return 1;
    fprintf(stderr,"%s\n",mess);
    return 0;
}

