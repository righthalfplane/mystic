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
cc -o cWriteSdsForShow cWriteSdsForShow.c -Wall -ldf -ljpeg -lz
*/


char WarningBuff[256];

int DFerror;

#define DATA_TYPE_DOUBLE 	0
#define DATA_TYPE_FLOAT  	1
#define DATA_TYPE_BYTE   	2
#define DATA_TYPE_DOUBLE_3D 3
#define DATA_TYPE_FLOAT_3D  4

struct SDS2Dout{
    char *path;
    char *name;
    char *pioName;
    long ixmax;
    long iymax;
    long izmax;
    double *data;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double zmin;
    double zmax;
    double vmin;
    double vmax;
    double time;
    int type;
    int n;
};
 
char *cMalloc(unsigned long length,int tag);
int zerol(char *s,unsigned long n);
int Warning(char *mess);
int cFree(char *ptr);
int writeData(char *nameFile,int type);
int writeData3d(char *nameFile,int type);
int writesds(struct SDS2Dout *sdsout);
static int writesds2dFloat(struct SDS2Dout *sdsout);
static int writesds2dBytes(struct SDS2Dout *sdsout);
static int writesds2dDouble(struct SDS2Dout *sdsout);
static int writesds3dFloat(struct SDS2Dout *sdsout);
static int writesds3dDouble(struct SDS2Dout *sdsout);

int main()
{

	writeData("waveFloat",DATA_TYPE_FLOAT);
		
	writeData("waveBytes",DATA_TYPE_BYTE);

	writeData("waveDouble",DATA_TYPE_DOUBLE);

	writeData3d("waveFloat3d",DATA_TYPE_FLOAT_3D);
	
	writeData3d("waveDouble3d",DATA_TYPE_DOUBLE_3D);
		
	return 0;
}
int writeData3d(char *nameFile,int type)
{
 	struct SDS2Dout sdsout;
    double xmin,xmax,ymin,ymax,zmin,zmax;
	char buff[256];
	char name[256];
	char *root;
	double dt,time;
	double *data;
	double value;
	double xc,yc,zc,r,x,y,z;
	double dxdi,dydj,dzdk;
    long ixmax;
    long iymax;
    long izmax;
    int ret;
    int n,i,j,k;
 	
 	
 	zerol((char *)&sdsout,sizeof(struct SDS2Dout));
 	
 	
 	ret=1;
 	
 	data=NULL;
 	
 	r=20;
 	
 	ixmax=150;
 	iymax=200;
 	izmax=250;
 	
 	xmin=0;
 	xmax=400;
 	
 	dxdi=(xmax-xmin)/(double)(ixmax);
 	
 	ymin=300;
 	ymax=600;
 	
 	dydj=(ymax-ymin)/(double)(iymax);
 
 	 	
 	zmin=400;
 	zmax=800;
 	
 	dzdk=(zmax-zmin)/(double)(izmax);
 
 	 	
 	dt=1e-6;
 	
 	time=0;
 	
	if(nameFile){
	    root=nameFile;
	}else{
	    root="testfile3d";
	}
	
	data=(double *)cMalloc(ixmax*iymax*izmax*sizeof(double),1000);
	if(!data){
	    sprintf(WarningBuff,"writeData error Trying To allocate %ld Bytes\n",ixmax*iymax*izmax*sizeof(double));
	    Warning(WarningBuff);
		goto ErrorOut;
	}
	
	
	yc=ymin;
	xc=xmin;
	zc=zmin;
	
	for(n=0;n<50;++n){
		for(k=0;k<izmax;++k){
			z=zmin+0.5*dzdk+(double)k*dzdk;  /* locate z center of pixel */
			for(j=0;j<iymax;++j){
				y=ymin+0.5*dydj+(double)j*dydj;  /* locate y center of pixel */
				for(i=0;i<ixmax;++i){
					x=xmin+0.5*dxdi+(double)i*dxdi;  /* locate x center of pixel */
					value=(double)sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc)+(z-zc)*(z-zc));
					if(value < r)value = 1000;
					
					data[i+ixmax*j+k*ixmax*iymax]=value;					
				}
			}
	  	}
	    
	    yc += (ymax-ymin)/50.;
		xc += (xmax-xmin)/50.;
		zc += (zmax-zmin)/50.;
		
		/*
	    sprintf(name,"dump.%04d%c",n,0);
	    */
	    
	    sprintf(name,"%s.%04d.s3d%c",root,n,0);
	    
	    sprintf(buff,"%s.%04d.s3d%c",root,n,0);
	    
		sdsout.path=buff;
	    
		sdsout.data=data;
		sdsout.name=name;
		sdsout.ixmax=ixmax;
		sdsout.iymax=iymax;
		sdsout.izmax=izmax;
		sdsout.xmin=xmin;
		sdsout.xmax=xmax;
		sdsout.ymin=ymin;
		sdsout.ymax=ymax;
		sdsout.zmin=zmin;
		sdsout.zmax=zmax;
		sdsout.time=time;
		sdsout.n=0;
		sdsout.pioName="pres";	
		sdsout.type=type;	
		if(writesds(&sdsout))goto ErrorOut;
		
		time += dt;
	
	}
	
	ret=0;
