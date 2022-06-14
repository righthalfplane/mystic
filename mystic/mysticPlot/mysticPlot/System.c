#define EXTERN22 extern
#include <stdio.h>
#include <memory.h> 
#include <stdlib.h> 
#include <math.h> 
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include "Xappl.h"
#include "xAux.h"
#include "uFiles.h"
#include "uGraph.h"

/*
int zerol(char *s,unsigned long n);
#define bzero zerol
 */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#ifdef __cplusplus
extern "C" {
#endif

	int DFR8restart(void);
	int DFPrestart(void);
	int DFPgetpal(char *name,char *pal);
	int DFR8setpalette(char *name);
	int DFR8getimage(char *name,char *image,int ixmax,int iymax,char *pal);
	int DFR8getdims(char *name ,int *ixmax,int *iymax,int *hasPalette);
	int DFR8lastref(void);
	int DFANgetlabel(char *file,unsigned int type,unsigned int tag,char *name,int length);
	int DFANputlabel(char *file,unsigned int type,unsigned int tag,char *name);
	int DFR8putimage(char *name,char *buffout,int ixmax,int iymax,int type);
	int DFR8addimage(char *name,char *buffout,int ixmax,int iymax,int type);
#ifndef FD_SET
#include <sys/select.h>
#else
        int select (int width, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
#ifdef sgi
	int gettimeofday(struct timeval *tp,...);
#else
#endif
#endif

#ifdef __cplusplus
}
#endif

#if defined(__cplusplus) || defined(c_plusplus)
#define class c_class
#endif

#ifndef FIONREAD
#include <sys/filio.h>
#endif

int GetTime(long *Seconds,long *milliseconds);

int SetTime(struct timeval *curTime,long milliseconds);

int TimeUp(struct timeval *LastTime);

int SetWindowName(IconPtr myIcon,char *name);

int CheckSocket(int serverSocket,int *count);

int Sleep2(int ms);

struct timeval BellTime;

void Bell(int tag);

int SearchVisualList(int image_depth,int  visual_class, Visual **visual);

int WriteHDFToScreen(struct DisplayF *d,unsigned char *data);

int WriteToScreen(struct SetFrameData *s,struct DisplayF *d,unsigned char *data,int type);

int Write24ToScreen(unsigned char *bin,unsigned char *bout,int xsize,int ysize,XColor *colors);

int do24to8(unsigned char *pal,unsigned char *pin,unsigned char *pout,long xsize,long ysize);

int getXcolors(char *pal,XColor *colors);

int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon);
int FontToRect(char *out,uRect *cRect,unsigned char **image,IconPtr myIcon);
int getColorFromRGB(unsigned long *c,int r,int g,int b);
int uCStringWidth(char *name,IconPtr myIcon);

char *ConvertPath(char *namein,int flag);

FILE *getPIPE();

FILE *getPIPE()
{
	return NULL;
}

char *ConvertPath(char *namein,int flag)
{
	if(!namein)return NULL;
	
	if(flag){
	    cFree((char *)namein);
	}
	return NULL;
}
void Bell(int tag)
{
	if(!TimeUp(&BellTime))return;
	//tag=tag;
	 XBell(xg.mydisplay, 50);   
	SetTime(&BellTime,1000);	
}

int GetTime(long *Seconds,long *milliseconds)
{
	struct timeval curTime;
	
	if(!Seconds || !milliseconds)return 1;

	gettimeofday(&curTime,(struct timezone *) NULL);
	*Seconds=curTime.tv_sec;
	*milliseconds=curTime.tv_usec/1000;
	return 0;
}
int SetTime(struct timeval *curTime,long milliseconds)
{

	if(!curTime)return 1;
	
	gettimeofday(curTime,(struct timezone *) NULL);
	curTime->tv_sec += milliseconds/1000;
	curTime->tv_usec += (milliseconds%1000)*1000;
	if (curTime->tv_usec > 1000000) {
            curTime->tv_usec -= 1000000;
            curTime->tv_sec += 1;
	}
    return 0;
}
int TimeUp(struct timeval *LastTime)
{
	struct timeval curTime;

	if(!LastTime)return 1;
	
       (void) gettimeofday(&curTime, (struct timezone *) NULL);
        if ((LastTime->tv_sec < curTime.tv_sec)
                || ((LastTime->tv_sec == curTime.tv_sec)
                &&  (LastTime->tv_usec < curTime.tv_usec))) {
                return 1;
        }
        return 0;
}

int Sleep2(int ms)
{
    usleep(ms*1000);

/*
    static struct timeval delay;

#define SELECT_MASK fd_set

    delay.tv_sec = ms/1000;
    delay.tv_usec = (ms%1000)*1000;
    (void) select(0, (SELECT_MASK *) 0, (SELECT_MASK *) 0,
            (SELECT_MASK *) 0, &delay);
*/
	return 0;
}

