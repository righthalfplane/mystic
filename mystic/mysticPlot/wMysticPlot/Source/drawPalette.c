#define EXTERN2 extern
#include "Xappl.h"
#include "uFiles.h"
#include <string.h>
#include <WINGDI.H>
#include "paletteDraw.h"
#include "uGraph.h"



struct fontData{
    int on;
    int xsize;
    int ysize;
    int first;
    int last;
    int xinc;
	unsigned char *buff;
};

struct fontData *fd;
int FontCount;
int getPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd);
int uFontSize(int *xsize,int *ysize,int fn);
int sear(double cmax,double cmin,double *zcon,int *ncon,int nmax);
int kpoint(int x,int y,int nc,struct screenData *s);
int LineB(int x,int y,int nc,struct screenData *s);
int FontOut(char *out,int fn,struct screenData *s);
int MoveB(int x,int y);
int FindB(int *x,int *y);
int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon);

int uCStringWidth(char *name,IconPtr myIcon);

int FontStart(IconPtr myIcon,HFONT hfont);

int FontToScreen(uRect *cRect,unsigned char *image,int nc,struct screenData *s);

int FontToRect(char *out,uRect *cRect,unsigned char **image,IconPtr myIcon);

int FontWrite(char *out,int *xsizei,int *ysizei,unsigned char **imagei,IconPtr myIcon)
{
	uRect textRect;
	HBITMAP hbm;
	HDC hdc;
	HDC hdcl;
	BITMAPINFOHEADER *ib;
	long xsize,xs,diff,ysize;
	BITMAPINFO ibitmap;
	HBITMAP hbitold;
	HFONT oldFont;
	unsigned char *image;
	int height,width;
	uFontInfo Info;
	int i,j;

	if(!out || !xsizei || !ysizei || !imagei || !myIcon)return 0;

	uGetFontInfo(myIcon,&Info);

	height=Info.LineHeight;

	width=uCStringWidth(out,myIcon);

	*imagei=cMalloc(height*width,9999);
	if(!*imagei)return 1;
	
	hbm=NULL;

	image=NULL;

	hdc=NULL;

	hbitold=NULL;

	oldFont=NULL;

		zerol((char *)&ibitmap,sizeof(BITMAPINFO));

		ib=&ibitmap.bmiHeader;

		  ib->biSize  = sizeof(BITMAPINFOHEADER);
		  ib->biPlanes  = 1;
		  ib->biBitCount  = 24;
		  xs=width;
		  xsize=xs/4;
		  xsize = 4*xsize;
		  diff=xs-xsize;
		  if(diff > 0)
		  xsize=xs+4-diff;
		  ib->biWidth  = xsize;
		  ysize=height;
		  ib->biHeight = ysize;
		  ib->biSizeImage = xsize*ysize*3;
		  ib->biCompression = BI_RGB;
		
		textRect.y=0;
	    textRect.x=0;
	    textRect.xsize=width;
	    textRect.ysize=height;

		hdcl=GetDC(myIcon->myWindow);
		if(!hdcl)goto ErrorOut;

		hbm=CreateDIBSection(hdcl,&ibitmap,DIB_RGB_COLORS,
			&image,NULL,0);
/*
		{
			static FILE *out;
			static long count;

			if(!out)out=fopen("junk1.jnk","w");
				fprintf(out,"CreateDIB %ld xsize %ld ysize %ld Error %ld ThreadID %ld myIcon %lx %lx %lx\n",
				++count,(long)xsize,(long)ysize,(long)GetLastError(),
				(long)GetCurrentThreadId(),(unsigned long)myIcon,(unsigned long)myIcon->hdc,(unsigned long)hbm);
				fflush(out);
		}
*/

		if(!hbm || !image){
			goto ErrorOut;
		}

		hdc=CreateCompatibleDC(myIcon->hdc);
		if(!hdc)goto ErrorOut;

		hbitold=SelectObject(hdc,hbm);
		if(!hbitold)goto ErrorOut;

		oldFont=SelectObject(hdc,myIcon->hfont);
		if(!oldFont)goto ErrorOut;


		SetTextColor(hdc,RGB(255,255,255));
		SetBkColor(hdc,RGB(0,0,0));

		zerol((char *)image,ib->biSizeImage);

	    TextOut(hdc,0,0,out,strlen(out));

		GdiFlush();

		for(j=0;j<height;++j){
			for(i=0;i<width;++i){
				(*imagei)[i+j*width]=image[3*i+j*3*xsize];
			}
		}

		SelectObject(hdc,hbitold);

		SelectObject(hdc,oldFont);

		DeleteObject(hbm);

		DeleteDC(hdc);

		ReleaseDC(myIcon->myWindow,hdcl);

		*xsizei = width;

		*ysizei = height;

	
	return 0;
ErrorOut:
	sprintf(WarningBuff,"FontWrite Error %ld\n",(long)GetLastError());
	// WarningBatch(WarningBuff);
	if(imagei && *imagei){
		cFree((char *)*imagei);
		*imagei=NULL;
	}
	return 1;
}
int FontToRect(char *out,uRect *cRect,unsigned char **image,IconPtr myIcon)
{
	int xsize,ysize,length;
		
	if(!out || !cRect || !image || !myIcon)return 1;

	*image = NULL;
	
	length=(int)strlen(out);
	
	if(length <= 0)return 1;
	
	FontWrite(out,&xsize,&ysize,image,myIcon);

	cRect->xsize=xsize;
	cRect->ysize=ysize;
	
	return 0;
}
int FontToScreen(uRect *cRect,unsigned char *image,int nc,struct screenData *s)
{
	int i,j;
	int x, y;
	
	if(!cRect || !image || !s)return 1;
	
	if(FindB(&x,&y))return 1;
	
	for(j=0;j<cRect->ysize;++j){
	    for(i=0;i<cRect->xsize;++i){
	         if(image[i+j*cRect->xsize]){
	             kpoint(i+x,j+y,nc,s);
	         }
	    }
	}
	
	if(image)cFree((char *)image);

	return 0;
}
int FontStart(IconPtr myIcon,HFONT hfont)
{
	int count;
	int n,k;

	if(fd)return 0;

	count = 1;

	fd=(struct fontData *)cMalloc(count*sizeof(struct fontData),9104);
	if(!fd)return 1;

	FontCount=count;
	for(n=0;n<count;++n){
	    uRect textRect;
		HBITMAP hbm;
		TEXTMETRIC tm;
		HDC hdc;
		SIZE size;
		BITMAPINFOHEADER *ib;
		long xsize,xs,diff,ysize;
		BITMAPINFO ibitmap;


		zerol((char *)&ibitmap,sizeof(BITMAPINFO));

	    fd[n].on=FALSE;

		GetTextMetrics(myIcon->hdc,&tm);

		GetTextExtentPoint32(myIcon->hdc,"0",1,&size);

		ib=&ibitmap.bmiHeader;

		  ib->biSize  = sizeof(BITMAPINFOHEADER);
		  ib->biPlanes  = 1;
		  ib->biBitCount  = 8;
		  xs=129*size.cx;
		  xsize=xs/4;
		  xsize = 4*xsize;
		  diff=xs-xsize;
		  if(diff > 0)
			  xsize=xs+4-diff;
		  ib->biWidth  = xsize;
		  ysize=tm.tmHeight+tm.tmExternalLeading;
		  ib->biHeight = -ysize;
		  ib->biSizeImage = xsize*ysize;
		  ib->biCompression = BI_RGB;
		
	    fd[n].xsize=size.cx;
	    fd[n].ysize=ysize;
	    fd[n].first=0;
	    fd[n].last=126;
	    textRect.y=0;
	    textRect.x=0;
	    textRect.xsize=(fd[n].last-fd[n].first+1)*fd[n].xsize;
	    textRect.ysize=fd[n].ysize;

		hbm=CreateDIBSection(myIcon->hdc,&ibitmap,DIB_RGB_COLORS,
			&fd[n].buff,NULL,0);
				
		if(!hbm || !fd[n].buff){
			sprintf(WarningBuff,"FontStart Error %ld\n",(long)GetLastError());
			WarningBatch(WarningBuff);
			if(fd[n].buff){
				cFree((char *)fd[n].buff);
				fd[n].buff;
			}
			return 1;
		}

		hdc=CreateCompatibleDC(myIcon->hdc);

		SelectObject(hdc,hbm);

		SelectObject(hdc,hfont);

		SetTextColor(hdc,RGB(255,255,255));
		SetBkColor(hdc,RGB(0,0,0));

		zerol((char *)fd[n].buff,ib->biSizeImage);

	    for(k=fd[n].first;k<=fd[n].last;++k){
	        unsigned char out[2];

	        out[0]=k;
	        out[1]=0;
			TextOut(hdc,k*size.cx,0,out,1);
	    }

	    fd[n].xinc=xsize;
	    fd[n].on=TRUE;

	}
	return 0;
}
int uFontSize(int *xsize,int *ysize,int fn)
{
	if(!xsize || !ysize)return 1;

	*xsize=0;
	*ysize=0;

	if(fn < 0 || fn >= FontCount)fn=0;
	if(!fd[fn].on)return 1;
	*xsize=fd[fn].xsize;
	*ysize=fd[fn].ysize;
	return 0;
}
int uDrawPalette(struct paletteDraw *pd,IconPtr myIcon)
{
	long xsizep,ysizep;
	int x,y,xs,ys,xinc,yinc,xp,yp;
	int yflip;
	unsigned char *b;
	struct screenData s;
	char out[256];
	double Small=1.0e-6;

	if(!pd)return 1;
	if(!pd->LabeledPalette)return 0;
	if(uGetPaletteSize(&xsizep,&ysizep,pd,myIcon))return 1;

	zerol((char *)&s,sizeof(struct screenData));

	xs=(int)(pd->left+xsizep-25);
	ys=(int)(pd->top+10);
	xinc=pd->xsize;
	yinc=pd->ysize;
	b=pd->buffer;
	if(!b)return 1;
	s.viewRect.xsize=pd->xsize;
	s.viewRect.ysize=pd->ysize;
	s.buffer=pd->buffer;
	s.ScreenBits=pd->ScreenBits;
	s.palette=&pd->palette[0];

	yflip=pd->ysize;

	if(pd->ScreenBits == RASTER_8 || pd->ScreenBits == RASTER_24){
	    for(y=0;y<256;++y){
	        yp=ys+y;
	        if(yp < 0 || yp > yinc)continue;
			for(x=0;x<15;++x){
				xp=xs+x;
	            if(xp < 0 || xp > xinc)continue;
				kpoint(xp,yflip-yp,255-y,&s);
			}
	    }
	if(pd->sType == 0 || pd->sType == 2){
	    static char *Format=(char *)"%9.2e";
	    double cmin,cmax,dz,dh;
	    double zcon[200];
	    int ncon,n,ysize,ih;
	    int xh,yh;

	    uFontSize(&xh,&yh,pd->paletteFont);

	    ysize=255;
	    if(pd->sType == 0){
	        cmin=pd->dmin;
	        cmax=pd->dmax;
	    }else{
	        cmin=pd->sPmin;
	        cmax=pd->sPmax;
	    }

	    if(cmin >= cmax)cmax=cmin+1.0;

	    sear(cmax,cmin,zcon,&ncon,10);
	    dz=cmax-cmin;
	    for(n=0;n<ncon;++n){
	        if(zcon[n] > (cmax+1.0e-4*dz)){
	            break;
	        }else if(zcon[n] < cmin){
	            continue;
	        }
	        dh=(zcon[n]-cmin)/dz;
	        ih=(int)(ys+ysize-ysize*(dh));
			sprintf((char *)out,Format,zcon[n]);
			MoveB((int)(xs-8-strlen(out)*xh),yflip-ih+yh/2);
			FontOut(out,pd->paletteFont,&s);  
			MoveB(xs-5,yflip-ih);
	        LineB(xs+15,yflip-ih,255,&s); 
	    }
	  }else if(pd->sType == 1){
	    double y,End,Start,Step,yViewMinLog,yViewMaxLog,scale,slide;
	    int iy,xh,yh,ysize,iEnd,iStart,iDiff;

	    uFontSize(&xh,&yh,pd->paletteFont);

	    ysize=255;

	    yViewMaxLog=log10(pd->sPmax);
	    if(yViewMaxLog < 0.0){
	       End=yViewMaxLog*(1-Small);
	    }else{
	       End=yViewMaxLog*(1+Small);
	    }


	    yViewMinLog=log10(pd->sPmin);
	    if(yViewMinLog < 0.0){
	       slide=yViewMinLog*(1+Small);
	    }else{
	       slide=yViewMinLog*(1-Small);
	    }
	    iEnd=(int)End;
	    iStart=(int)(yViewMinLog-.9999);
	    iDiff=iEnd-iStart;
	    if((iDiff) <= 9){
	        Step=1;
	    }else{
	        Step=4;
	    }
	    Start=iStart;
	    scale=ysize/(log10(pd->sPmax)-yViewMinLog);
  	    for(y=Start;y <= End;y += Step){
  	        double yy,yn;
	        if(y > End)break;	   
	        if(y < slide)continue;
			iy=(int)(ys+ysize-scale*(y-yViewMinLog)+.5);
	        sprintf((char *)out,"%g%c",pow(10.,y),0);
			MoveB((int)(xs-8-strlen(out)*xh),yflip-iy+yh/2);
			FontOut(out,pd->paletteFont,&s);  
			MoveB(xs-5,yflip-iy);
	        LineB(xs+15,yflip-iy,255,&s); 
	        if(iDiff > 4)continue;
	        for(yn=2;yn < 10;++yn){
	            yy=y+log10(yn);
	            if(yy > End)break;
				iy=(int)(ys+ysize-scale*(yy-yViewMinLog)+.5);
				MoveB(xs-3,yflip-iy);
	            LineB(xs,yflip-iy,255,&s); 
		}
	    }
	  }
	}else {
		WarningBatch("drawPalette Not PICT_8 or PICT_24\n");
		return 1;
	}
	return 0;
}
int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon)
{
	int xsize,ysize;
	
	if(!xsizep || !ysizep )return 1;
	
	myIcon=myIcon;
	
	*xsizep=0;
	*ysizep=0;
	if(!pd)return 1;
	if(!pd->LabeledPalette)return 0;
	uFontSize(&xsize,&ysize,pd->paletteFont);
	*xsizep = 40+xsize*9;
	*ysizep = 256+10+ysize;
	return 0;
}
int FontOut(char *out,int fn,struct screenData *s)
{
	int xBuff,yBuff;
	int xsize,ysize,n;
	int yflip;
	int xinc,yinc,x,y,xs,ys,xp,yp,dinc;
	unsigned char *b,*data;
	long nb;

	if(!out || !s)return 1;

	if(fn < 0 || fn >= FontCount)fn=0;

	if(uFontSize(&xsize,&ysize,fn))return 1;
	xinc=s->viewRect.xsize;;
	yinc=s->viewRect.ysize;
	b=s->buffer;
	yflip=yinc;
	if(!b)return 1;
	data=(unsigned char *)fd[fn].buff;
	dinc=fd[fn].xinc;
	while(n = *out++){
		if(n < fd[fn].first || n > fd[fn].last)continue;
			if(s->ScreenBits == RASTER_8){
				FindB(&xBuff,&yBuff);
				xs=xBuff;
				ys=yBuff-ysize;
				for(y=0;y<ysize;++y){
					yp=ys+y;
					if(yp < 0 || yp >= yinc)continue;
					for(x=0;x<xsize;++x){
						xp=xs+x;
						if(xp < 0 || xp >= xinc)continue;
						if(data[n*xsize+x+(ysize-y-1)*dinc]){
							b[(long)xp+(yflip-yp)*xinc]=255;
						}else{
							b[(long)xp+(yflip-yp)*xinc]=0;
						}
					}
				}
				xBuff += xsize;
				MoveB(xBuff,yBuff);
	    }else if(s->ScreenBits == RASTER_24){
			FindB(&xBuff,&yBuff);
			xs=xBuff;
			ys=yBuff-ysize;
	        for(y=0;y<ysize;++y){
	            yp=ys+y;
	            if(yp < 0 || yp >= yinc)continue;
				for(x=0;x<xsize;++x){
					xp=xs+x;
	                if(xp < 0 || xp >= xinc)continue;
	                    nb=xp*3+(yflip-yp)*xinc*3;
					if(data[n*xsize+x+(ysize-y-1)*dinc]){
						b[nb]=0;
						b[1+nb]=0;
						b[2+nb]=0;
					}else{
						b[nb]=255;
						b[1+nb]=255;
						b[2+nb]=255;
					}
				}
	        }
			xBuff += xsize;
			MoveB(xBuff,yBuff);
	    }
	}
	return 0;
}
int FontToRect2(char *out,uRect *cRect,unsigned char **image,IconPtr myIcon)
{
	int xsize,ysize,fn,length,n;
	unsigned char *b,*data;
	int xinc,yinc,x,y,xs,ys,xp,yp,dinc;
	int xb,yb;
		
	if(!out || !cRect || !image || !myIcon)return 1;
	*image = NULL;
	
	length=(int)strlen(out);
	
	if(length <= 0)return 1;
	
	
	fn=0;
	
	if(uFontSize(&xsize,&ysize,fn))return 1;
	
	b = (unsigned char *)cMalloc(length*xsize*ysize,8267);
	if(!b)return 1;
	zerol((char *)(b),length*xsize*ysize);
	*image = b;
	xinc=length*xsize;
	cRect->xsize=xinc;
	yinc=ysize;
	cRect->ysize=yinc;
	data=(unsigned char *)fd[fn].buff;
	dinc=fd[fn].xinc;
	
	xb=0;
	yb=0;
	while((n = *out++) != 0){
	    if(n < fd[fn].first || n > fd[fn].last)continue;
		xs=xb;
		ys=yb;
        for(y=0;y<ysize;++y){
            yp=yinc-(ys+y);
            if(yp < 0 || yp >= yinc)continue;
		    for(x=0;x<xsize;++x){
		        xp=xs+x;
	            if(xp < 0 || xp >= xinc)continue;
			    if(data[n*xsize+x+y*dinc]){
			        b[(long)xp+yp*xinc]=255;
			    }else{
			        b[(long)xp+yp*xinc]=0;
			    }
		    }
        }
		xb += xsize;	
	}	
	return 0;
}
