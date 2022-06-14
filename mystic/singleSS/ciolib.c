#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

double rtime(void);
int GetTime(long *Seconds,long *milliseconds);


/*    ciolib   Version  3	*/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

#ifdef CRAY
#include <fortran.h>
#define copen		COPEN
#define ccreate		CCREATE
#define cwrite		CWRITE
#define cread		CREAD
#define cclose		CCLOSE
#define cclear		CCLEAR
#define second		SECOND
#else
#define copen		copen_
#define ccreate		ccreate_
#define cwrite		cwrite_
#define cread		cread_
#define cclose		cclose_
#define cclear		cclear_
#define second		second2_

typedef char *_fcd;

#define _fcdtocp	   
#define _fcdlen         strlen

#endif

typedef long long intret;


#define MAXFILE         100
#define MAXSTRING       256
#define WORDSIZE          8

struct files{
    FILE *fp;
    int iopen;
    int  ierror;
    char name[MAXSTRING];
};

static struct files file[MAXFILE];

static void cerror(char *msg);

intret copen(intret *iui, _fcd str)
{
        FILE *fp;
        char buff[256];
        long iu;
        int n;

        if(!iui){
            cerror("copen: NULL unit number pointer");
            return -1;
        }

        iu = *iui;

        if(iu < 0 || iu >= MAXFILE){
            sprintf(buff,"copen: unit number %ld out of range (0 < %ld)",iu,MAXFILE);
            cerror(buff);
            return -1;
        }

        if(file[iu].iopen){
            file[iu].ierror = TRUE;
            sprintf(buff,"copen: file %s unit %d already open",file[iu].name,iu);
            cerror(buff);
            return -1;
        }

        for(n=0;n<MAXSTRING;++n)file[iu].name[n]=0;

        strncpy(file[iu].name,_fcdtocp(str),_fcdlen(str));

        for(n=MAXSTRING-1;n>0;--n){
            if(file[iu].name[n] == 0 || file[iu].name[n] == ' ')continue;
            if(n < MAXSTRING-2){
                file[iu].name[n+1]=0;
            }else{
                file[iu].name[MAXSTRING-1]=0;
            }
            break;
        }

        
        for(n=0;n<MAXSTRING;++n){
            if(file[iu].name[n] == 0 || file[iu].name[n] == ';' || file[iu].name[n] == ' '){
               file[iu].name[n] = 0;
               break;
            }
        }
 
        
        fprintf(stderr,"file[iu].name '%s'\n",file[iu].name);


        fp=fopen(file[iu].name,"rb");
        if(fp == NULL){
            sprintf(buff,"copen: Could Not Open file %s unit %d",file[iu].name,iu);
            cerror(buff);
            return -1;
        }

        file[iu].ierror=FALSE;
        file[iu].iopen=TRUE;
        file[iu].fp=fp;

        return 0;
}
intret ccreate(intret *iui, _fcd str)
{
        FILE *fp;
        char buff[256];
        long iu;
        int n;

        if(!iui){
            cerror("ccreate: NULL unit number pointer");
            return -1;
        }

        iu = *iui;

        if(iu < 0 || iu >= MAXFILE){
            sprintf(buff,"ccreate: unit number %ld out of range (0 < %ld)",iu,MAXFILE);
            cerror(buff);
            return -1;
        }

        if(file[iu].iopen){
            file[iu].ierror = TRUE;
            sprintf(buff,"ccreate: file %s unit %d already open",file[iu].name,iu);
            cerror(buff);
            return -1;
        }

        for(n=0;n<MAXSTRING;++n)file[iu].name[n]=0;

        strncpy(file[iu].name,_fcdtocp(str),_fcdlen(str));


        for(n=0;n<MAXSTRING;++n){
            if(file[iu].name[n] == 0 || file[iu].name[n] == ';' || file[iu].name[n] == ' '){
               file[iu].name[n] = 0;
               break;
            }
        }

        for(n=MAXSTRING-1;n>0;--n){
            if(file[iu].name[n] == 0 || file[iu].name[n] == ' ')continue;
            if(n < MAXSTRING-2){
                file[iu].name[n+1]=0;
            }else{
                file[iu].name[MAXSTRING-1]=0;
            }
            break;
        }

	fp=fopen(file[iu].name,"rb+");
	if(fp == NULL){
	    fp=fopen(file[iu].name,"wb+");
	    if(fp == NULL){
                sprintf(buff,"ccreate: Could Not Open file %s unit %d",file[iu].name,iu);
                cerror(buff);
                return -1;
	    }
	}

        file[iu].ierror=FALSE;
        file[iu].iopen=TRUE;
        file[iu].fp=fp;

        return 0;
}
intret cwrite(intret *iui,unsigned char *data,intret *counti,intret *addressi)
{
        long iu,count,address;
        unsigned int ret;
        char buff[256];

        if(!iui || !counti || !addressi || !data){
            sprintf(buff,"cwrite: NULL Pointer unit %ld data %ld count %ld adress %ld",
                    iui,data,counti,addressi);
            cerror(buff);
            return 1;
        }

        iu= *iui;

        if(iu < 0 || iu >= MAXFILE){
            sprintf(buff,"cwrite: unit number %ld out of range (0 < %ld)",iu,MAXFILE);
            cerror(buff);
            return 1;
        }

        if(!file[iu].iopen){
            sprintf(buff,"cwrite: unit number %ld not open",iu);
            cerror(buff);
            return 1;
        }

        if(file[iu].ierror){
            sprintf(buff,"cwrite: unit number %ld error flag set",iu);
            cerror(buff);
            return 1;
        }

        count= *counti;
        address= *addressi;
/*
	fprintf(stderr,"cwrite iui %lld counti %lld addressi %lld\n",*iui,*counti,*addressi);
*/
        if(fseek(file[iu].fp, address*WORDSIZE, SEEK_SET)){
            file[iu].ierror=TRUE;
            sprintf(buff,"cwrite: fseek errror\n file %s unit %d address %ld",
                    file[iu].name,iu,address);
            cerror(buff);
            return 1;
        }

	ret = fwrite(data,WORDSIZE,count,file[iu].fp);
        if(ret != count){
            file[iu].ierror=TRUE;
            sprintf(buff,"cwrite: write error\n file %s unit %ld address %d\n words requested: %ld words written: %ld",
                    file[iu].name,iu,address,count,ret);
            cerror(buff);
            return 1;
        }

        return 0;

}
intret cread(intret *iui,unsigned char *data,intret *counti,intret *addressi)
{
        long iu,count,address;
        unsigned int ret;
        char buff[256];

        if(!iui || !data || !counti || !addressi){
            sprintf(buff,"cread: NULL Pointer unit %ld data %ld count %ld adress %ld",
                    iui,data,counti,addressi);
            cerror(buff);
            return 1;
        }

        iu= *iui;

        if(iu < 0 || iu >= MAXFILE){
            sprintf(buff,"cread: unit number %ld out of range (0 < %ld)",iu,MAXFILE);
            cerror(buff);
            return 1;
        }

        if(!file[iu].iopen){
            sprintf(buff,"cread: unit number %ld not open",iu);
            cerror(buff);
            return 1;
        }

        if(file[iu].ierror){
            sprintf(buff,"cread: unit number %ld error flag set",iu);
            cerror(buff);
            return 1;
        }

        count= *counti;
        address= *addressi;
/*
	fprintf(stderr,"cread iui %lld counti %lld addressi %lld\n",*iui,*counti,*addressi);
*/
        if(fseek(file[iu].fp, address*WORDSIZE, SEEK_SET)){
            file[iu].ierror=TRUE;
            sprintf(buff,"cread: fseek errror\n file %s unit %d address %ld",
                    file[iu].name,iu,address);
            cerror(buff);
            return 1;
        }

        ret=fread(data,WORDSIZE,count,file[iu].fp);

        if(ret != count){
            file[iu].ierror=TRUE;
            sprintf(buff,"cread: read error\n file %s unit %ld address %d\n words requested: %ld words read: %ld",
                    file[iu].name,iu,address,count,ret);
            cerror(buff);
            return 1;
        }

        return 0;
}
intret cclose(intret *iui)
{
        char buff[256];
        long iu;

        if(!iui){
            cerror("cclose: NULL pointer to unit number");
            return -1;
        }

        iu= *iui;

        if(iu < 0 || iu >= MAXFILE){
            sprintf(buff,"cclose: unit number %ld out of range (0 < %ld)",iu,MAXFILE);
            cerror(buff);
            return -1;
        }

        if(!file[iu].iopen){
            sprintf(buff,"cclose: unit number %ld not open",iu);
            cerror(buff);
            return -1;
        }

        file[iu].iopen=FALSE;
        if(file[iu].fp)fclose(file[iu].fp);
        file[iu].fp=NULL;
        return 0;
}
intret cclear(intret *iui)
{
        char buff[256];
        long iu;

        if(!iui){
            cerror("cclear: NULL pointer to unit number");
            return -1;
        }

        iu= *iui;

        if(iu < 0 || iu >= MAXFILE){
            sprintf(buff,"cclear: unit number %ld out of range (0 < %ld)",iu,MAXFILE);
            cerror(buff);
            return -1;
        }

        if(!file[iu].iopen){
            sprintf(buff,"cclear: unit number %ld not open",iu);
            cerror(buff);
            return -1;
        }

        file[iu].ierror=FALSE;

	if(file[iu].fp)clearerr(file[iu].fp);

        return 0;
}
static void cerror(char *msg)
{
	fprintf(stderr,"%s\n",msg);
}
/*
intret second_(double  *t)
{
	time_t ship;
	unsigned itime;

        itime=time(&ship);
        *t=itime;
        return itime;
}
*/
void second(double  *t)
{
	if(!t)return;
	*t=rtime();
	return;
}
double rtime(void)
{
        long milliseconds;
        long Seconds;
        double ret;


        GetTime(&Seconds,&milliseconds);

        ret=(double)Seconds+(double)milliseconds/1000.;

        return ret;

}
int GetTime(long *Seconds,long *milliseconds)
{
        struct timeval curTime;

        gettimeofday(&curTime,(struct timezone *) NULL);
        *Seconds=curTime.tv_sec;
        *milliseconds=curTime.tv_usec/1000;
        return 0;
}