int CheckSocket(int serverSocket,int *count)
{
       static struct timeval   timeout = {0L, 0L};
       fd_set filedes;

		if(!count)return 1;
		
        FD_ZERO(&filedes);
        FD_SET(serverSocket, &filedes);

        if (select(32, &filedes, (fd_set *)NULL, (fd_set *)NULL, &timeout))  {
                ioctl(serverSocket, FIONREAD, count);
                return TRUE;
        }
		*count = 0;
        return FALSE;
}
int SearchVisualList(int image_depth,int  visual_class, Visual **visual)
{
    XVisualInfo template_visual, *visual_list, *vl;
    int i, n_visuals;
    
    if(!visual)return 1;

    template_visual.screen =  DefaultScreen(xg.mydisplay);
    vl = visual_list = XGetVisualInfo(xg.mydisplay, VisualScreenMask,
        &template_visual, &n_visuals);

    if (n_visuals == 0) {
        fprintf(stderr, "visual list not available\n");
        return 1;
    }
/*
    printf("request image_depth %d visual_class %d\n\n",image_depth,visual_class);
    for (i = 0; i < n_visuals; vl++, i++) {
        printf("class %d depth %d colormap_size %d bits_per_rgb %d ",
                vl->class,vl->depth,vl->colormap_size,vl->bits_per_rgb);
        printf("r %x g %x b %x id %d\n",
                vl->red_mask,vl->green_mask,vl->blue_mask,vl->visualid);
    }
*/
    vl = visual_list;
    for (i = 0; i < n_visuals; vl++, i++) {   
	if(vl->class != visual_class)continue;
        if (vl->depth == image_depth){
            *visual = vl->visual;
            XFree((char *) visual_list);
            return 0;
        }
    }

    XFree((char *) visual_list);
    return 1;
}
int WriteToScreen(struct SetFrameData *s,struct DisplayF *d,unsigned char *data,int type)
{

	if(!s || !d || !data)return 1;

	if(type == HDF_FILES){
	        return WriteHDFToScreen(d,data);
	}
	return 0;
}
int Write24ToScreen(unsigned char *bini,unsigned char *bout,int xsize,int ysize,XColor *colors)
{
	register unsigned char *ps,*bin;
	unsigned char pal[3*256];
	int i,j;
	
	if(!bini || !bout)return 1;
	
	bin=bini;

	if(xg.vBits == 8){
	    if(!colors)return 1;
	    do24to8(pal,bin,bout,xsize,ysize);
	    if(!getXcolors((char *)pal,colors)){
	        if(xg.vBits == 8)XStoreColors(xg.mydisplay,xg.xColormapDefault,colors,256);
	    }
	}else if(xg.vBits == 15){
	    unsigned short c0,c1,c2;
	    unsigned short sr;
	    if(xg.RedIsLow){
	        if(xg.ScreenIsHigh){
	            for(j=0;j<ysize;++j){
					ps=(unsigned char *)(bout+j*xsize*2);
	                for(i=0;i<xsize;++i){
			            c0=(*bin++ >> 3) & 0x1f;
			            c1=(*bin++ >> 3) & 0x1f;
			            c2=(*bin++ >> 3) & 0x1f;
				    	sr = (c0) | (c1 << 5) | (c2 << 10);
				    	*ps++ = ((sr >> 8) & 0xff);
				    	*ps++ = (sr & 0xff);
	                }
	            }
	        }else{
	            for(j=0;j<ysize;++j){
					ps=(unsigned char *)(bout+j*xsize*2);
	                for(i=0;i<xsize;++i){
			            c0=(*bin++ >> 3) & 0x1f;
			            c1=(*bin++ >> 3) & 0x1f;
			            c2=(*bin++ >> 3) & 0x1f;
				    	sr = (c0) | (c1 << 5) | (c2 << 10);
				    	*ps++ = (sr & 0xff);
				    	*ps++ = ((sr >> 8) & 0xff);
	                }
	            }
	        }
	    }else{
	        if(xg.ScreenIsHigh){
	            for(j=0;j<ysize;++j){
					ps=(unsigned char *)(bout+j*xsize*2);
	                for(i=0;i<xsize;++i){
			            c0=(*bin++ >> 3) & 0x1f;
			            c1=(*bin++ >> 3) & 0x1f;
			            c2=(*bin++ >> 3) & 0x1f;
				    	sr = (c0 << 10) | (c1 << 5) | (c2);
				    	*ps++ = ((sr >> 8) & 0xff);
				    	*ps++ = (sr & 0xff);
	                }
	            }
	        }else{
	            for(j=0;j<ysize;++j){
					ps=(unsigned char *)(bout+j*xsize*2);
	                for(i=0;i<xsize;++i){
			            c0=(*bin++ >> 3) & 0x1f;
			            c1=(*bin++ >> 3) & 0x1f;
			            c2=(*bin++ >> 3) & 0x1f;
				    	sr = (c0 << 10) | (c1 << 5) | (c2);
				    	*ps++ = (sr & 0xff);
				    	*ps++ = ((sr >> 8) & 0xff);
	                }
	            }
	        }
	    }
	}else if(xg.vBits == 16){
	    unsigned short c0,c1,c2;
	    unsigned short sr;
	    if(xg.RedIsLow){
	        if(xg.ScreenIsHigh){
	            for(j=0;j<ysize;++j){
	                for(i=0;i<xsize;++i){
			            c0=(*bin++ >> 3) & 0x1f;
			            c1=(*bin++ >> 2) & 0x3f;
			            c2=(*bin++ >> 3) & 0x1f;
				    	ps=(unsigned char *)bout;
				    	sr = (c0) | (c1 << 5) | (c2 << 11);
				    	*ps++ = (sr >> 8) & 0xff;
				    	*ps++ = sr & 0xff;
			            bout += 2;
	                }
	            }
	        }else{
	            for(j=0;j<ysize;++j){
	                for(i=0;i<xsize;++i){
			            c0=(*bin++ >> 3) & 0x1f;
			            c1=(*bin++ >> 2) & 0x3f;
			            c2=(*bin++ >> 3) & 0x1f;
				    	ps=(unsigned char *)bout;
				    	sr = (c0) | (c1 << 5) | (c2 << 11);
				    	*ps++ = sr & 0xff;
				    	*ps++ = (sr >> 8) & 0xff;
			            bout += 2;
	                }
	            }
	        }
	    }else{
	        if(xg.ScreenIsHigh){
	            for(j=0;j<ysize;++j){
	                for(i=0;i<xsize;++i){
			            c0=(*bin++ >> 3) & 0x1f;
			            c1=(*bin++ >> 2) & 0x3f;
			            c2=(*bin++ >> 3) & 0x1f;
				    	ps=(unsigned char *)bout;
				    	sr = (c0 << 11) | (c1 << 5) | (c2);
				   	 	*ps++ = (sr >> 8) & 0xff;
				    	*ps++ = sr & 0xff;
			            bout += 2;
	                }
	            }
	        }else{
	            for(j=0;j<ysize;++j){
	                for(i=0;i<xsize;++i){
			            c0=(*bin++ >> 3) & 0x1f;
			            c1=(*bin++ >> 2) & 0x3f;
			            c2=(*bin++ >> 3) & 0x1f;
				    	ps=(unsigned char *)bout;
				    	sr = (c0 << 11) | (c1 << 5) | (c2);
				    	*ps++ = sr & 0xff;
				    	*ps++ = (sr >> 8) & 0xff;
			            bout += 2;
	                }
	            }
	        }
	    }
	}else if(xg.vBits == 24){
	    unsigned char c0,c1,c2;
            if(xg.RedIsLow){
               if(xg.ScreenIsHigh){
	            for(j=0;j<ysize;++j){
		        	ps=(unsigned char *)(bout+j*4*xsize);
	                for(i=0;i<xsize;++i){
			            c0 = *bin++;
			            c1 = *bin++;
			            c2 = *bin++;
				    	*ps++ = 0;
				    	*ps++ = c2;
				    	*ps++ = c1;
				    	*ps++ = c0;
	                }
	            }
               }else{
	            for(j=0;j<ysize;++j){
		        	ps=(unsigned char *)(bout+j*4*xsize);
	                for(i=0;i<xsize;++i){
			            c0 = *bin++;
			            c1 = *bin++;
			            c2 = *bin++;
				    	*ps++ = c0;
				    	*ps++ = c1;
				    	*ps++ = c2;
				    	*ps++ = 0;
	                }
	            }
               }
            }else{
              if(xg.ScreenIsHigh){
	            for(j=0;j<ysize;++j){
		        	ps=(unsigned char *)(bout+j*4*xsize);
	                for(i=0;i<xsize;++i){
				    	*ps++ = 0;
				    	*ps++ = *bin++;
				    	*ps++ = *bin++;
				    	*ps++ = *bin++;
	                }
	            }
               }else{
	            for(j=0;j<ysize;++j){
		        	ps=(unsigned char *)(bout+j*4*xsize);
	                for(i=0;i<xsize;++i){
			            c0 = *bin++;
			            c1 = *bin++;
			            c2 = *bin++;
				    	*ps++ = c2;
				    	*ps++ = c1;
				    	*ps++ = c0;
				    	*ps++ = 0;
	                }
	            }
               }
            }
	}
	return 0;
	
}
int WriteHDFToScreen(struct DisplayF *d,unsigned char *data)
{
	unsigned char *buffer,*pal;
	int xwindow,xinc,ixx;
	int ywindow,iyy;
	long iyinc;
	unsigned char cc0,cc1,cc2;
	unsigned int nc;

	if(!d || !data)return 1;
	
	pal=&d->pd.palette[0];

	xwindow=d->imageRect.xsize;

	xinc=d->xsize;

	xwindow=xinc;

	ywindow=d->ysize;

	buffer=d->buffer;
	if(!buffer)return 1;

	if(xg.vBits == 8){
	    for(iyy=0;iyy<ywindow;++iyy){
	        iyinc=iyy*xinc;
	        for(ixx=0;ixx<xwindow;++ixx){
	            buffer[ixx+iyinc] = data[ixx+iyinc];
	        }
	    }
	}else if(xg.vBits == 15){
	    unsigned char pal16[256*3];
	    unsigned short c0,c1,c2;
	    unsigned char *ps;
	    unsigned short sr;

	    for(nc=0;nc<256;++nc){
	        pal16[nc*3]=(pal[nc*3]   >> 3) & 0x1f;
	        pal16[nc*3+1]=(pal[nc*3+1] >> 3) & 0x1f;
	        pal16[nc*3+2]=(pal[nc*3+2] >> 3) & 0x1f;
	    }

	    if(xg.RedIsLow){
	        if(xg.ScreenIsHigh){
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*2);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		c0=pal16[nc*3];
			    		c1=pal16[nc*3+1];
			    		c2=pal16[nc*3+2];
			    		sr = (c0) | (c1 << 5) | (c2 << 10);
			    		*ps++ = ((sr >> 8) & 0xff);
			    		*ps++ = (sr & 0xff);
	                }
	            }
	        }else{
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*2);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		c0=pal16[nc*3];
			    		c1=pal16[nc*3+1];
			    		c2=pal16[nc*3+2];
			    		sr = (c0) | (c1 << 5) | (c2 << 10);
			    		*ps++ = (sr & 0xff);
			    		*ps++ = ((sr >> 8) & 0xff);
	                }
	            }
	        }
	    }else{
	        if(xg.ScreenIsHigh){
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*2);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		c0=pal16[nc*3];
			    		c1=pal16[nc*3+1];
			    		c2=pal16[nc*3+2];
			    		sr = (c0 << 10) | (c1 << 5) | (c2);
			    		*ps++ = ((sr >> 8) & 0xff);
			    		*ps++ = (sr & 0xff);
	                }
	            }
	        }else{
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*2);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		c0=pal16[nc*3];
			    		c1=pal16[nc*3+1];
			    		c2=pal16[nc*3+2];
			    		sr = (c0 << 10) | (c1 << 5) | (c2);
			    		*ps++ = (sr & 0xff);
			    		*ps++ = ((sr >> 8) & 0xff);
	                }
	            }
	        }
	    }
	}else if(xg.vBits == 16){
	    unsigned char pal16[256*3];
	    unsigned short c0,c1,c2;
	    unsigned char *ps;
	    unsigned short sr;

	    for(nc=0;nc<256;++nc){
	        pal16[nc*3]=(pal[nc*3]   >> 3) & 0x1f;
	        pal16[nc*3+1]=(pal[nc*3+1] >> 2) & 0x3f;
	        pal16[nc*3+2]=(pal[nc*3+2] >> 3) & 0x1f;
	    }

	    if(xg.RedIsLow){
	        if(xg.ScreenIsHigh){
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*2);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		c0=pal16[nc*3];
			    		c1=pal16[nc*3+1];
			    		c2=pal16[nc*3+2];
			    		sr = (c0) | (c1 << 5) | (c2 << 11);
			   	 		*ps++ = ((sr >> 8) & 0xff);
			    		*ps++ = (sr & 0xff);
	                }
	            }
	        }else{
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*2);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		c0=pal16[nc*3];
			    		c1=pal16[nc*3+1];
			    		c2=pal16[nc*3+2];
			    		sr = (c0) | (c1 << 5) | (c2 << 11);
			    		*ps++ = (sr & 0xff);
			    		*ps++ = ((sr >> 8) & 0xff);
	                }
	            }
	        }
	    }else{
	        if(xg.ScreenIsHigh){
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*2);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		c0=pal16[nc*3];
			    		c1=pal16[nc*3+1];
			    		c2=pal16[nc*3+2];
			    		sr = (c0 << 11) | (c1 << 5) | (c2);
			    		*ps++ = ((sr >> 8) & 0xff);
			    		*ps++ = (sr & 0xff);				    				    		
	                }
	            }
	        }else{
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*2);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		c0=pal16[nc*3];
			    		c1=pal16[nc*3+1];
			    		c2=pal16[nc*3+2];
			    		sr = (c0 << 11) | (c1 << 5) | (c2);
			    		*ps++ = (sr & 0xff);
			    		*ps++ = ((sr >> 8) & 0xff);
	                }
	            }
	        }
	    }
	}else if(xg.vBits == 24){
	    unsigned char *ps;
            if(xg.RedIsLow){
               if(xg.ScreenIsHigh){
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*4);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		cc0=pal[nc*3];
			    		cc1=pal[nc*3+1];
			    		cc2=pal[nc*3+2];
			    		*ps++ = 0;
			    		*ps++ = cc2;
			    		*ps++ = cc1;
			    		*ps++ = cc0;
	                }
	            }                 
               }else{
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*4);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		cc0=pal[nc*3];
			    		cc1=pal[nc*3+1];
			    		cc2=pal[nc*3+2];
			    		*ps++ = cc0;
			    		*ps++ = cc1;
			    		*ps++ = cc2;
			    		*ps++ = 0;
	                }
	            }                 
               }
            }else{
               if(xg.ScreenIsHigh){
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*4);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);

			    		cc0=pal[nc*3];
			    		cc1=pal[nc*3+1];
			    		cc2=pal[nc*3+2];
			    		
			    		*ps++ = 0;
			    		*ps++ = cc0;
			    		*ps++ = cc1;
			    		*ps++ = cc2;
			    		
	                }
	            }                 
               }else{
	            for(iyy=0;iyy<ywindow;++iyy){
	                iyinc=iyy*xinc;
		        	ps=(unsigned char *)(buffer+iyinc*4);
	                for(ixx=0;ixx<xwindow;++ixx){
	                    nc=(unsigned int )(data[ixx+iyinc]);
			    		cc0=pal[nc*3];
			    		cc1=pal[nc*3+1];
			    		cc2=pal[nc*3+2];
			    		*ps++ = cc2;
			    		*ps++ = cc1;
			    		*ps++ = cc0;
			    		*ps++ = 0;
	                }
	            }                 
               }
            }
	}
	return 0;
}

