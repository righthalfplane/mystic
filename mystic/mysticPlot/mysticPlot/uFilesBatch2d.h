#ifndef __UFILESBATCH2D__
#define __UFILESBATCH2D__

#define USE_XY			0
#define USE_X			1
#define USE_Y 			2


#define max(x1,x2)    (((x1) > (x2)) ? (x1) : (x2))
#define min(x1,x2)    (((x1) < (x2)) ? (x1) : (x2))

struct DrawInfo{
    struct FilePIOInfo *pio;
	struct areadata *ai;
	double dxset,dyset;
	double odxset,odyset;
    double rx,ry,sx,sy;
	double p1x,p1y,v1x,v1y,v2x,v2y,d,od;
	double l1,l2;
	double drHalf,dsHalf;
	double dr,ds;
	long nx,ny;
	double *sout;
};


struct dRange{
      double xmin,xmax,ymin,ymax,dxlim,dylim;
};

int doSage2DGetDataBlocks(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

#endif