ErrorOut:
	if(data)cFree((char *)data);
	return ret;
}
int writeData(char *nameFile,int type)
{
 	struct SDS2Dout sdsout;
    double xmin,xmax,ymin,ymax;
	char buff[256];
	char name[256];
	double dt,time;
	double *data;
	double value;
	double xc,yc,r,x,y;
	double dxdi,dydj;
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
 	xmax=1440;
 	
 	dxdi=(xmax-xmin)/(double)(ixmax);
 	
 	ymin=480;
 	ymax=1440;
 	
 	dydj=(ymax-ymin)/(double)(iymax);
 
 	 	
 	dt=1e-6;
 	
 	time=0;
 	
	if(nameFile){
		strncpy(buff,nameFile,256);
		if(type == DATA_TYPE_BYTE){
			strncat(buff,".hdf",256);
		}else{
			strncat(buff,".s2d",256);
		}
		sdsout.path=buff;
	}else{
		if(type == DATA_TYPE_BYTE){
			sdsout.path="testfile.hdf";
		}else{
			sdsout.path="testfile.s2d";
		}
	}
	
	data=(double *)cMalloc(ixmax*iymax*sizeof(double),1000);
	if(!data){
	    sprintf(WarningBuff,"writeData error Trying To allocate %ld Bytes\n",ixmax*iymax*sizeof(double));
	    Warning(WarningBuff);
		goto ErrorOut;
	}
	
	
	yc=ymin;
	xc=xmin;
	
	for(n=0;n<50;++n){
	    for(j=0;j<iymax;++j){
	        y=ymin+0.5*dydj+(double)j*dydj;  /* locate y center of pixel */
	        for(i=0;i<ixmax;++i){
	            x=xmin+0.5*dxdi+(double)i*dxdi;  /* locate x center of pixel */
	            value=(double)sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc));
	            if(value < r)value = 1000;
	            
	            data[i+ixmax*(iymax-j-1)]=value;
	            
	        }
	    }
	    
	    yc += (ymax-ymin)/100.;
		xc += (xmax-xmin)/100.;
		
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
		sdsout.type=type;	
				
		if(writesds(&sdsout))goto ErrorOut;
		
		time += dt;
	
	}
	
	ret=0;
ErrorOut:
	if(data)cFree((char *)data);
	return ret;
}
 
