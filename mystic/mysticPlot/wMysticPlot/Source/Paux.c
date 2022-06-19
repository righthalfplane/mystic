#include "Paux.h"
#include "Xappl.h"
#include "uFiles.h"
#include <sys/timeb.h>
static int xold;
static int yold;

int putLongLow(unsigned long n,FILE *out);
int putIntLow(unsigned int i,FILE *out);
int putString(unsigned char *data,long length,FILE *out);
int getLongLow(unsigned long *n,FILE *out);
int getIntLow(unsigned int *i,FILE *in);
int flip(unsigned char *b,int xsize,int ysize);
int getString(char *s,long n,FILE *in); 

int doBmp(char *name,unsigned char *image,int ixmax,int iymax)
{
	FILE *out;
	unsigned long size,offset,imagesize,where,linesize;
	unsigned long xPixelPerMeter,yPixelPerMeter;
	int n;
	int ret;

	if(!name || !image)return 1;

	out=fopen(name,"wb");
	if(!out){
	    fprintf(stderr,"Could Not Open %s To Write\n",name);
	    return 1;
	}

	xPixelPerMeter = 0;
	yPixelPerMeter = 0;
/*
	xPixelPerMeter = 144*39.37;
	yPixelPerMeter = 144*39.37;
*/

	ret = 1;

	offset=14L+40L;

	linesize=12*((ixmax+3)/4);

	imagesize = linesize*iymax;

	size=offset+imagesize;

	/* File Header */
	
	if(putString("BM",2L,out))goto ErrorOut;

	if(putLongLow(size,out))goto ErrorOut;

	if(putIntLow(0,out))goto ErrorOut;

	if(putIntLow(0,out))goto ErrorOut;

	if(putLongLow(offset,out))goto ErrorOut;

	/* BitMap Header */

	if(putLongLow(40,out))goto ErrorOut;

	if(putLongLow(ixmax,out))goto ErrorOut;

	if(putLongLow(iymax,out))goto ErrorOut;

	if(putIntLow(1,out))goto ErrorOut;

	if(putIntLow(24,out))goto ErrorOut;

	if(putLongLow(0,out))goto ErrorOut;

	if(putLongLow(imagesize,out))goto ErrorOut;

	if(putLongLow(xPixelPerMeter,out))goto ErrorOut;

	if(putLongLow(yPixelPerMeter,out))goto ErrorOut;

	if(putLongLow(0,out))goto ErrorOut;

	if(putLongLow(0,out))goto ErrorOut;

	where=ftell(out);

	if(where != offset){
	    fprintf(stderr,"Error File %s where %ld offset %ld\n",name,where,offset);
	}

	flip(image,ixmax,iymax);

	if(((ixmax+3)/4)*4 != ixmax){
	    fprintf(stderr,"Warning File %s ixmax %d is not a Multiple of 4\n",name,ixmax);
	    for(n=0;n<iymax;++n){
	        if(putString(&image[n*3L*ixmax],linesize,out))goto ErrorOut;
	    }
	}else{
	    if(putString(image,imagesize,out))goto ErrorOut;
	    if((long)imagesize != (3L*ixmax*iymax)){
	        fprintf(stderr,"Warning File %s ixmax %d  iymax %d imagesize %ld\n",name,
	                  ixmax,iymax,imagesize);
	    }
	}

	where=ftell(out);

	if(where != size){
	    fprintf(stderr,"Error File %s where %ld size %ld\n",name,where,size);
	}

	ret = 0;
ErrorOut:
	if(out)fclose(out);
	return ret;
}
/*int flip2(unsigned char *b,int xsize,int ysize)
{
	register unsigned char *p1,*p2,c1,c2,c3;
	int x,y,xinc;

	if(!b)return 1;

	xinc=xsize*3;

	for(y=0;y<ysize/2;++y){
		p1=b+y*xinc;
		p2=b+(ysize-1-y)*xinc;
		for(x=0;x<xsize;++x){
			c1 = *p1;
			c2 = *(p1+1);
			c3 = *(p1+2);
			*p1++ = *(p2+2);
			*p1++ = *(p2+1);
			*p1++ = *p2;
			*p2++ = c3;
			*p2++ = c2;
			*p2++ = c1;
		}
	}

	if(ysize & 1){
	    y=ysize/2;
	    p1=b+y*xinc;
	    for(x=0;x<xsize;++x){
		c1 = *p1;
		c2 = *(p1+1);
		c3 = *(p1+2);
		*p1++ = c3;
		*p1++ = c2;
		*p1++ = c1;
	    }

	}
	return 0;
}*/
/*int getIntLow(unsigned int *i,FILE *in)
{
      unsigned char c[2];
      unsigned int c0,c1;

        if(!i || !in)return 1;
        if(getString(c,2L,in))return 1;
	c0=c[0];
	c1=c[1];
        *i = c0 + (c1 << 8);
        return 0;
}
int getLongLow(unsigned long *n,FILE *out)
{
      unsigned char c[4];
      unsigned long c0,c1,c2,c3;

        if(!n || !out)return 1;

        if(getString(c,4L,out))return 1;

        c0=c[0];
        c1=c[1];
        c2=c[2];
        c3=c[3];

	*n = c0 + (c1 << 8) + (c2 << 16) + (c3 << 24);
	return 0;
}*/
int putIntLow(unsigned int i,FILE *out)
{
      unsigned char c[2];

        if(!out)return 1;
        c[0] = i & 255;
        c[1] = (i >> 8) & 255;
        return putString(c,2L,out);
}
int putLongLow(unsigned long n,FILE *out)
{
      unsigned char c[4];

        if(!out)return 1;

        c[0] = (unsigned char)(n & 255);
        c[1] = (unsigned char)((n >> 8) & 255);
        c[2] = (unsigned char)((n >> 16) & 255);
        c[3] = (unsigned char)((n >> 24) & 255);

        return putString(c,4L,out);

}
int gettimeofday(struct timeval *curTime,struct timezone *v)
{
	struct timeb t;

	ftime(&t);
	curTime->tv_sec=t.time;
	curTime->tv_usec=t.millitm*1000;
	return 0;
}
int MoveToM(int x,int y)
{
	xold=x;
	yold=y;
	return 0;
}
int LineToM(int x,int y,HWND mywindow,HDC hdc)
{
		POINT p[2];

		p[0].x=xold;
		p[0].y=yold;
		p[1].x=x;
		p[1].y=y;
	    Polyline(hdc,p,2);
        xold=x;
        yold=y;
	return 0;
}
int DrawString(char *buff,HWND mywindow,HDC hdc,int len)
{

	TextOut(hdc,xold,yold,buff,strlen(buff));

	return 0;
}
int Warning(char *message)
{
	if(!message)return 1;
	
	if(IsShift())return 1;

	MessageBox(HWND_DESKTOP,message,"Warning",MB_OK);

	return 0;
}
