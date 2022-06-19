#define EXTERN22 extern
#include "Xappl.h"

#include <stdio.h>
#include <math.h>

#include "image.h"

static unsigned short *rs;
static unsigned short *gs;
static unsigned short *bs;

int savescreen(char *name,int x1,int x2,int y1,int y2,unsigned int parray[]);

int savescreen(char *name,int x1,int x2,int y1,int y2,unsigned int parray[])
{

  IMAGE *oimage;
  int xsize, ysize;
  int xorg, yorg;
  int y, i, ii;

  int writeerr;

  unsigned int *uiparray;
  
  
  if(!name || !parray)return 1;

   uiparray = (unsigned int *)parray;

   xorg = (x1 < x2) ? x1 : x2;
   yorg = (y1 < y2) ? y1 : y2;
   if (xorg < 0) xorg = 0;
   if (yorg < 0) yorg = 0;
   xsize = x2 - x1;
   ysize = y2 - y1;
   if (xsize < 0) xsize = -xsize;
   if (ysize < 0) ysize = -ysize;
   xsize++;
   ysize++;

   writeerr = 1;

	rs=(unsigned short *)cMalloc(sizeof(unsigned short)*xsize,8794);
	gs=(unsigned short *)cMalloc(sizeof(unsigned short)*xsize,8795);
	bs=(unsigned short *)cMalloc(sizeof(unsigned short)*xsize,8796);
	
	if(!rs || !gs || !bs)goto ErrorOut;

   if ((oimage = iopen(name,"w",RLE(1),3,xsize,ysize,3)) == NULL)
     {
         sprintf(WarningBuff, "Error - unable to open rgb output file!\n");
         WarningBatch(WarningBuff);
         goto ErrorOut2;
     }

   writeerr = 0;


   for (y = 0; y < ysize; y++)
     {
      for (i = 0; i < xsize; i++)
        {
         ii = (ysize-y-1)*xsize + i;
         rs[i] = (short) ((uiparray[ii] >> 24) & 255);
         gs[i] = (short) ((uiparray[ii] >> 16) & 255);
         bs[i] = (short) ((uiparray[ii] >> 8) & 255);
        }

      if (putrow(oimage,rs,y,0) != xsize) writeerr = 1;
      if (putrow(oimage,gs,y,1) != xsize) writeerr = 1;
      if (putrow(oimage,bs,y,2) != xsize) writeerr = 1;
     }

ErrorOut2:

   if (iclose(oimage) < 0)
     {
      writeerr = 1;
     }
  
ErrorOut:
	if(rs)cFree((char *)rs);
	if(gs)cFree((char *)gs);
	if(bs)cFree((char *)bs);
	
	return writeerr;
}

