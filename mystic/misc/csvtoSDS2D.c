#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>


/*
gcc -o csvtoSDS2D csvtoSDS2D.c -L/usr/local/hdf/lib -ldf -ljpeg -lz
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
    double dt;
    int n;
};
 

char fin[256],*fp;

int getFloatList(FILE *in,double *FloatData,unsigned long count);
int getLongList(FILE *in,long *LongData,unsigned long count);
int getString(unsigned char *data,long length,FILE *out);
int cFree(char *ptr);
char *cRealloc(char *p,unsigned long r,int tag);
char *cMalloc(unsigned long length,int tag);
int getLong(long *n,FILE *in);
int zerol(char *s,unsigned long n);
int LoadBlock(char *name,char *nameout);
int writesds2d(struct SDS2Dout *sdsout);
int Warning(char *mess);
int WriteBlock(char *name,float *data,long xsize,long ysize);

double dt,scale;

char *nameFile;

int main(int argc,char *argv[])
{
	unsigned long start,total;
	time_t ship;
	int n;

	
		scale=1.0;
		
        n=1;
        start=time(&ship);
        for(n=1;n<argc;++n){
            if(!strcmp(argv[n],"-dt")){
                dt=atof(argv[++n]);
                continue;
            }else if(!strcmp(argv[n],"-name")){
                nameFile=argv[++n];
                continue;
            }else if(!strcmp(argv[n],"-scale")){
               	scale=atof(argv[++n]);
               	printf("scale = %g\n",scale);
               	continue;
            }
	    	strcpy(fin,argv[n]);
	    	fp=strrchr(fin,'.');
	    	if(fp)*fp=0;
	    	strcat(fin,".s2d");
            if(LoadBlock(argv[n],fin))continue;

	}
        total=time(&ship)-start;

        printf("Time To Write Files  %ld Seconds\n",total);
        return 0;
}
int LoadBlock(char *file,char *nameout)
{
	long xsize,ysize,length;
    unsigned char *bp;
    double datain;
	float *data,*dataout;
	FILE *inout;
    char *buffin;
	long blength;
	int n,m,c,i,j;
	int itWas;
	int cs=',';
	
	if(!file || !nameout)return 1;

	if((inout=fopen(file,"rb")) == NULL){
		sprintf(WarningBuff,"Cannot open file : %s to read%c\n",file,0);
		Warning(WarningBuff);
		goto ErrorOut;;
	}
	
	blength=100000;
	buffin=cMalloc(blength,1000);
	if(!buffin)goto ErrorOut;
	
	
	itWas = -7777;
	xsize=0;
	ysize=0;
	
	while((m=(int)fread(buffin,1,blength,inout)) > 0){
		bp=(unsigned char *)buffin;
		for(n=0;n<m;++n){
			c = *bp++;
			if((c == cs) && (ysize == 0))++xsize;
			if(c == '\r')++ysize;
			if((c == '\n') && (itWas != '\r'))++ysize;
			itWas = c;			
		}
	}

	rewind(inout);
	
	++xsize;
	length=xsize*ysize;
	data=(float *)cMalloc(length*sizeof(float),8221);
	dataout=(float *)cMalloc(length*sizeof(float),1000);
	if(!data || !dataout){
		Warning("LoadBlock Out Of Memory\n");
		goto ErrorOut;
	}
	
	for(n=0;n<length;++n){
		fscanf(inout,"%lf,",&datain);
		data[n]=datain;
	}
	
	printf("nameout %s xsize %ld ysize %ld\n",nameout,xsize,ysize);
	
	for(j=0;j<ysize;++j){
	    for(i=0;i<xsize;++i){
	       dataout[j+(xsize-i-1)*ysize]=data[i+j*xsize];
	    }
	}
	
	/* WriteBlock(nameout,data,xsize,ysize); */
	
	WriteBlock(nameout,dataout,ysize,xsize);
	
ErrorOut:

	return 0;
	
}

int WriteBlock(char *name,float *data,long xsize,long ysize)
{
	long ixmin,iymin,ixmax,iymax,length;
	double xmin,xmax,ymin,ymax;
	struct SDS2Dout sdsout;
	static int count;

	if(!name || !data )return 1;


	xmin =      0;
	xmax =  xsize;
	ymin =      0;
	ymax =  ysize;

	ixmin=0;
	iymin=0;

	ixmax=xsize;
	iymax=ysize;

	length=ixmax*iymax;
	
	sdsout.path=name;
	sdsout.name=name;
	
	sdsout.ixmax=ixmax;
	sdsout.iymax=iymax;
	sdsout.data=data;
	sdsout.xmin=xmin;
	sdsout.xmax=xmax;
	sdsout.ymin=ymin;
	sdsout.ymax=ymax;
	sdsout.zmin=0;
	sdsout.zmax=0;
	sdsout.dt=dt;
	sdsout.n=count;
	sdsout.pioName=nameFile;
	
	writesds2d(&sdsout);

	return 0;
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
char *cRealloc(char *p,unsigned long r,int tag)
{
        char *pn;

        tag=tag;

        if(!(pn=(char *)realloc(p,r))){
            return (char *)NULL;
        }

        return pn;
}
int cFree(char *ptr)
{
        if(ptr){
            free(ptr);
            return 0;
        }
        return 1;
}
int getLong(long *n,FILE *in)
{
        unsigned long c1,c2,c3,c4;
        unsigned char c[4];

	if(getString((unsigned char *)c,4L,in))return 1;

	c1=c[0];
	c2=c[1];
	c3=c[2];
	c4=c[3];
        *n = (long)((c1 << 24) | (c2 << 16) | (c3 << 8) | c4);

        return 0;
 
}
int getString(unsigned char *data,long length,FILE *out)
{
        if(!data || !out)return 1;
        if(fread(data,1,length,out) != length){
            return 1;
        }
        return 0;
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
int writesds2d(struct SDS2Dout *sdsout)
{
	extern int DFerror;
	char buff[256];
	int rank,size[2];
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
                vmin,vmax,sdsout->dt*sdsout->n,sdsout->pioName);
	}else{
	    sprintf(buff,"xmin %g ymin %g zmin %g xmax %g ymax %g zmax %g vmin %g vmax %g time %g",
	            sdsout->xmin,sdsout->ymin,sdsout->zmin,
                sdsout->xmax,sdsout->ymax,sdsout->zmax,
                vmin,vmax,sdsout->dt*sdsout->n);
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