struct fontData{
    int on;
    int xsize;
    int ysize;
    int first;
    int last;
    int xinc;
    XImage *xi;
};

struct fontData *fd;
static int FontCount;

int FontEnd(void);
int FontStart(GC gc);
int getColors(unsigned long *c);
int FontOut(char *out,int fn,struct screenData *s);
int FontSize(int *xsize,int *ysize,int fn);
int LineB(int x,int y,int nc,struct screenData *s);
int kpoint(int x,int y,int nc,struct screenData *s);
int MoveB(int x,int y);
int FindB(int *x,int *y);
int sear(double cmax,double cmin,double *zcon,int *ncon,int nmax);

int FontEnd(void)
{
	int n;
	if(fd){
		for(n=0;n<FontCount;++n)XDestroyImage(fd[n].xi);
	    cFree((char *)fd);
	}
	return 0;
}
int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon)
{
	int xsize,ysize;
	
	if(!xsizep || !ysizep )return 1;
	
	//myIcon=myIcon;
	
	xsize=0;
	ysize=0;
	*xsizep=0;
	*ysizep=0;
	if(!pd)return 1;
	if(!pd->LabeledPalette)return 0;
	FontSize(&xsize,&ysize,pd->paletteFont);
	*xsizep = 40+xsize*9;
	*ysizep = 256+10+ysize;
	return 0;
}
int FontSize(int *xsize,int *ysize,int fn)
{
	if(fn < 0 || fn >= FontCount || !fd[fn].on)return 1;
	if(!xsize || !ysize)return 1;
	*xsize=fd[fn].xsize;
	*ysize=fd[fn].ysize;
	return 0;
}
int FontToRect(char *out,uRect *cRect,unsigned char **imagei,IconPtr myIcon)
{
	XImage *dd;
	Pixmap pixmap;
	int length;
	int height,width;
	uFontInfo Info;
	unsigned char *image;
	XRectangle r;
		
	if(!out || !cRect || !imagei || !myIcon)return 1;
	*imagei = NULL;
	
	length=(int)strlen(out);
	
	if(length <= 0)return 1;
	
	uGetFontInfo(myIcon,&Info);
	


	height=Info.LineHeight;

	width=uCStringWidth(out,myIcon);
	
	cRect->xsize=width;
	cRect->ysize=height;
	

	image=cMalloc(height*width,9999);
	if(!image)return 1;
	zerol((char *)image,height*width);
	*imagei=image;

    pixmap = XCreatePixmap(xg.mydisplay,RootWindow(xg.mydisplay, xg.myscreen),
                       width, height,xg.vBits);
    if(pixmap == (Pixmap)NULL)return 1;
    
 	r.x=0;
	r.y=0;
	r.width=20000;
	r.height=20000;



   	XCopyGC(xg.mydisplay,myIcon->gc,GCFont,myIcon->gcFont);


	XSetClipRectangles(xg.mydisplay,myIcon->gcFont,0,0,&r,1,0);
	
   {
		unsigned long c[10];

		getColors(c);


		if(xg.vBits == 8){	
		    XSetForeground (xg.mydisplay,myIcon->gcFont,1);
		    XSetBackground (xg.mydisplay,myIcon->gcFont,0);
		}else{
		    XSetForeground (xg.mydisplay,myIcon->gcFont,c[6]);
		    XSetBackground (xg.mydisplay,myIcon->gcFont,c[7]);
		}
    }

    MoveTo(0,height-Info.Descent);
	DrawString((unsigned char *)out,pixmap,myIcon->gcFont);

    dd=XGetImage(xg.mydisplay,pixmap,0,0,
           width,height,AllPlanes,ZPixmap);
	if(!dd)return 1;
	
	
	if(xg.vBits == 15 || xg.vBits == 16){	
        int i,j,mm,off;
		for(j=0;j<height;++j){
		    mm=0;
		    off=j*dd->bytes_per_line;
		    for(i=0;i<dd->bytes_per_line;i += 2,++mm){
		       if(mm >= width)break;
		       image[mm+(height-1-j)*width]=dd->data[i+off];
		    }
		}
    }else if(xg.vBits == 24){	
        int i,j,mm,off;
		for(j=0;j<height;++j){
		    mm=0;
		    off=j*dd->bytes_per_line;
		    for(i=0;i<dd->bytes_per_line;i += 4,++mm){
		       if(mm >= width)break;
		       image[mm+(height-1-j)*width]=dd->data[i+off+1];
		    }
		}

    }



    XFreePixmap(xg.mydisplay,pixmap);
    
    XDestroyImage(dd);
	
	return 0;
}
int FontOut(char *out,int fn,struct screenData *s)
{
	int xBuff,yBuff;
	int xsize,ysize,n;
	int xinc,yinc,x,y,xs,ys,xp,yp,dinc;
	unsigned char *b,*data;
	long nb;

	if(!out || !s)return 1;
	if(FontSize(&xsize,&ysize,fn))return 1;
	xinc=s->viewRect.xsize;
	yinc=s->viewRect.ysize;
	b=s->buffer;
	if(!b)return 1;
	data=(unsigned char *)fd[fn].xi->data;
	dinc=fd[fn].xinc;
	while((n = *out++) != 0){
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
			    if(data[n*xsize+x+y*dinc]){
			        b[(long)xp+yp*xinc]=255;
			    }else{
			        b[(long)xp+yp*xinc]=0;
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
	                    nb=xp*3+yp*xinc*3;
			    if(data[n*xsize+x+y*dinc]){
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
int getColors(unsigned long *c)
{
	unsigned long c0,c1,c2;
	int n,iflag;

	if(!c || !xg.xVisual)return 1;
/*
	printf("red_mask %d green_mask %d blue_mask %d\n",
	       xg.xVisual->red_mask,xg.xVisual->green_mask,xg.xVisual->blue_mask);
*/
	if(xg.vBits == 24){
	    if(xg.xVisual->red_mask & 0xff){
	       iflag = 1;
	    }else{
	       iflag = 0;
	    }
	    for(n=0;n<4;++n){
			c[n]=0;
			if(iflag){
			    c0=(xg.colors[n].red >> 8) & 0xff;
			    c1=(xg.colors[n].green >> 8) & 0xff;
			    c2=(xg.colors[n].blue >> 8) & 0xff;
			}else{
			    c2=(xg.colors[n].red >> 8) & 0xff;
			    c1=(xg.colors[n].green >> 8) & 0xff;
			    c0=(xg.colors[n].blue >> 8) & 0xff;
			}
			c[n] = (c0) | (c1 << 8) | (c2 << 16);
	    }
	    c[4]=0;
	    if(iflag){
			c0=(xg.colors[2].red >> 8) & 0xff;
			c1=(xg.colors[2].green >> 8) & 0xff;
			c2=(xg.colors[2].blue >> 8) & 0xff;
	    }else{
			c2=(xg.colors[2].red >> 8) & 0xff;
			c1=(xg.colors[2].green >> 8) & 0xff;
			c0=(xg.colors[2].blue >> 8) & 0xff;
	    }
	    c[5] = (c0) | (c1 << 8) | (c2 << 16);
	    c0 = 255;
	    c1 = 255;
	    c2 = 255;
	    c[6] = (c0) | (c1 << 8) | (c2 << 16);
	    c[7]=0;
	    c0 = 250;
	    c1 = 250;
	    c2 = 250;
	    c[8] = (c0) | (c1 << 8) | (c2 << 16);
	}else if (xg.vBits == 16){
	    if(xg.xVisual->red_mask & 0x1f){
	       iflag = 1;
	    }else{
	       iflag = 0;
	    }
	    for(n=0;n<4;++n){
			c[n]=0;
			if(iflag){
			    c0=(xg.colors[n].red >> 11) & 0x1f;
			    c1=(xg.colors[n].green >> 10) & 0x3f;
			    c2=(xg.colors[n].blue >> 11) & 0x1f;
			}else{
			    c2=(xg.colors[n].red >> 11) & 0x1f;
			    c1=(xg.colors[n].green >> 10) & 0x3f;
			    c0=(xg.colors[n].blue >> 11) & 0x1f;
			}
			c[n] = (c0) | (c1 << 5) | (c2 << 11);
	    }
	    c[4]=0;

	    if(iflag){
			c0=(xg.colors[2].red >> 11) & 0x1f;
			c1=(xg.colors[2].green >> 10) & 0x3f;
			c2=(xg.colors[2].blue >> 11) & 0x1f;
	    }else{
			c2=(xg.colors[2].red >> 11) & 0x1f;
			c1=(xg.colors[2].green >> 10) & 0x3f;
			c0=(xg.colors[2].blue >> 11) & 0x1f;
	    }
	    c[5] = (c0) | (c1 << 5) | (c2 << 11);

	    c0 = 255;
	    c1 = 255;
	    c2 = 255;
	    c[6] = (c0) | (c1 << 5) | (c2 << 11);

	    c[7]=0;
	    
	    c0 = 250;
	    c1 = 250;
	    c2 = 250;
	    c[8] = (c0) | (c1 << 5) | (c2 << 11);
	}else if (xg.vBits == 15){
	    if(xg.xVisual->red_mask & 0x1f){
	       iflag = 1;
	    }else{
	       iflag = 0;
	    }
	    for(n=0;n<4;++n){
			c[n]=0;
			if(iflag){
			    c0=(xg.colors[n].red >> 11) & 0x1f;
			    c1=(xg.colors[n].green >> 11) & 0x1f;
			    c2=(xg.colors[n].blue >> 11) & 0x1f;
			}else{
			    c2=(xg.colors[n].red >> 11) & 0x1f;
			    c1=(xg.colors[n].green >> 11) & 0x1f;
			    c0=(xg.colors[n].blue >> 11) & 0x1f;
			}
			c[n] = (c0) | (c1 << 5) | (c2 << 10);
	    }
	    c[4]=0;

	    if(iflag){
			c0=(xg.colors[2].red >> 11) & 0x1f;
			c1=(xg.colors[2].green >> 11) & 0x1f;
			c2=(xg.colors[2].blue >> 11) & 0x1f;
	    }else{
			c2=(xg.colors[2].red >> 11) & 0x1f;
			c1=(xg.colors[2].green >> 11) & 0x1f;
			c0=(xg.colors[2].blue >> 11) & 0x1f;
	    }
	    c[5] = (c0) | (c1 << 5) | (c2 << 10);

	    c0 = 255;
	    c1 = 255;
	    c2 = 255;
	    c[6] = (c0) | (c1 << 5) | (c2 << 10);

	    c[7]=0;
	    c0 = 250;
	    c1 = 250;
	    c2 = 250;
	    c[8] = (c0) | (c1 << 5) | (c2 << 10);
	    
	}else{
	    for(n=0;n<4;++n){
	       c[n]=xg.c[n];
	    }
	    c[4]=xg.myforeground;
	    c[5]=xg.c[2];
	    c[6]=0;
	    c[7]=255;
	    c[8]=250;
	}


	return 0;
}
int getColorFromRGB(unsigned long *c,int r,int g,int b)
{
	unsigned long c0,c1,c2;
	int iflag;

	if(!c || !xg.xVisual)return 1;

	if(xg.vBits == 24){
	    if(xg.xVisual->red_mask & 0xff){
	       iflag = 1;
	    }else{
	       iflag = 0;
	    }
	    if(iflag){
			c0=(r) & 0xff;
			c1=(g) & 0xff;
			c2=(b) & 0xff;
	    }else{
			c2=(r) & 0xff;
			c1=(g) & 0xff;
			c0=(b) & 0xff;
	    }
	    c[0] = (c0) | (c1 << 8) | (c2 << 16);
	}else if (xg.vBits == 16){
	    if(xg.xVisual->red_mask & 0x1f){
	       iflag = 1;
	    }else{
	       iflag = 0;
	    }

	    if(iflag){
			c0=(r >> 3) & 0x1f;
			c1=(g >> 2) & 0x3f;
			c2=(b >> 3) & 0x1f;
	    }else{
			c2=(r >> 3) & 0x1f;
			c1=(b >> 2) & 0x3f;
			c0=(b >> 3) & 0x1f;
	    }
	    c[0] = (c0) | (c1 << 5) | (c2 << 11);

	}else if (xg.vBits == 15){
	    if(xg.xVisual->red_mask & 0x1f){
	       iflag = 1;
	    }else{
	       iflag = 0;
	    }
	    if(iflag){
			c0=(r >> 3) & 0x1f;
			c1=(g >> 3) & 0x1f;
			c2=(b >> 3) & 0x1f;
	    }else{
			c2=(r >> 3) & 0x1f;
			c1=(g >> 3) & 0x1f;
			c0=(b >> 3) & 0x1f;
	    }
	    c[0] = (c0) | (c1 << 5) | (c2 << 10);

	}else{
	    c[0]=r;
	}


	return 0;
}
int FontStart(GC gc)
{
	static Font *fl[]={&(xg.font12),&(xg.font13),
	                &(xg.font14),&(xg.font16),
	                &(xg.font24)};
	static int count=sizeof(fl)/sizeof(Font *);
	static  XFontStruct **info[]={&(xg.font12info),&(xg.font13info),
	                &(xg.font14info),&(xg.font16info),
	                &(xg.font24info)};

	XFontStruct *f;
	XCharStruct *c;
	Pixmap pixmap;
	int n,k;

	if(!gc)return 1;
	if(fd)return 0;
	fd=(struct fontData *)cMalloc(count*sizeof(struct fontData),8657);
	if(!fd)return 1;
	FontCount=count;
	for(n=0;n<count;++n){
	    uRect textRect;

	    /* if(n != 2)continue; */

	    fd[n].on=FALSE;

	    XSetFont(xg.mydisplay,gc,*fl[n]); 
/*
	    f=XQueryFont(xg.mydisplay,XGContextFromGC(gc));
*/
	    f = *info[n];
	    if(!f){
	        fprintf(stderr,"StartFonts font %d XQueryFont failed\n",n);

	        continue;
	    }
	    if(f->min_byte1 || f->max_byte1){
	        fprintf(stderr,"StartFonts font %d two byte font error min_byte1 %d max_byte1 %d\n",
	                n,f->min_byte1,f->max_byte1);
	        continue;
	    }
	    c=&(f->max_bounds);
	    fd[n].xsize=c->width;
	    fd[n].ysize=c->ascent+c->descent;
	    fd[n].first=f->min_char_or_byte2;
	    fd[n].last=f->max_char_or_byte2;
	    textRect.y=0;
	    textRect.x=0;
	    textRect.xsize=(fd[n].last-fd[n].first+1)*fd[n].xsize;
	    textRect.ysize=fd[n].ysize;
	    pixmap = XCreatePixmap(xg.mydisplay,RootWindow(xg.mydisplay, xg.myscreen),
	                       textRect.xsize, textRect.ysize,xg.vBits);
	    if(pixmap == (Pixmap)NULL){
	        fprintf(stderr,"StartFonts font %d NULL pixmap\n",n);
	        continue;
	    }
	    {
			unsigned long c[10];

			getColors(c);


			if(xg.vBits == 8){	
			    XSetForeground (xg.mydisplay,gc,1);
			    XSetBackground (xg.mydisplay,gc,0);
			}else{
			    XSetForeground (xg.mydisplay,gc,c[6]);
			    XSetBackground (xg.mydisplay,gc,c[7]);
			}
	    }
	    for(k=fd[n].first;k<=fd[n].last;++k){
	        unsigned char out[2];

	        MoveTo(k*c->width,c->ascent);
	        out[0]=k;
	        out[1]=0;
			DrawString((unsigned char *)out,pixmap,gc);
	    }

	    fd[n].xi=XGetImage(xg.mydisplay,pixmap,0,0,
	           textRect.xsize,textRect.ysize,AllPlanes,ZPixmap);


	    XFreePixmap(xg.mydisplay,pixmap);
	    if(fd[n].xi == NULL){
	        fprintf(stderr,"StartFonts font %d NULL Image\n",n);
	        continue;
	    }
	    fd[n].xinc=fd[n].xi->bytes_per_line;
	    fd[n].on=TRUE;
	    if(xg.vBits == 15 || xg.vBits == 16){	
	        int i,j,mm,off;
			for(j=0;j<fd[n].ysize;++j){
			    mm=0;
			    off=j*fd[n].xinc;
			    for(i=0;i<fd[n].xinc;i += 2,++mm){
			       fd[n].xi->data[mm+off]=fd[n].xi->data[i+off];
			    }
			}
	    }else if(xg.vBits == 24){	
	        int i,j,mm,off;
			for(j=0;j<fd[n].ysize;++j){
			    mm=0;
			    off=j*fd[n].xinc;
			    for(i=0;i<fd[n].xinc;i += 4,++mm){
			       fd[n].xi->data[mm+off]=fd[n].xi->data[i+off+1];
			    }
			}

	    }
	}
	return 0;
}
/*
int FontToRect(char *out,uRect *cRect,unsigned char **image,IconPtr myIcon)
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
	
	if(FontSize(&xsize,&ysize,fn))return 1;
	
	b = (unsigned char *)cMalloc(length*xsize*ysize,8267);
	if(!b)return 1;
	zerol((char *)(b),length*xsize*ysize);
	*image = b;
	xinc=length*xsize;
	cRect->xsize=xinc;
	yinc=ysize;
	cRect->ysize=yinc;
	data=(unsigned char *)fd[fn].xi->data;
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
*/