int writesds(struct SDS2Dout *sdsout)
{

	if(!sdsout){
	    Warning("writesds2d Found NULL struct pointer\n");
	    return 1;
	}
	
	if(sdsout->xmin >= sdsout->xmax){
	    sprintf(WarningBuff,"Warning            : Time %g\n",sdsout->time);
	    Warning(WarningBuff);
	    sprintf(WarningBuff,"Warning Bad Bounds : xmin %g xmax %g\n",sdsout->xmin,sdsout->xmax);
	    Warning(WarningBuff);
	    sdsout->xmax = sdsout->xmin+fabs(sdsout->xmin)+1.0;
	    sprintf(WarningBuff,"Reset To           : xmin %g xmax %g\n\n",sdsout->xmin,sdsout->xmax);
	    Warning(WarningBuff);
	}
	
	if(sdsout->ymin >= sdsout->ymax){
	    sprintf(WarningBuff,"Warning            : Time %g\n",sdsout->time);
	    Warning(WarningBuff);
	    sprintf(WarningBuff,"Warning Bad Bounds : ymin %g ymax %g\n",sdsout->ymin,sdsout->ymax);
	    Warning(WarningBuff);
	    sdsout->ymax = sdsout->ymin+fabs(sdsout->ymin)+1.0;
	    sprintf(WarningBuff,"Reset To           : ymin %g ymax %g\n\n",sdsout->ymin,sdsout->ymax);
	    Warning(WarningBuff);
	}
	
	if(sdsout->type == DATA_TYPE_DOUBLE){
		return writesds2dDouble(sdsout);
	}else if(sdsout->type == DATA_TYPE_FLOAT){
		return writesds2dFloat(sdsout);
	}else if(sdsout->type == DATA_TYPE_BYTE){
		return writesds2dBytes(sdsout);
	}else if(sdsout->type == DATA_TYPE_FLOAT_3D){
		return writesds3dFloat(sdsout);
	}else if(sdsout->type == DATA_TYPE_DOUBLE_3D){
		return writesds3dDouble(sdsout);
	}
	
	sprintf(WarningBuff,"writesds2d Found Unknown data type %d\n",sdsout->type);
	Warning(WarningBuff);

	return 1;
}
static int writesds3dDouble(struct SDS2Dout *sdsout)
{
	extern int DFerror;
	int32 rank,size[3];
	double vmin,vmax;
	char buff[256];
	long n,length;
	int lastref;
	double v;
	int ret;
	
	ret=1;
	
	if(!sdsout){
	    Warning("writesds3dDouble Found NULL struct pointer\n");
		goto OutOfHere;
	}
	
	if(sdsout->type != DATA_TYPE_DOUBLE_3D){
	    sprintf(WarningBuff,"writesds3dDouble Found Wrong data type %d\n",sdsout->type);
	    Warning(WarningBuff);
		goto OutOfHere;
	}
	
	if(!sdsout->data){
	    Warning("writesds3dDouble Found NULL data Pointer\n");
		goto OutOfHere;
	}
	
	length=sdsout->ixmax*sdsout->iymax*sdsout->izmax;
	
	if(length <= 0){
	    Warning("writesds3dDouble Found data length less than one\n");
		goto OutOfHere;
	}
	

	vmin =  1e33;
	vmax = -1e33;
	for(n=0;n<length;++n){
	    v=sdsout->data[n];
	    if(v < vmin)vmin = v;
	    if(v > vmax)vmax = v;
	}
	/*
	fprintf(stderr,"writesds3dDouble dmin %g dmax %g\n",vmin,vmax);
	*/
	rank=3;
	size[0]=(int)sdsout->izmax;
	size[1]=(int)sdsout->iymax;
	size[2]=(int)sdsout->ixmax;

	if(sdsout->n == 0){
	    unlink((char *)sdsout->path);
	    DFSDclear();
	    DFSDrestart();
        DFSDsetNT(DFNT_FLOAT64);
	    if(DFSDputdata((char *)sdsout->path,rank,size,(float *)sdsout->data)){
	        sprintf(WarningBuff,"writesds3dDouble DFSDputdata error %d",DFerror);
	        Warning(WarningBuff);
	        goto OutOfHere;
	    }
	}else{
	    if(DFSDadddata((char *)sdsout->path,rank,size,(float *)sdsout->data)){
	        sprintf(WarningBuff,"writesds3dDouble DFSDadddata error %d",DFerror);
	        Warning(WarningBuff);
	        goto OutOfHere;
	    }
	}
	

	lastref=DFSDlastref();
	if(lastref == -1){
	    sprintf(WarningBuff,"writesds3dDouble DFSDlastref error %d",DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}

	if(DFANputlabel(sdsout->path,DFTAG_SDG,lastref,sdsout->name) == -1){
	    sprintf(WarningBuff,"writesds3dDouble DFANputlabel %s Name %s lastref %d error %d",
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
	    sprintf(WarningBuff,"writesds3dDouble DFANputdesc %s Name %s lastref %d DFerror %d",
		sdsout->path,sdsout->name,lastref,DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}
	ret = 0;
OutOfHere:
	return ret;
}
static int writesds3dFloat(struct SDS2Dout *sdsout)
{
	extern int DFerror;
	int32 rank,size[3];
	double vmin,vmax;
	char buff[256];
	long n,length;
	int lastref;
	float *data;
	double v;
	int ret;
	
	ret=1;
	
	data=NULL;

	if(!sdsout){
	    Warning("writesds3dFloat Found NULL struct pointer\n");
		goto OutOfHere;
	}
	
	if(sdsout->type != DATA_TYPE_FLOAT_3D){
	    sprintf(WarningBuff,"writesds3dFloat Found Wrong data type %d\n",sdsout->type);
	    Warning(WarningBuff);
		goto OutOfHere;
	}
	
	if(!sdsout->data){
	    Warning("writesds3dFloat Found NULL data Pointer\n");
		goto OutOfHere;
	}
	
	length=sdsout->ixmax*sdsout->iymax*sdsout->izmax;
	
	if(length <= 0){
	    Warning("writesds3dFloat Found data length less than one\n");
		goto OutOfHere;
	}
	
	data=(float *)cMalloc(length*sizeof(float),1002);
	if(!data){
	    sprintf(WarningBuff,"writesds3dFloat error Trying To allocate %ld Bytes\n",length*sizeof(float));
	    Warning(WarningBuff);
		goto OutOfHere;
	}


	vmin =  1e33;
	vmax = -1e33;
	for(n=0;n<length;++n){
	    v=sdsout->data[n];
	    data[n]=(float)v;
	    if(v < vmin)vmin = v;
	    if(v > vmax)vmax = v;
	}
/*
	fprintf(stderr,"writesds3dFloat dmin %g dmax %g\n",vmin,vmax);
*/

	rank=3;
	size[0]=(int)sdsout->izmax;
	size[1]=(int)sdsout->iymax;
	size[2]=(int)sdsout->ixmax;

	if(sdsout->n == 0){
	    unlink((char *)sdsout->path);
	    DFSDclear();
	    DFSDrestart();
        DFSDsetNT(DFNT_FLOAT32);
	    if(DFSDputdata((char *)sdsout->path,rank,size,(float *)data)){
	        sprintf(WarningBuff,"writesds3dFloat DFSDputdata error %d",DFerror);
	        Warning(WarningBuff);
	        goto OutOfHere;
	    }
	}else{
	    if(DFSDadddata((char *)sdsout->path,rank,size,(float *)data)){
	        sprintf(WarningBuff,"writesds3dFloat DFSDadddata error %d",DFerror);
	        Warning(WarningBuff);
	        goto OutOfHere;
	    }
	}
	

	lastref=DFSDlastref();
	if(lastref == -1){
	    sprintf(WarningBuff,"writesds3dFloat DFSDlastref error %d",DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}

	if(DFANputlabel(sdsout->path,DFTAG_SDG,lastref,sdsout->name) == -1){
	    sprintf(WarningBuff,"writesds3dFloat DFANputlabel %s Name %s lastref %d error %d",
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
	    sprintf(WarningBuff,"writesds3dFloat DFANputdesc %s Name %s lastref %d DFerror %d",
		sdsout->path,sdsout->name,lastref,DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}
	ret = 0;
OutOfHere:
	if(data)cFree((char *)data);
	data=NULL;
	return ret;
}
static int writesds2dDouble(struct SDS2Dout *sdsout)
{
	extern int DFerror;
	int32 rank,size[2];
	double vmin,vmax;
	char buff[256];
	long n,length;
	int lastref;
	double v;
	int ret;
	
	ret=1;
	
	if(!sdsout){
	    Warning("writesds2dDouble Found NULL struct pointer\n");
		goto OutOfHere;
	}
	
	if(sdsout->type != DATA_TYPE_DOUBLE){
	    sprintf(WarningBuff,"writesds2dDouble Found Wrong data type %d\n",sdsout->type);
	    Warning(WarningBuff);
		goto OutOfHere;
	}
	
	if(!sdsout->data){
	    Warning("writesds2dDouble Found NULL data Pointer\n");
		goto OutOfHere;
	}
	
	length=sdsout->ixmax*sdsout->iymax;
	
	if(length <= 0){
	    Warning("writesds2dDouble Found data length less than one\n");
		goto OutOfHere;
	}
	

	vmin =  1e33;
	vmax = -1e33;
	for(n=0;n<length;++n){
	    v=sdsout->data[n];
	    if(v < vmin)vmin = v;
	    if(v > vmax)vmax = v;
	}
	
	rank=2;
	size[0]=(int)sdsout->iymax;
	size[1]=(int)sdsout->ixmax;

	if(sdsout->n == 0){
	    unlink((char *)sdsout->path);
	    DFSDclear();
	    DFSDrestart();
        DFSDsetNT(DFNT_FLOAT64);
	    if(DFSDputdata((char *)sdsout->path,rank,size,(float *)sdsout->data)){
	        sprintf(WarningBuff,"writesds2dDouble DFSDputdata error %d",DFerror);
	        Warning(WarningBuff);
	        goto OutOfHere;
	    }
	}else{
	    if(DFSDadddata((char *)sdsout->path,rank,size,(float *)sdsout->data)){
	        sprintf(WarningBuff,"writesds2dDouble DFSDadddata error %d",DFerror);
	        Warning(WarningBuff);
	        goto OutOfHere;
	    }
	}
	

	lastref=DFSDlastref();
	if(lastref == -1){
	    sprintf(WarningBuff,"writesds2dDouble DFSDlastref error %d",DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}

	if(DFANputlabel(sdsout->path,DFTAG_SDG,lastref,sdsout->name) == -1){
	    sprintf(WarningBuff,"writesds2dDouble DFANputlabel %s Name %s lastref %d error %d",
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
	    sprintf(WarningBuff,"writesds2dDouble DFANputdesc %s Name %s lastref %d DFerror %d",
		sdsout->path,sdsout->name,lastref,DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}
	ret = 0;
OutOfHere:
	return ret;
}

static int writesds2dBytes(struct SDS2Dout *sdsout)
{
	extern int DFerror;
	double vmin,vmax;
	char buff[256];
	long n,length;
	int lastref;
	unsigned char *data;
	double v;
	int ret;
	
	ret=1;
	
	data=NULL;

	if(!sdsout){
	    Warning("writesds2dBytes Found NULL struct pointer\n");
		goto OutOfHere;
	}
	
	if(sdsout->type != DATA_TYPE_BYTE){
	    sprintf(WarningBuff,"writesds2dBytes Found Wrong data type %d\n",sdsout->type);
	    Warning(WarningBuff);
		goto OutOfHere;
	}
	
	if(!sdsout->data){
	    Warning("writesds2dBytes Found NULL data Pointer\n");
		goto OutOfHere;
	}
	
	length=sdsout->ixmax*sdsout->iymax;
	
	if(length <= 0){
	    Warning("writesds2dBytes Found data length less than one\n");
		goto OutOfHere;
	}
	
	data=(unsigned char *)cMalloc(length*sizeof(unsigned char),1001);
	if(!data){
	    sprintf(WarningBuff,"writesds2dBytes error Trying To allocate %ld Bytes\n",length*sizeof(unsigned char));
	    Warning(WarningBuff);
		goto OutOfHere;
	}


	vmin =  1e33;
	vmax = -1e33;
	for(n=0;n<length;++n){
	    v=sdsout->data[n];
	    if(v < vmin)vmin = v;
	    if(v > vmax)vmax = v;
	}
	
	if(vmax <= vmin){
		vmax=vmin+1+2*fabs(vmin);
		for(n=0;n<length;++n){
		    data[n]=(unsigned char)(2);
		}
	}else{
		for(n=0;n<length;++n){
			v=sdsout->data[n];
			data[n]=(unsigned char)(2+252.*(v-vmin)/(vmax-vmin));
		}
	}
		
	if(sdsout->n == 0){
	    unlink((char *)sdsout->path);
		DFR8restart();
	    if(DFR8putimage((char *)sdsout->path,data,(int)sdsout->ixmax,(int)sdsout->iymax,11)){
		    goto OutOfHere;
	    }

	}else{
	    if(DFR8addimage((char *)sdsout->path,data,(int)sdsout->ixmax,(int)sdsout->iymax,11)){
		    goto OutOfHere;
	    }
	}
	

	lastref=DFR8lastref();
	if(lastref == -1){
	    sprintf(WarningBuff,"writesds2dBytes DFR8lastref error %d",DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}

	if(DFANputlabel(sdsout->path,DFTAG_SDG,lastref,sdsout->name) == -1){
	    sprintf(WarningBuff,"writesds2dBytes DFANputlabel %s Name %s lastref %d error %d",
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
	    sprintf(WarningBuff,"writesds2dBytes DFANputdesc %s Name %s lastref %d DFerror %d",
		sdsout->path,sdsout->name,lastref,DFerror);
	    Warning(WarningBuff);
	    goto OutOfHere;
	}
	ret = 0;
OutOfHere:
	if(data)cFree((char *)data);
	data=NULL;
	return ret;
}


static int writesds2dFloat(struct SDS2Dout *sdsout)
{
	extern int DFerror;
	int32 rank,size[2];
	double vmin,vmax;
	char buff[256];
	long n,length;
	int lastref;
	float *data;
	double v;
	int ret;
	
	ret=1;
	
	data=NULL;

	if(!sdsout){
	    Warning("writesds2dFloat Found NULL struct pointer\n");
		goto OutOfHere;
	}
	
	if(sdsout->type != DATA_TYPE_FLOAT){
	    sprintf(WarningBuff,"writesds2dFloat Found Wrong data type %d\n",sdsout->type);
	    Warning(WarningBuff);
		goto OutOfHere;
	}
	
	if(!sdsout->data){
	    Warning("writesds2dFloat Found NULL data Pointer\n");
		goto OutOfHere;
	}
	
	length=sdsout->ixmax*sdsout->iymax;
	
	if(length <= 0){
	    Warning("writesds2dFloat Found data length less than one\n");
		goto OutOfHere;
	}
	
	data=(float *)cMalloc(length*sizeof(float),1002);
	if(!data){
	    sprintf(WarningBuff,"writesds2dFloat error Trying To allocate %ld Bytes\n",length*sizeof(float));
	    Warning(WarningBuff);
		goto OutOfHere;
	}


	vmin =  1e33;
	vmax = -1e33;
	for(n=0;n<length;++n){
	    v=sdsout->data[n];
	    data[n]=(float)v;
	    if(v < vmin)vmin = v;
	    if(v > vmax)vmax = v;
	}
	
	rank=2;
	size[0]=(int)sdsout->iymax;
	size[1]=(int)sdsout->ixmax;

	if(sdsout->n == 0){
	    unlink((char *)sdsout->path);
	    DFSDclear();
	    DFSDrestart();
        DFSDsetNT(DFNT_FLOAT32);
	    if(DFSDputdata((char *)sdsout->path,rank,size,(float *)data)){
	        sprintf(WarningBuff,"writesds2d DFSDputdata error %d",DFerror);
	        Warning(WarningBuff);
	        goto OutOfHere;
	    }
	}else{
	    if(DFSDadddata((char *)sdsout->path,rank,size,(float *)data)){
	        sprintf(WarningBuff,"writesds2d DFSDadddata error %d",DFerror);
	        Warning(WarningBuff);
	        goto OutOfHere;
	    }
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
	ret = 0;
OutOfHere:
	if(data)cFree((char *)data);
	data=NULL;
	return ret;
}
char *cMalloc(unsigned long length,int tag)
{
        char *ret;

        tag=tag;

        ret=(char *)malloc(length+4L);
        if(ret == NULL){
            sprintf(WarningBuff,"cMalloc Out of Memory Requested (%ld)",length);
	    	Warning(WarningBuff);
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
    fprintf(stderr,"%s",mess);
    return 0;
}

