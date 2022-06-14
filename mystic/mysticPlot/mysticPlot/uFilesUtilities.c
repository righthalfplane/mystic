#define EXTERN22 extern
#include "Xappl.h"
#include "uFiles.h"
#include "Universal.h"


#define uint16 unsigned int
#define DFTAG_SDG   ((uint16)700) /* Scientific Data Group*/
int DFR8restart(void);
int DFANputlabel(char *file,unsigned short int type,unsigned short int tag,char *name);
int DFANputdesc(char *file,unsigned short int type,unsigned short int tag,char *name,int lenght);
int DFSDputdata(char *path,int rank,int *size,float *data);
int DFSDadddata(char *path,int rank,int *size,float *data);
int DFSDlastref(void);
int DFSDrestart(void); 
int DFSDclear(void); 
int DFR8putimage(char *name,char *buffout,int ixmax,int iymax,int type);
int DFR8setpalette(char *name);
int DFR8addimage(char *name,char *buffout,int ixmax,int iymax,int type);
unsigned short int DFR8lastref(void);
unsigned short int DFANlastref(void);
int DFerrno(void);

int DFSDsetNT(int data_type);

int Stuff(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,unsigned char *dataOut,long Lin,long *Lout,unsigned int mask);
int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits);
int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);
int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);
int sdsOut(char *path,char *name,char *limits,int rank,int *size,double *data,long n);
int getString(unsigned char *s,long n,FILE *in);
int getInt(int *i,FILE *in);
int getLong(long *n,FILE *in);
int putString(unsigned char *s,long n,FILE *in);
int putLong(long n,FILE *out);
int putInt(long n,FILE *out);
float *sdsToFloat(char *path,int rank,int *size,double *data);

int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits)
{
	int rank,size[3];
	char *command;
	int ret;
	
	rank=3;
	size[0]=(int)zsize;
	size[1]=(int)ysize;
	size[2]=(int)xsize;
	
	if(limits && limits->iGot){
	    command=(char *)limits->command;
	}else{
	    command=NULL;
	}
	
	ret=sdsOut(path,name,command,rank,size,data,n);
	
	if(!ret && flag){
		ret=SetFileType(path,"SDS3");
	}
	
	return ret;
	
}
int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits)
{
	int rank,size[2];
	char *command;
	int ret;
	
	ret = 1;
	
	rank=2;
	size[0]=(int)ysize;
	size[1]=(int)xsize;
	
	if(limits && limits->iGot){
	    command=(char *)limits->command;
	}else{
	    command=NULL;
	}
	
	if(sdsOut(path,name,command,rank,size,data,n))goto ErrorOut;
	
	if(flag){
		if(SetFileType(path,"SDS2"))goto ErrorOut;
	}
	ret = 0;
ErrorOut:
	return ret;
	
}
float *sdsToFloat(char *path,int rank,int *size,double *data)
{
	long length,n;
	float *dat;
	
	
	if(!path || !size || !data)return NULL;
	
	dat=NULL;
	
	length=1;
	
	for(n=0;n<rank;++n){
	   length *= size[n];
	}
	
	if(length <= 1)goto ErrorOut;
	
	dat=(float *)cMalloc(length*sizeof(float),7777);
	if(!dat)goto ErrorOut;
	
	for(n=0;n<length;++n){
	    dat[n]=(float)data[n];
	}
	
	
ErrorOut:

	if(!dat){
	    WarningBatch("sdsToFloat Failed\n");
	}
	
	return dat;
}

