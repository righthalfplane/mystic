#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <df.h>

//cc -O2 -o ToSds2d ToSds2d.c -Wall -ldf -ljpeg -lz


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




int main(int argc,char *argv[])
{
	FILE *inout;
	char *file=argv[1];
	int n,m,c,itWas;
	char buff[4096];
	char lab[4096],labold[4096];
	
	unsigned char *buffin,*bp;
	int blength=100000;
	
	buffin=(unsigned char *)malloc(blength);
	if(buffin == NULL){
		fprintf(stderr,"Memory Error blength %d\n",blength);
		exit(1);
	}
	
	
	if((inout=fopen(file,"rb")) == NULL){
		fprintf(stderr,"Cannot open file : %s to read%c\n",file,0);
		exit(1);
	}
	
	bp=buffin;
	
	
	double xmin=1e33;
	double xmax=-1e33;
	double ymin=1e33;
	double ymax=-1e33;
	
	int xsize=0;
	int ysize=0;
	int ysizel=0;
	
	labold[0]=0;
	int fout=0;
	int nn=0;
	int nb=0;
	int newline=0;
	
	while((m=(int)fread(buffin,1,blength,inout)) > 0){
		bp=(unsigned char *)buffin;
		for(n=0;n<m;++n){
			c = *bp++;
			if(c == ','){
				buff[nn++]=0;
				if(fout == 1){
					stpcpy(lab,buff);
					if(strcmp(lab,labold) == 0){
						;
					}else{
						if(newline){
							ysize++;
							nb=0;
						}
						newline=0;
					}
					stpcpy(labold,buff);
				}else if(fout == 2 || fout == 3){
					float f=atof(buff);
					if(f < xmin)xmin=f;
					if(f > xmax)xmax=f;
				}else if(fout > 5){
					if(nb > xsize){
					  xsize=nb;
					}
					++nb;
				}
				nn=0;
				fout++;
			}else if(c == '\n' && (itWas != '\r')){
				++ysizel;
				newline=1;
				fout=0;
				nn=0;
			}else if(c == '\r'){
				printf("CR Found\n");				
			}else{
			   buff[nn++]=c;
			}
			itWas = c;		
		}
		
	}
	
	ymin=0;
	ymax=ysize;
	
	
	fprintf(stderr,"xsize %d ysize %d ysizel %d\n",xsize,ysize,ysizel);
	fprintf(stderr,"xmin %g MHZ xmax %g MHZ\n",xmin/1e6,xmax/1e6);
	
	int ixmax=xsize;
	int iymax=ysize;
	
	rewind(inout);
	
	long length=xsize*ysize;
	
	
	double *buffout;
	buffout=(double *)malloc(length*sizeof(double));
	if(buffout == NULL){
		fprintf(stderr,"Memory Error blength %ld\n",(long)(length*sizeof(double)));
		exit(1);
	}
		
	labold[0]=0;
	fout=0;
	nn=0;
	nb=0;
	newline=0;
	int nc=0;
	while((m=(int)fread(buffin,1,blength,inout)) > 0){
		bp=(unsigned char *)buffin;
		for(n=0;n<m;++n){
			c = *bp++;
			if(c == ','){
				buff[nn++]=0;
				if(fout == 1){
					stpcpy(lab,buff);
					if(strcmp(lab,labold) == 0){
						;
					}else{
						if(newline){
							nc++;
							nb=0;
						}			
						if(nc >= ysize)goto OutOfHere1;
						newline=0;
					}
					stpcpy(labold,buff);
				}else if(fout > 5){
			    	float f=atof(buff);
					buffout[nb++ + nc*xsize]=f;
				}
				nn=0;
				fout++;
			}else if(c == '\n' && (itWas != '\r')){
				newline=1;
				fout=0;
				nn=0;
			}else if(c == '\r'){
				printf("CR Found\n");				
			}else{
			   buff[nn++]=c;
			}
			itWas = c;		
		}
		
	}
	
	
OutOfHere1:	
		(void)nb;
	 	struct SDS2Dout sdsout;
 		zerol((char *)&sdsout,sizeof(struct SDS2Dout));

		sdsout.data=buffout;
		sdsout.path="out33.sds";
		sdsout.name="Power(db)";
		sdsout.ixmax=ixmax;
		sdsout.iymax=iymax;
		sdsout.xmin=xmin;
		sdsout.xmax=xmax;
		sdsout.ymin=ymin;
		sdsout.ymax=ymax;
		sdsout.zmin=0;
		sdsout.zmax=0;
		sdsout.time=0.0;
		sdsout.n=0;
		sdsout.pioName="Power(db)";	
		sdsout.type=DATA_TYPE_FLOAT;	
				
		if(writesds(&sdsout))goto OutOfHere;
	
OutOfHere:
	
	
	if(buffin)free(buffin);
	
	if(inout)fclose(inout);
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

		(void)tag;
		
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