int sdsOut(char *path,char *name,char *limits,int rank,int *size,double *data,long n)
{
	float *dataFloat;
	int lastref;
	int ret;
	
	if(!path || !size || !data)return 1;
	
	
	
	ret = 1;
	
	dataFloat=NULL;
    if(!uPref.SaveSDS64Bits){
       dataFloat=sdsToFloat(path,rank,size,data);
       if(!dataFloat)goto OutOfHere;
       data=(double *)dataFloat;
    }
	
	if(n == 0){
	    unlink((char *)path);
        DFSDclear();
        DFSDrestart();
        if(uPref.SaveSDS64Bits){
           DFSDsetNT(DFNT_FLOAT64);
        }else{
           DFSDsetNT(DFNT_FLOAT32);
        }
	    if(DFSDputdata((char *)path,rank,size,(float *)data)){
		    goto OutOfHere;
	    }
	}else{	    
	    if(DFSDadddata((char *)path,rank,size,(float *)data)){
		    goto OutOfHere;
	    }
	}
    lastref=DFSDlastref();
    if(lastref == -1){
	    sprintf(WarningBuff,"sdsOut DFSDlastref error %d n %ld\n", DFerrno(),n);
	    WarningBatch(WarningBuff);
        goto OutOfHere;
    }
    
    if(name && name[0]){
	    if(DFANputlabel((char *)path,(unsigned short)DFTAG_SDG,(unsigned short)lastref,name) == -1){
		    sprintf(WarningBuff,"sdsOut DFANputlabel %s Name %s lastref %d error %d n %ld\n",
		               path,name,lastref, DFerrno(),n);
		    WarningBatch(WarningBuff);
	        goto OutOfHere;
	    }
    }

    if(limits && limits[0]){
        if(DFANputdesc((char *)path,(unsigned short)DFTAG_SDG,(unsigned short)lastref,(char *)limits,(int)strlen((char *)limits)) == -1){
	        sprintf(WarningBuff,"sdsOut DFANputdesc %s lastref %d error %d n %ld\n",
	               path,lastref, DFerrno(),n);
	        WarningBatch(WarningBuff);
            goto OutOfHere;
        }
    }
	
	
	
	ret=0;
OutOfHere:

	if(dataFloat)cFree((char *)dataFloat);
	
	return ret;
}
int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits)
{
	char *command;
	int lastref;
	int ret;
	
	if(!filepath)return 1;
	
	ret = 1;
	
	if(limits && limits->iGot){
	    command=(char *)limits->command;
	}else{
	    command=NULL;
	}
			
	if(n == 0){
    	unlink((char *)filepath);
		DFR8restart();
		if(palette){
			if(DFR8setpalette((char *)palette))goto ErrorOut;
		}	     
	    if(DFR8putimage((char *)filepath,data,(int)xsize,(int)ysize,11)){
		    goto ErrorOut;
	    }
	}else{
	    
	    if(DFR8addimage((char *)filepath,data,(int)xsize,(int)ysize,11)){
		    goto ErrorOut;
	    }
	}
    lastref=DFR8lastref();
    if(lastref == -1){
	    sprintf(WarningBuff,"DFR8lastref error %d\n", DFerrno());
	    WarningBatch(WarningBuff);
        goto ErrorOut;
    }
	
    if(name){
	    if(DFANputlabel(filepath,(unsigned short)306,(unsigned short)lastref,name) == -1){
		    sprintf(WarningBuff,"DFANputlabel %s Name %s lastref %d error %d\n",
		               filepath,name,lastref, DFerrno());
		    WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
    }
    
    if(command){
        if(DFANputdesc(filepath,(unsigned short)306,(unsigned short)lastref,(char *)command,(int)strlen((char *)command)) == -1){
	        sprintf(WarningBuff,"DFANputdesc %s Name %s lastref %d error %d\n",
	               filepath,name,lastref, DFerrno());
	        WarningBatch(WarningBuff);
            goto ErrorOut;
        }
    }

		
	if(flag){
		if(SetFileType(filepath,"HDFR"))goto ErrorOut;
	}
	
ErrorOut:

	ret = 0;
		
	return ret;
	
}
int UnCompress(unsigned int Esc1,unsigned int Esc2,long UnCompressed,long Compressed,unsigned char *fData,unsigned char *oData)
{
	unsigned int c,c1,c2,c3,k,length;
	unsigned int i1,i2;
	long n,nn;
	
	nn=0;
	for(n=0;n<Compressed;n += 3){
	    c = *fData++;
	    if(c == Esc1){
	        length = *fData++;
	        n += 2;
	    }else if(c == Esc2){
	        i1 = *fData++;
	        i2 = *fData++;
	        length = (i2 << 8) | i1;
	        n += 3;
	    }else{
	        if((nn += 3)  > UnCompressed){
	            sprintf(WarningBuff,"UnCompress Error #1 %ld > %ld\n",nn,UnCompressed);
		        WarningBatch(WarningBuff);
	            return 1;
	        }
	        *oData++ = c;
	        *oData++ = *fData++;
	        *oData++ = *fData++;
	        continue;
	    }
	    c1 = *fData++;
	    c2 = *fData++;
	    c3 = *fData++;
	    for(k=0;k<length;++k){
	        if((nn += 3)  > UnCompressed){
	            sprintf(WarningBuff,"UnCompress Error #2 %ld > %ld length %d k %d",
	                        nn,UnCompressed,length,k);
		        WarningBatch(WarningBuff);
	            return 1;
	        }
	        *oData++ = c1;
	        *oData++ = c2;
	        *oData++ = c3;
	    }
	}
	if(nn != UnCompressed){
	    sprintf(WarningBuff,"UnCompress Error #3 %ld  != %ld",nn,UnCompressed);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	return 0;
}
int Stuff(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,unsigned char *dataOut,long Lin,long *Lout,unsigned int mask)
{
        unsigned char entry,c1,c2,c3,*dataStart;
        unsigned int E1,E2;
        long nuse[256],n1,n2,k,kk,kkn;
        int k1,k2,n;

        for(n=0;n<256;++n){
           nuse[n]=0;
        }

        for(k=0;k<Lin;++k){
            dataIn[k] &= mask;
            entry=dataIn[k];
            ++nuse[entry];
        }

        n1=300000000;
        k1 = -1;
        n2=300000000;
        k2 = -1;
        for(n=0;n<256;++n){
           k=nuse[n];
           if(k < n1){
               n1=k;
               k1=n;
           }else if(k < n2){
               n2=k;
               k2=n;
           }

        }
        if(k1 < 0 || k2 < 0){
            Warning("Stuff Packing Error");
            return 1;
        }
        E1=k1;
        E2=k2;
        dataStart=dataOut;
        for(k=0;k<Lin;k += 3){
            c1=dataIn[k];
            c2=dataIn[k+1];
            c3=dataIn[k+2];
            kk=3;
            while(c1 == dataIn[k+kk] && 
                  c2 == dataIn[k+kk+1] && 
                  c3 == dataIn[k+kk+2]){
                if((kk+k) >= Lin || kk > 180000){
                    break;
                }
                kk += 3;
            }
            kkn=kk/3;
            if(kkn >= 2 || c1 == E1 || c1 == E2){
                 if(kkn < 255){
                    *dataOut++ = E1;
                    *dataOut++ = (unsigned char)(kkn);
                }else{
                    *dataOut++ = E2;
                    *dataOut++ = (unsigned char)(kkn & 255);
                    *dataOut++ = (unsigned char)((kkn >> 8) & 255);
                }
                k  += 3*(kkn-1);
            }
            *dataOut++ = c1;
            *dataOut++ = c2;
            *dataOut++ = c3;
        }
        *Esc1 = E1;
        *Esc2 = E2;
        *Lout = dataOut-dataStart;
        return 0;
}
int putLong(long n,FILE *out)
{
      unsigned char c[4];

	if(!out)return 1;

        c[3] = (unsigned char)(n & 255);
        c[2] = (unsigned char)((n >> 8) & 255);
        c[1] = (unsigned char)((n >> 16) & 255);
        c[0] = (unsigned char)((n >> 24) & 255);
        return putString((unsigned char *)c,4L,out);
}
int putInt(long n,FILE *out)
{
      unsigned char c[2];

	if(!out)return 1;

        c[1] = (unsigned char)(n & 255);
        c[0] = (unsigned char)((n >> 8) & 255);
        return putString((unsigned char *)c,2L,out);
}
int getInt(int *i,FILE *in)
{
        unsigned int c1,c2;
 
        if((c1=fgetc(in)) == EOF)return 1;
        if((c2=fgetc(in)) == EOF)return 1;
        *i = (c1 << 8) | c2;
        return 0;
 
}
int getLong(long *n,FILE *in)
{
        unsigned long c1,c2,c3,c4;
 
        if((c1=fgetc(in)) == EOF)return 1;
        if((c2=fgetc(in)) == EOF)return 1;
        if((c3=fgetc(in)) == EOF)return 1;
        if((c4=fgetc(in)) == EOF)return 1;
        *n = (long)((c1 << 24) | (c2 << 16) | (c3 << 8) | c4);
        return 0;
 
}
int putString(unsigned char *data,long length,FILE *out)
{
	if(!data || !out)return 1;
	if((long)fwrite(data,1,length,out) != length){
	    return 1;
	}
	return 0;
}
int getString(unsigned char *data,long length,FILE *out)
{
	if(!data || !out)return 1;
	if((long)fread(data,1,length,out) != length){
	    return 1;
	}
	return 0;
}
